*This project has been created as part of the 42 curriculum by svaladar.*

<p align="center">
  <img src="https://raw.githubusercontent.com/ayogun/42-project-badges/main/badges/minishellm.png" alt="minishell badge"/>
</p>

# minishell

## Description

**minishell** is a simplified version of `bash`, written in C. It reads a command line, splits it into tokens, builds a syntax tree, expands variables and runs the commands as processes connected by pipes and redirections, while handling signals the way bash does.

### Features

**Mandatory**
- Interactive prompt with **history** (through `readline`)
- Runs binaries found in `PATH`, or given by a relative or absolute path
- Single quotes `'` (no interpretation) and double quotes `"` (only `$` is expanded)
- Redirections:
  - `<` input
  - `>` output
  - `>>` output in append mode
  - `<<` heredoc (variables are expanded when the delimiter is not quoted)
- Pipes `|`
- Environment variables `$VAR` and the exit status `$?`
- `ctrl-C`, `ctrl-D` and `ctrl-\` behave like in bash

**Built-ins**

| Command | Description |
| :-- | :-- |
| `echo` | With the `-n` option |
| `cd` | Relative or absolute path |
| `pwd` | No options |
| `export` | No options |
| `unset` | No options |
| `env` | No options or arguments |
| `exit` | No options |

**Bonus**
- Logical operators `&&` and `||`
- Parentheses `( )` for priority (run in a subshell)
- Wildcard `*` in the current directory

### Architecture

Each input line goes through four stages:

```
 input ─► lexer ─► parser ─► expander ─► executor
          tokens    AST       words       fork / pipe / execve
```

1. **Lexer**: splits the line into words and operators (`|`, `<`, `>>`, `&&`, `(`...), keeping track of quotes.
2. **Parser**: builds an **AST**. Pipes and logical operators become nodes, commands with their redirections become leaves, and syntax errors are reported here.
3. **Expander**: replaces `$VAR` and `$?`, removes quotes and expands `*`.
4. **Executor**: walks the tree. A built-in that runs on its own executes in the shell process itself, so `cd`, `export`, `unset` and `exit` can change the shell's state. Every other command is forked and run with `execve`. All heredocs are read before execution starts.

### Structure

```
.
├── include/minishell.h
├── libft/            # my own library (git submodule)
└── src/
    ├── main.c
    ├── shell/        # main loop, prompt and history
    ├── env/          # environment variable list
    ├── lexer/        # tokenization
    ├── parser/       # AST construction
    ├── expander/     # $VAR, $?, quotes and wildcards
    ├── heredoc/      # heredoc reading and expansion
    ├── executor/     # execution, pipes, redirections and PATH lookup
    ├── builtins/     # built-in commands
    ├── signals/      # signal handling
    └── utils/        # exit and cleanup
```

## Instructions

### Requirements

`cc`, `make` and the `readline` library (on Debian/Ubuntu: `sudo apt install libreadline-dev`).

### Cloning

[libft](https://github.com/Soppinha/libft) is a submodule, and it has [ft_printf](https://github.com/Soppinha/ft_printf) and [get_next_line](https://github.com/Soppinha/GNL) as submodules of its own, so clone recursively:

```bash
git clone --recurse-submodules https://github.com/Soppinha/Minishell.git
cd Minishell
```

If the repository was cloned without the submodules:

```bash
git submodule update --init --recursive
```

### Compilation

```bash
make          # builds libft and minishell
make bonus    # same as make: the bonus is built in
make clean    # removes the object files
make fclean   # also removes the binary and libft.a
make re       # rebuilds everything
```

### Usage

```bash
./minishell
```

```bash
minishell$ echo "Hello, $USER" | tr a-z A-Z > out.txt
minishell$ cat << EOF
> line 1
> EOF
minishell$ ls *.c && echo ok || echo failed
minishell$ (cd /tmp && pwd) || echo $?
```

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html), especially *Shell Operation*, *Quoting* and *Redirections*
- `man 3 readline`, `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 dup2`, `man 2 waitpid`, `man 7 signal`
- [Writing your own shell – Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- [Abstract syntax tree – Wikipedia](https://en.wikipedia.org/wiki/Abstract_syntax_tree)

### AI usage

AI was used to draft this README from the existing source code. The implementation was written by the author.
