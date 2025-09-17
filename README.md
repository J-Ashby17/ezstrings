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
6. Without Make Below:
7. gcc -o ezstrings main.c parser.c search.c
8. ./ezstrings or ezstrings.exe
*can use clang or MSVC if no gcc

---

## Demonstration

Tool options
<img width="775" height="163" alt="git1" src="https://github.com/user-attachments/assets/d43dec31-de45-4614-bc12-08dc4404c722" />

Flag search w/context
<img width="750" height="237" alt="image" src="https://github.com/user-attachments/assets/e2da5e21-40d2-4f10-8832-27a3b4a59e0c" />

Unique Strings(no duplicates)
<img width="750" height="237" alt="git 3" src="https://github.com/user-attachments/assets/49939c33-2fbe-46cb-a32e-6c125ec71b1f" />


## Thoughts

Origionally I had set out to make strings lightning fast for large files, but unfortunately i failed. I wanted to impliment 1 pass through for ASCII/UTF-8 and UTF-16BE/LE to find english chars. I tried some try implimentation but the hardest part is that ASCII/UTF-8 eng chars are 1 byte while UTF-16BE/LE are two bytes. The UTF-8 and UTF-16 buffers were not able to differentiate which char belonged to who and the chars would bleed into the wrong buffer. It would return something like this:

ASCII = hellow
UTF-16 = orld

which seems to still be a problem in --parse and I havent worked out a fix. but for static analysis i think this will still be a useful tool because only a few of the words are a character off. I would rely on this tool for quick checks not for scripting.

HOWEVER, --nodupes works really well its just two pass throughs as i removed my trie implementation and the search functionality will be a great tool for ctfs.
