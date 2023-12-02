# Main user-facing CMD script

import os
import re
import sys
import shutil
import tarfile
import time
import traceback
import zipfile
import argparse
import requests
import platform
from subprocess import Popen, PIPE
from datetime import datetime
from io import TextIOBase
from typing import IO, TextIO


# Some day this will probably change and need updating.
PROTOLOGIC_ZIP = "https://github.com/Protologic/Release/archive/refs/heads/master.zip"
PROTOLOGIC_REPO_API = "https://api.github.com/repos/Protologic/Release"

OS = platform.system()

if getattr(sys, "frozen", False):
    PROTOLUA_PATH = os.path.dirname(sys.executable)
elif __file__:
	PROTOLUA_PATH = os.path.dirname(os.path.abspath(__file__))

TOOLS_PATH = os.path.join(PROTOLUA_PATH, "tools")
WASM_PATH = os.path.join(PROTOLUA_PATH, "build", "protolua.wasm")
TEMPLATE_PATH = os.path.join(PROTOLUA_PATH, "lua_template")
TYPING_PATH = os.path.join(PROTOLUA_PATH, "lua_typing")

def get_bin_path(bin_name: str, tools_path: "list[str]"):
	path = os.path.join(TOOLS_PATH, *tools_path, bin_name)
	if os.path.exists(path):
		return path
	if os.path.exists(path + ".exe"):
		return path + ".exe"
	return shutil.which(bin_name)

WASM_OPT_BIN = get_bin_path("wasm-opt", ["binaryen", "bin"])
WIZER_BIN = get_bin_path("wizer", ["wizer"])
WASM_2_WAT_BIN = get_bin_path("wasm2wat", ["wabt", "bin"])
PROTOLOGIC_SIM_BIN = get_bin_path("Protologic.Terminal", ["protologic", "Sim", OS])
PROTOLOGIC_PLAYER_BIN = get_bin_path("SaturnsEnvy", ["protologic", "Player", OS])


def get_gh_releases(owner: str, repo: str) -> "list[dict]":
	response = requests.get(f"https://api.github.com/repos/{owner}/{repo}/releases")
	response.raise_for_status()
	return response.json()


def download(url: str, out: str):
	print(f"Downloading {url}")
	data_request = requests.get(url)
	data_request.raise_for_status()
	with open(out, "wb") as f:
		f.write(data_request.content)


def download_gh_release(owner, repo, asset_name: str, out_dir: str, os_map: "dict[str, str]"):
	if OS not in os_map:
		print(f"Failed to find asset for '{owner}/{repo}' with name '{asset_name}' and '{os_map[OS]}' (OS not supported)", file=sys.stderr)
		exit(-1)
	release = next(
		(
			release for release in get_gh_releases(owner, repo)
				if not release["prerelease"]
			),
		None
		)
	if release is None:
		print(f"Failed to find release for '{owner}/{repo}'", file=sys.stderr)
		exit(-1)
	asset = next(
		(
			asset for asset in release["assets"]
				if asset_name in asset["name"] and os_map[OS] in asset["name"] and "sha256" not in asset["name"]
			),
		None
		)
	if asset is None:
		print(f"Failed to find asset for '{owner}/{repo}' with name '{asset_name}' and '{os_map[OS]}'", file=sys.stderr)
		exit(-1)
	download(asset["browser_download_url"], os.path.join(out_dir, asset["name"]))
	return asset


def extract_archive(file: str, out: str):
	print(f"Extracting {file}")
	if os.path.isdir(out):
		shutil.rmtree(out)
	if file.endswith(".zip"):
		with zipfile.ZipFile(file, "r") as zip:
			zip.extractall(out)
	elif ".tar" in file:
		with tarfile.open(file, "r:*") as tar:
			tar.extractall(out)
	else:
		print(f"Unknown archive file format {file}", file=sys.stderr)
		exit(-1)
	out_contents = os.listdir(out)
	if len(out_contents) == 1 and os.path.isdir(os.path.join(out, out_contents[0])):
		old_dir = os.path.join(out, out_contents[0])
		for filename in os.listdir(old_dir):
			shutil.move(os.path.join(old_dir, filename), out)
		os.rmdir(old_dir)


def ensure_tool(owner: str, repo: str, os_map: "dict[str, str]"):
	if not os.path.isdir(TOOLS_PATH):
		os.mkdir(TOOLS_PATH)

	path = os.path.join(TOOLS_PATH, repo)
	if not os.path.isdir(path):
		asset = download_gh_release(owner, repo, repo, TOOLS_PATH, os_map)
		extract_archive(os.path.join(TOOLS_PATH, asset["name"]), path)
		os.remove(os.path.join(TOOLS_PATH, asset["name"]))
	return path


def update_protologic():
	try:
		if PROTOLOGIC_SIM_BIN is not None:
			local_updated = datetime.utcfromtimestamp(os.path.getmtime(PROTOLOGIC_SIM_BIN))
			repo_request = requests.get(PROTOLOGIC_REPO_API)
			repo_request.raise_for_status()
			repo_updated = datetime.fromisoformat(repo_request.json()["pushed_at"][:-1])
			if local_updated > repo_updated:
				print("Protologic update not found.")
				return
		out_zip = os.path.join(TOOLS_PATH, "protologic.zip")
		download(PROTOLOGIC_ZIP, out_zip)
		extract_archive(out_zip, os.path.join(TOOLS_PATH, "protologic"))
		os.remove(out_zip)
	except Exception as e:
		traceback.print_exc()
		print("Failed to download & extract protologic sim & player. (Skipped)")


