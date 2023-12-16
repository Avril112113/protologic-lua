# Handles all protolua sim stdout utilities from `lua_template/protolua/out_utils.lua`

import re
import os
import sys
import typing


class ProtoLuaSimOutUtils:
	def __init__(self):
		self.fileAppend = None
		self.files: "dict[str,typing.IO]" = {}

	def __del__(self):
		for f in self.files.values():
			f.close()

	def handle(self, line: str) -> bool:
		line = re.sub(r"\[.*?\] \w+: ", "", line)
		if self._handle_fileAppend(line): return True
		return False
	
	def _handle_fileAppend(self, line: str) -> bool:
		if line.startswith("__PROTOLUA__:fileAppend:"):
			self.state = "fileAppend"
			split = line.split(":", 3)
			name = split[2]
			data = split[3]  # We keep the newline here.
			file_path = os.path.join("sim", name)
			if file_path.find("..") >= 0:
				print(f"Bad fileAppend name \"{name}\"", file=sys.stderr)
				return True
			if name not in self.files:
				os.makedirs(os.path.dirname(file_path), exist_ok=True)
				self.files[name] = open(file_path, "w")
			self.files[name].write(data)
			self.files[name].flush()  # Sim can take time, it can be nice to see the output live.
			return True
		return False
