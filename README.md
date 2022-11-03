# Shell

![Release](https://img.shields.io/badge/Release-v1.0-blueviolet)
![Language](https://img.shields.io/badge/Language-C%2B%2B-0052cf)
![Size](https://img.shields.io/badge/Size-28Ko-f12222)
![Open Source](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)

<br/>

This program is a simplified Shell running basic Unix commands.

<br/>

<p align="center">
	<img src="https://i.imgur.com/VS1gXCz.png" width="700">
</p>

<br/>

# Features

* Launch binaries :
  * Default Linux binaries (examples : `pwd`, `ls`, `cat`...).
  * Any program with `path/name` (example : `./a.out`).
  * Builtins : `cd`, `cp` and `mkdir`.

<br/>

* Process input and output redirection with `< filename`, `> filename` or `>> filename` (example : `echo hello > test.txt`).

<br/>

* Pipes with `|` (example : `printf "A\nB\nC" | grep B | cat`).

<br/>

* Process running in the background with `&` (example : `sleep 5 &`).

<br/>

# Notes

* The program only works on a Unix environment.

<br/>

# Credits

* [**Dario Nonis**](https://github.com/DarioNonis) : Creator of the project.