def file_replace_content(file: str, subs: "dict[str, str]"):
	if not os.path.exists(file):
		print("Failed to find file '{file}' for content replacement.", file=sys.stderr)
		return
	with open(file, "r") as f:
		data = f.read()
	rep = dict((re.escape(k), v) for k, v in subs.items()) 
	data = re.sub("|".join(rep.keys()), lambda m: rep[re.escape(m.group(0))], data)
	with open(file, "w") as f:
		f.write(data)


def protolua_project_create(name: str, replace=False):
	if os.path.isdir(name):
		if replace:
			shutil.rmtree(name)
		else:
			print(f"Directory already exists '{name}'", file=sys.stderr)
			exit(-1)
	shutil.copytree(TEMPLATE_PATH, name)
	file_replace_content(os.path.join(name, ".vscode", "settings.json"), {
		"$PROTOLUA_TYPING_PATH": TYPING_PATH,
	})
	print(f"Project created '{name}'")


def protolua_project_build(out: str, optimization: int, wat=False):
	print("~ Running wizer")
	if Popen([
		WIZER_BIN,
		WASM_PATH,
		"-o", out,
		"--allow-wasi",
		"--mapdir", "/::./lua/",
		"--wasm-simd", "true",
		"--wasm-bulk-memory", "true",
	]).wait() != 0:
		exit(-1)
	print("~ Running Bineryen wasm-opt")
	if Popen([
		WASM_OPT_BIN,
		out,
		"-o", out,
		"--strip-dwarf",
		"--enable-bulk-memory",
		"--enable-simd",
		f"-O{optimization}",
	]).wait() != 0:
		exit(-1)
	if wat:
		print("~ Running WABT wasm2wat")
		if WASM_2_WAT_BIN is None:
			print("Skipped wasm2wat: Unable to find wasm2wat", file=sys.stderr)
		else:
			wat_out = out.replace(".wasm", ".wat")
			if not wat_out.endswith(".wat"):
				wat_out += ".wat"
			if Popen([
				WASM_2_WAT_BIN,
				out,
				"-o", wat_out,
			]).wait() != 0:
				exit(-1)
	print(f"Project build to '{out}'")


def protolua_sim(fleets: "list[str]", replay_out: str, log: str="sim.log"):
	print(f"~ Simulating {fleets} -> {replay_out} & {log}")
	if PROTOLOGIC_SIM_BIN is None:
		print("ProtoLogic sim not found (Is it supported on {OS}? try '--update')", file=sys.stderr)
		exit(-1)
	log_f = open(log, "w")
	p = Popen([
		PROTOLOGIC_SIM_BIN,
		"--debug", "true", "false",
		"--output", os.path.abspath(replay_out.replace('.json.deflate', '')),
		"-f", *[os.path.abspath(fleet) for fleet in fleets]
	], stdout=PIPE, stderr=PIPE)
	for line in p.stdout:
		line = line.decode("utf-8")
		sys.stdout.write(line)
		log_f.write(line)
	for line in p.stderr:
		line = line.decode("utf-8")
		sys.stderr.write(line)
		log_f.write(line)
	code = p.wait()
	log_f.close()
	if code != 0:
		exit(-1)


def protolua_play(replay_path: str):
	print(f"~ Playing {replay_path}")
	if PROTOLOGIC_PLAYER_BIN is None:
		print("ProtoLogic player not found (Is it supported on {OS}? try '--update')", file=sys.stderr)
		exit(-1)
	if Popen([
		PROTOLOGIC_PLAYER_BIN,
		os.path.abspath(replay_path)
	]).wait() != 0:
		exit(-1)


if __name__ == "__main__":
	args_parser = argparse.ArgumentParser(
		prog="protolua",
		description="ProtoLua command-line tool."
	)
	args_parser.add_argument("--no-tools", action="store_true", help="Do not download tools if they are missing")
	args_parser.add_argument("--update", action="store_true", help="Update protologic (not other tools)")
	args_parser_actions = args_parser.add_subparsers(dest="action", required=True)

	args_parser_create = args_parser_actions.add_parser("create", help="Create new protolua project.")
	args_parser_create.add_argument("name")
	args_parser_create.add_argument("--delete", action="store_true", help=argparse.SUPPRESS)

	args_parser_build = args_parser_actions.add_parser("build", help="Builds a protolua project.")
	args_parser_build.add_argument("-o", "--out", help="Name to output as")
	args_parser_build.add_argument("--fast", action="store_true", help="Skips bineryen wasm-opt optimization step.")
	args_parser_build.add_argument("--wat", action="store_true", help="Runs bineryen wasm2wat.")
	args_parser_build.add_argument("--sim", action="store_true", help="After building, run the sim.")
	args_parser_build.add_argument("--play", action="store_true", help="After building, run the player.")


	args = args_parser.parse_args()

	if not args.no_tools:
		ensure_tool("WebAssembly", "binaryen", {"Windows": "x86_64-windows", "Linux": "x86_64-linux"})
		ensure_tool("bytecodealliance", "wizer", {"Windows": "x86_64-windows", "Linux": "x86_64-linux"})
	if args.update or PROTOLOGIC_SIM_BIN is None:
		print("~ Downloading protologic sim & player.", "(because sim was not found)" if PROTOLOGIC_SIM_BIN is None else "(requested update check)")
		update_protologic()

	if args.action == "create":
		protolua_project_create(args.name, args.delete)
	elif args.action == "build":
		ship_wasm = args.out if args.out else "ship.wasm"
		protolua_project_build(
			out=ship_wasm,
			optimization=0 if args.fast else 4,
			wat=args.wat
		)
		if args.sim:
			protolua_sim([ship_wasm, ship_wasm], "test.json.deflate")
			if args.play:
				protolua_play("test.json.deflate")
	else:
		raise Exception(f"Unhandled arg action {args.action}")
