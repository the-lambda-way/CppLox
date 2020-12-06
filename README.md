# CppLox

An implementation of *jlox* from Part II of [Crafting Interpreters](https://www.craftinginterpreters.com/) written in C++17. Designed to be able to follow along with the book, replacing the inline Java code.

*Crafting Interpreters* is a user-friendly, free, online book which teaches you how to create a scripting language from scratch. *jlox* is the book's implementation of the book's language *Lox* using a tree-walker interpreter, written in Java. This is a C++ version of that interpreter.


# Building

Run `make` or `make jlox` to compile the program.

Tests can be run with a variant of `make test-X` to pass test-X.lox as input to the program and diff with test-X.lox.expected. The available tests are:

| Command          | Input                | Expected                      | Chapter |
| ---------------- | -------------------- | ----------------------------- | ------- |
| test-lex         | test-lex.lox         | test-lex.lox.expected         | 4       |
| test-parse       | test-parse.lox       | test-parse.lox.expected       | 6       |
| test-expressions | test-expressions.lox | test-expressions.lox.expected | 7       |
| test-statements  | test-statements.lox  | test-statements.lox.expected  | 8       |
| test-statements2 | test-statements2.lox | test-statements2.lox.expected | 8       |

Run `make generate_ast` to compile GenerateAst.cpp and produce Expr.h and Stmt.h.

Run `make ast_printer` to compile AstPrinterDriver.cpp.


# Current Progress

Finished through Chapter 8.


# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.
