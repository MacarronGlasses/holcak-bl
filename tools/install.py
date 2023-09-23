#!/bin/python3
from sys import argv
from os import path

if len(argv) < 4:
	print(f"Usage: {argv[0]} <image.img> <stage1.bin> <stage2.bin>")
	print("Error: Not enough arguments were provided!")
	exit(1);
with open(argv[1], "rb+") as f:
	size = path.getsize(argv[2])
	assert size == 0x200
	f.write(open(argv[2], "rb").read(0x1B0))
	size = path.getsize(argv[3])
	assert size < 0x7EFFF
	f.write(int(0x01).to_bytes(6, 'little'))
	f.write(int((size + 0x1FF) // 0x200).to_bytes(2, 'little'))
	f.seek(0x200)
	f.write(open(argv[3], "rb").read())
