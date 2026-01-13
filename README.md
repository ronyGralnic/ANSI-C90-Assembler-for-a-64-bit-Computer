# ANSI Assembler 🧩  
**A full two-pass assembler written in ANSI C for a custom 14-bit virtual machine**

---

The assembler translates a custom-designed assembly language into machine code for a simulated 14-bit computer.  
It includes a macro preprocessor, symbol table management, error handling, and full instruction encoding.

This project emphasizes **system-level programming**, **low-level architecture understanding**, and **clean modular C design**.



📌 Overview
---

The goal of this project was to design and implement an **assembler** for a custom CPU architecture.

The assembler receives one or more assembly source files and produces:

- Machine code files  
- Entry label files  
- External reference files  
- Detailed error messages

The project follows strict **ANSI C** standards and is compiled with:

```bash
gcc -ansi -pedantic -Wall

```

The system simulates a computer with:

- 14-bit words
- 4096 memory cells
- 8 general-purpose registers
- Custom instruction set
- Multiple addressing modes



✨ Key Features
-

- 🧠 Two-pass assembler architecture

- 🧩 Macro preprocessor (macro expansion stage)

- 🗂️ Symbol table with extern & entry support

- ⚙️ Full instruction encoding to 14-bit machine words

- 🧮 Custom base-4 encrypted output format

- 📚 Support for directives: `.data`, `.string`, `.extern`, `.entry`, `.define`.

- 🧱 Support for arrays and index addressing

- 🚦 Extensive syntax & semantic error detection

- 🧼 Clean modular design (separated by responsibility).

📄Output Files:
 -

- Object file `(.ob)`

- Entries file `(.ent)`

- Externals file `(.ext)`




🏗️ Architecture
-
1. The assembler is built in clear processing stages:

- **Pre-Assembler**

- Expands macros

- Produces an extended source file

2. **First Pass***

- Builds the symbol table

- Counts instruction & data memory

- Validates syntax structure

3. **Second Pass**

- Encodes machine instructions

- Resolves labels

- Handles externals & entries

4. **Output Generation**

- Encodes memory image

- Writes output files

- The system uses structured modules for:

- Parsing

- Symbol management

- Instruction encoding

- Error handling

- File generation

---

🛠️ Tech Stack
-
- Language: ANSI C

- Compiler: GCC

- Build System: Makefile

- Environment: Linux (Ubuntu compatible)



⚙️ Compilation
-
```
make
```


Or manually:
```
gcc -ansi -pedantic -Wall *.c -o assembler
```


▶️ Usage
```bash
./assembler file1.as file2.as
```

The assembler will generate output files for each valid source file.

🧪 Example Capabilities
-
- The assembler supports:

- Immediate, direct, index, and register addressing

- Macro definitions and expansion

- Arrays and constants

- External labels and multi-file linking preparation

- Detailed error reports (line-based)

📜 License
-
This project was developed for academic and educational purposes.
