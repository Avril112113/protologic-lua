# Main user-facing CMD script

import os
import re
import stat
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


# Must be in GH release name.
VERSION = "0.1.8"


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

PROTOLOGIC_PATH = os.path.join(TOOLS_PATH, "protologic")
PROTOLOGIC_UPDATEDAT_PATH = os.path.join(PROTOLOGIC_PATH, "updatedat.txt")

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

PROTOLUA_UPDATE_PATH = PROTOLUA_PATH
if os.path.exists("./CMakeLists.txt"):
	PROTOLUA_UPDATE_PATH = "update_test"
	if not os.path.isdir(PROTOLUA_UPDATE_PATH):
		os.mkdir(PROTOLUA_UPDATE_PATH)


def get_gh_releases(owner: str, repo: str, count=100, page=1) -> "list[dict]":
	response = requests.get(f"https://api.github.com/repos/{owner}/{repo}/releases?per_page={count}&page={page}")
	response.raise_for_status()
	return response.json()


def download(url: str, out: str):
	print(f"Downloading {url}")
	data_request = requests.get(url)
	data_request.raise_for_status()
	with open(out, "wb") as f:
		f.write(data_request.content)


def download_gh_release(owner, repo, asset_name: str, out_dir: str, os_map: "dict[str, str]"=None, prerelease=False) -> dict:
	if os_map is not None and OS not in os_map:
		print(f"Failed to find asset for '{owner}/{repo}' with name '{asset_name}' and '{os_map[OS]}' (OS not supported)", file=sys.stderr)
		exit(-1)
	release = next(
		(
			release for release in get_gh_releases(owner, repo, count=5)
				if prerelease or not release["prerelease"]
		),
		None
	)
	if release is None:
		print(f"Failed to find release for '{owner}/{repo}'", file=sys.stderr)
		exit(-1)
	asset = next(
		(
			asset for asset in release["assets"]
				if (asset_name is None or asset_name in asset["name"]) and (os_map is None or os_map[OS] in asset["name"]) and "sha256" not in asset["name"]
		),
		None
	)
	if asset is None:
		print(f"Failed to find asset for '{owner}/{repo}' with name '{asset_name}' and '{os_map[OS]}'", file=sys.stderr)
		exit(-1)
	download(asset["browser_download_url"], os.path.join(out_dir, asset["name"]))
	return asset


def extract_archive(file: str, out: str) -> "list[str]":
	print(f"Extracting {file}")
	root_extracted = []
	if out not in file and os.path.isdir(out):
		shutil.rmtree(out)
	if file.endswith(".zip"):
		with zipfile.ZipFile(file, "r") as zip:
			for member in zip.filelist:
				root_dir = member.filename[:member.filename.find("/")]
				if root_dir not in root_extracted:
					root_extracted.append(root_dir)
			zip.extractall(out)
	elif ".tar" in file:
		with tarfile.open(file, "r:*") as tar:
			for member in tar.getmembers():
				root_dir = member.name[:member.name.find("/")]
				if root_dir not in root_extracted:
					root_extracted.append(root_dir)
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
	return root_extracted


def ensure_tool(owner: str, repo: str, os_map: "dict[str, str]"):
	if not os.path.isdir(TOOLS_PATH):
		os.mkdir(TOOLS_PATH)

	path = os.path.join(TOOLS_PATH, repo)
	if not os.path.isdir(path):
		asset = download_gh_release(owner, repo, repo, TOOLS_PATH, os_map)
		archive_path = os.path.join(TOOLS_PATH, asset["name"])
		extract_archive(archive_path, path)
		os.remove(archive_path)
	return path


