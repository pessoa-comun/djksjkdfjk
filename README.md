# MIPS Assembler 2nd Edition Ver. 1.01

This project is an assembler for a subset of the **MIPS** (Microprocessor without Interlocked Pipelined Stages) architecture, implemented in the **C** language. It is the second version of an assembler originally developed for an exam in my **Computer Organization and Architecture I** class.

## Building the Assembler

To build the assembler, simply use the provided Makefile. After building, an executable named **Assembler** will be created.

## Using the Assembler

The assembler requires two command-line arguments: the source file and the output file. The source file must have a **.asm** extension.

### Running the Assembler

```sh
./Assembler sourceFile.asm binaryFile
```

This command will create a file named **binaryFile** containing the translated source code.

If you only provide the source file:

```sh
./Assembler sourceFile.asm
```

The assembler will create a file named **output.dat** with the binary output.

## Features

- **Labels**: Supports rudimentary interpretation of labels for addressing jumps and branches.
- **Error Handling**: Provides basic feedback on syntax and semantic errors.

## List of Future Improvements

- **Better Error Handling**: Enhance the feedback for syntax and semantic errors.
- **Improved Label Implementation**: Use the `regex.h` library for more robust label parsing.
- **Support for All MIPS Instructions**: Extend the assembler to handle the full set of MIPS instructions.

## References

This assembler was developed with the assistance of the book *Computer Organization and Architecture* by David Patterson, the [Mars45 simulator](https://courses.missouristate.edu/kenvollmar/mars/download.htm), and the [CSFG Web Assembler](https://www.csfieldguide.org.nz/en/interactives/mips-assembler/).
