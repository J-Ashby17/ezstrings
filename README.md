# ezstrings

A lightweight tool for searching english strings in binary files (ASCII/UTF-8 and UTF-16BE/LE).

---

## Features
- Fast string searching
- Context bytes support
- Simple command-line usage

---

## Build Instructions
With Make:
1. Download Zip
2. Open Terminal
3. Navigate to folder -> cd ezstrings-main
4. If make installed -> make
5. ./ezstrings or ezstrings.exe

Without Make:
4. gcc -o ezstrings main.c parser.c search.c
5. ./ezstrings or ezstrings.exe
*can use clang or MSVC if no gcc

---
