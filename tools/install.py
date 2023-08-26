#!/bin/python3
from sys import argv

if len(argv) < 4:
	print(f"Usage: {argv[0]} <image.img> <stage1.bin> <stage2.bin>")
	print("Error: Not enough arguments were provided!")
	exit(1);
with open(argv[1], "rb+") as f:
	f.write(open(argv[2]).read())
	