def update_protologic():
	out_zip = os.path.join(TOOLS_PATH, "protologic.zip")
	
	try:
		repo_request = requests.get(PROTOLOGIC_REPO_API)
		repo_request.raise_for_status()
		pushed_at = repo_request.json()["pushed_at"]

		if os.path.exists(PROTOLOGIC_UPDATEDAT_PATH):
			with open(PROTOLOGIC_UPDATEDAT_PATH, "r") as f:
				if f.read() == pushed_at:
					print("No protologic sim & player update found.")
					return
				else:
					print("Found protologic update.")

		download(PROTOLOGIC_ZIP, out_zip)
		if os.path.exists(PROTOLOGIC_PATH):
			shutil.rmtree(PROTOLOGIC_PATH)
		extract_archive(out_zip, PROTOLOGIC_PATH)
		os.remove(out_zip)
		with open(PROTOLOGIC_UPDATEDAT_PATH, "w") as f:
			f.write(pushed_at)
		
		PROTOLOGIC_SIM_BIN = get_bin_path("Protologic.Terminal", ["protologic", "Sim", OS])
		PROTOLOGIC_PLAYER_BIN = get_bin_path("SaturnsEnvy", ["protologic", "Player", OS])
		if OS != "Windows":
			if PROTOLOGIC_SIM_BIN is not None:
				os.chmod(PROTOLOGIC_SIM_BIN, os.stat(PROTOLOGIC_SIM_BIN).st_mode | stat.S_IEXEC)
			if PROTOLOGIC_PLAYER_BIN is not None:
				os.chmod(PROTOLOGIC_PLAYER_BIN, os.stat(PROTOLOGIC_PLAYER_BIN).st_mode | stat.S_IEXEC)
	except Exception as e:
		traceback.print_exc()
		print("Failed to download & extract protologic sim & player.", file=sys.stderr)


def update_protolua():
	release = get_gh_releases("Avril112113", "protologic-lua", count=1)[0]
	if VERSION in release["name"]:
		print("No protolua update found.")
		return
	# Due to the protologic binary clashing with the directory name of the update, we need another folder...
	tmp_path = os.path.join(PROTOLUA_UPDATE_PATH, "update")
	if os.path.exists(tmp_path):
		shutil.rmtree(tmp_path)
	os.mkdir(tmp_path)
	asset = download_gh_release("Avril112113", "protologic-lua", None, tmp_path, prerelease=True)
	archive_path = os.path.join(tmp_path, asset["name"])
	dir = extract_archive(archive_path, tmp_path)[0]
	os.remove(archive_path)
	src_path = os.path.abspath(os.path.join(tmp_path, dir))
	dst_path = os.path.abspath(PROTOLUA_UPDATE_PATH)
	print(f"!! Auto-update feature is currently limited.\nPlease manually copy the contents of\n     '{src_path}'\ninto '{dst_path}'")
	# TODO: Automatically move contents of src_path -> dst_path


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
		"$PROTOLUA_TYPING_PATH": TYPING_PATH.replace("\\", "\\\\"),
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
		print("ProtoLogic sim not found (Is it supported on {OS}? try 'protolua update')", file=sys.stderr)
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
		print("ProtoLogic player not found (Is it supported on {OS}? try 'update')", file=sys.stderr)
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
	args_parser_actions = args_parser.add_subparsers(dest="action", required=True)

	args_parser_create = args_parser_actions.add_parser("version", help="Gets the current version of protolua.")

	args_parser_create = args_parser_actions.add_parser("create", help="Create new protolua project.")
	args_parser_create.add_argument("name")
	args_parser_create.add_argument("--delete", action="store_true", help=argparse.SUPPRESS)

	args_parser_build = args_parser_actions.add_parser("build", help="Builds a protolua project.")
	args_parser_build.add_argument("-o", "--out", help="Name to output as")
	args_parser_build.add_argument("--fast", action="store_true", help="Skips bineryen wasm-opt optimization step.")
	args_parser_build.add_argument("--wat", action="store_true", help="Runs bineryen wasm2wat.")
	args_parser_build.add_argument("--sim", action="store_true", help="After building, run the sim.")
	args_parser_build.add_argument("--play", action="store_true", help="After building, run the player.")

	args_parser_update = args_parser_actions.add_parser("update", help="Update protolua & protologic.")


	args = args_parser.parse_args()

	if not args.no_tools:
		ensure_tool("WebAssembly", "binaryen", {"Windows": "x86_64-windows", "Linux": "x86_64-linux"})
		ensure_tool("bytecodealliance", "wizer", {"Windows": "x86_64-windows", "Linux": "x86_64-linux"})
	if PROTOLOGIC_SIM_BIN is None and args.action != "update":
		print("~ Downloading protologic sim & player. (sim binary was not found)")
		update_protologic()

	if args.action == "version":
		print(f"protolua version: {VERSION}")
	elif args.action == "update":
		print("~ Updating protologic sim & player.")
		update_protologic()
		print("~ Updating protolua.")
		update_protolua()
	elif args.action == "create":
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
