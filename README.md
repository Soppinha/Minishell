<p align="center">
  <img src="https://raw.githubusercontent.com/ayogun/42-project-badges/main/badges/minishellm.png" alt="minishell badge"/>
</p>

<h1 align="center">minishell</h1>

<p align="center">
  <b><i>As beautiful as a shell — projeto da 42 São Paulo.</i></b>
</p>

---

## 📖 Sobre

O **minishell** é uma versão simplificada do `bash`, escrita em C. O projeto envolve leitura de linha de comando, análise léxica e sintática, expansão de variáveis, execução de processos, pipes, redirecionamentos e tratamento de sinais.

## ✨ Funcionalidades

### Obrigatórias
- Prompt interativo com **histórico** (via `readline`)
- Busca e execução de binários pelo `PATH`, ou por caminho relativo/absoluto
- Aspas simples `'` (sem interpretação) e duplas `"` (expandem apenas `$`)
- Redirecionamentos:
  - `<` entrada
  - `>` saída
  - `>>` saída em modo append
  - `<<` heredoc (com expansão de variáveis quando o delimitador não está entre aspas)
- Pipes `|`
- Variáveis de ambiente `$VAR` e status de saída `$?`
- Sinais `ctrl-C`, `ctrl-D` e `ctrl-\` com o mesmo comportamento do bash

### Built-ins
| Comando | Descrição |
| :-- | :-- |
| `echo` | Com a opção `-n` |
| `cd` | Caminho relativo ou absoluto |
| `pwd` | Sem opções |
| `export` | Sem opções |
| `unset` | Sem opções |
| `env` | Sem opções ou argumentos |
| `exit` | Sem opções |

### Bônus
- Operadores lógicos `&&` e `||`
- Parênteses `( )` para prioridade (subshell)
- Wildcard `*` no diretório atual

## 🏗️ Arquitetura

```
 entrada ─► lexer ─► parser ─► expander ─► executor
            tokens    AST       palavras     fork / pipe / execve
```

```
.
├── include/minishell.h
├── libft/            # biblioteca própria (submódulo)
└── src/
    ├── main.c
    ├── shell/        # loop principal, prompt e histórico
    ├── env/          # lista de variáveis de ambiente
    ├── lexer/        # tokenização da entrada
    ├── parser/       # construção da AST
    ├── expander/     # $VAR, $?, aspas e wildcards
    ├── heredoc/      # leitura e expansão de heredocs
    ├── executor/     # execução, pipes, redirecionamentos e PATH
    ├── builtins/     # comandos internos
    ├── signals/      # tratamento de sinais
    └── utils/        # saída e limpeza
```

## 🛠️ Compilação

Requisitos: `cc`, `make` e a biblioteca `readline`.

```bash
git clone --recurse-submodules <url-do-repositorio>
cd minishell
make
```

Se o repositório já foi clonado sem os submódulos:

```bash
git submodule update --init
```

| Regra | Ação |
| :-- | :-- |
| `make` | Compila a libft e o minishell |
| `make clean` | Remove os objetos |
| `make fclean` | Remove objetos e binários |
| `make re` | Recompila do zero |

## 🚀 Uso

```bash
./minishell
```

```bash
minishell$ echo "Olá, $USER" | tr a-z A-Z > saida.txt
minishell$ cat << EOF
> linha 1
> EOF
minishell$ ls *.c && echo ok || echo falhou
minishell$ (cd /tmp && pwd) || echo $?
```

## 📚 Bibliotecas

<p align="left">
  <a href="libft"><img src="https://raw.githubusercontent.com/ayogun/42-project-badges/main/badges/libftm.png" alt="libft badge" height="100"/></a>
</p>

Este projeto utiliza a [libft](libft), minha biblioteca própria que inclui `ft_printf` e `get_next_line`.

---

<p align="center">Feito por <a href="https://github.com/Soppinha">svaladar</a> · 42 São Paulo</p>
