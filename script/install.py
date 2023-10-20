#!/bin/python3
from sys import argv
from os import path

# TODO: Add support for GPT

if len(argv) < 3:
	print(f"Usage: {argv[0]} <image> <build>")
	print("    <image> - Existing raw file with size")
	print("    <build> - Build directory with binaries")
	print("Error: Insufficient number of arguments were provided!")
	exit(1)
with open(argv[1], "rb+") as f:
	size = path.getsize(f"{argv[2]}/stage1/build.bin")
	assert size == 0x200
	f.write(open(f"{argv[2]}/stage1/build.bin", "rb").read(0x1B0))
	size = path.getsize(f"{argv[2]}/stage2/build.bin")
	assert size < 0x7EFFF
	f.write(int(0x01).to_bytes(6, 'little'))
	f.write(int((size + 0x1FF) // 0x200).to_bytes(2, 'little'))
	f.seek(0x200)
	f.write(open(f"{argv[2]}/stage2/build.bin", "rb").read())
