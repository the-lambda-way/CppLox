# CppLox

An implementation of *jlox* from Part II of [Crafting Interpreters](https://www.craftinginterpreters.com/) written in C++17. Designed to be able to follow along with the book, replacing the inline Java code.

*Crafting Interpreters* is a user-friendly, free, online book which teaches you how to create a scripting language from scratch. *jlox* is the book's implementation of the book's language *Lox* using a tree-walking interpreter, written in Java. This is a C++ version of that interpreter.

This project is meant to be an educational aid, so I encourage anyone to point out mistakes or propose improvements.


# Building

Run `make` or `make jlox` to compile the program.

Tests can be run with a variant of `make test-X` to pass test-X.lox as input to the program and diff the output with test-X.lox.expected. The available tests are:

| Command           | Input                 | Expected                       | Chapter |
| ----------------- | --------------------- | ------------------------------ | ------- |
| test-lexing       | test-lexing.lox       | test-lexing.lox.expected       | 4       |
| test-parsing      | test-parsing.lox      | test-parsing.lox.expected      | 6       |
| test-expressions  | test-expressions.lox  | test-expressions.lox.expected  | 7       |
| test-statements   | test-statements.lox   | test-statements.lox.expected   | 8       |
| test-statements2  | test-statements2.lox  | test-statements2.lox.expected  | 8       |
| test-control-flow | test-control-flow.lox | test-control-flow.lox.expected | 9       |
| test-functions    | test-functions.lox    | test-functions.lox.expected    | 10      |
| test-functions2   | test-functions2.lox   | test-functions2.lox.expected   | 10      |
| test-functions3   | test-functions3.lox   | test-functions3.lox.expected   | 10      |
| test-resolving    | test-resolving.lox    | test-resolving.lox.expected    | 11      |

The following diff the contents of stderr rather than of stdout.

| Command           | Input                 | Expected                       | Chapter |
| ----------------- | --------------------- | ------------------------------ | ------- |
| test-resolving2   | test-resolving2.lox   | test-resolving2.lox.expected   | 11      |
| test-resolving3   | test-resolving3.lox   | test-resolving3.lox.expected   | 11      |
| test-resolving4   | test-resolving4.lox   | test-resolving4.lox.expected   | 11      |

Run `make generate_ast` to compile GenerateAst.cpp and produce Expr.h and Stmt.h.

Run `make ast_printer` to compile AstPrinterDriver.cpp.


# Current Progress

Finished through Chapter 11.


# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.
