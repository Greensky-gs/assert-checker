# Assert checker

This is a simple assertion checker written in C.

## Installation

Follow the installation steps for your platform :

- [Linux](#linux)
- [Windows](#windows)

MacOS is not listed here, because I never tried it. Please propose changes if you tested the installation on MacOS

### Linux

1. Install dependencies, that are : `git`, `gcc` and `make`. For example, on debian-based distributions : `sudo apt install git gcc make`
2. Get source code (using git) : `git clone https://git.greensky.tf/Greensky/assert-checker && cd assert-checker`
3. Compile the code to an executable : `make`
4. You can now use the `main.exe` file, by following the [usage instructions](#usage)

> Run the command listed above in a terminal

### Windows

1. Install dependencies : `git`, `gcc` and `make` through regular installation, or using winget :
```
winget install -e --id Arm.GnuArmEmbeddedToolchain
winget install -e --id GnuWin32.Make
winget install -e --id Git.Git
```
2. Get source code (using git) : `git clone https://git.greensky.tf/Greensky/assert-checker && cd assert-checker`
3. Compile the code to an executable : `make`
4. You can now use the `main.exe` file, by following the [usage instructions](#usage)

## Usage

Once you have the `main.exe` file executable, you can use it by launching the script. It will ask you to enter an expression. An expression is composed of :

- Variables, represented by single letters
- Operators
- Parenthesis, that create groups

### Operators list

| Operator | Symbol | Syntax |
|:--------:|:------:|:------:|
| Logical **and** | `&` | `X&Y` |
| Logical **or** | `\|` | `X|Y` |
| Logical **not** | `!` | `!X` |
| Logical **implication** | `-` | `X-Y` |
| Group of expression | `(` and `)` | `(X)` |

> In the above syntax description, a X, Y means a valid assertion, so it is at least a variable, and at most one or two assertions, joined by operator

### Examples

All the below examples are valid expressions to the program to parse (one per line)

```
a
B&c
(a&c)-d
(a|(b&d))|c-d
(a|b)-(c&(d|v)|a)
(((!p&s)|(g|!s))|(!t&!g))&((s|!g)&(r|s))
```

## Program complexity

If **n** is the number of **different variables**, the program has a complexity of `O(2^n)`.

Since it is written in native C, it is very fast anyway.

## Contributors

- [ @Greensky-gs ](https://github.com/Greensky-gs)
