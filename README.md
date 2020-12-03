# CppLox

An implementation of *jlox* from Part II of [Crafting Interpreters](https://www.craftinginterpreters.com/) written in C++. Designed to be able to follow along with the book, replacing the inline Java code.

*Crafting Interpreters* is a user-friendly, free, online book which teaches you how to create a scripting language from scratch. *jlox* is the book's implementation of the book's language *Lox* using a tree-walker interpreter, written in Java. This is a C++ version of that interpreter.


# Building

Run `make` or `make jlox` to compile the program.

Run `make test-lex` to pass test-lex.lox as input to the program and diff with test-lex.lox.expected. Run `make test-parse` to do the same with test-parse.lox and test-parse.lox.expected.

Run `make generate_ast` to compile GenerateAst.cpp.

Run `make ast_printer` to compile AstPrinterDriver.cpp.


# Current Progress

Finished through Chapter 6.


# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.
