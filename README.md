# CppLox

An implementation of *jlox* from Part II of [Crafting Interpreters](https://www.craftinginterpreters.com/) written in C++17. Designed to be able to follow along with the book, replacing the inline Java code.

*Crafting Interpreters* is a user-friendly, free, online book which teaches you how to create a programming language from scratch. *jlox* is the book's implementation of the book's language *Lox* using a tree-walking interpreter.

This project is meant to be an educational aid, so I encourage anyone to point out mistakes or propose improvements.


# Reading the Code

Each chapter has its own folder which contains the state of the code as it exists at the end of that chapter, as well as all the machinery to build and run it. In a few cases, early code within a chapter gets modified by later code in the same chapter; in these cases both versions are present with the earlier commented out.


# Challenges

Some challenges have been added to their chapter's folder in a subfolder *challenge#*, where # is the challenge number. The challenges contain a complete copy of the chapter's code with the appropriate changes made, so they can be run independently. All tests are included as well to ensure that no new bugs are introduced. New tests covering the challenge are named test-challenge#.lox.


# Building

Run `make` or `make jlox` to compile the program.

Run `make generate_ast` to compile GenerateAst.cpp.

Run `make ast_printer` to compile AstPrinterDriver.cpp. As specified by the book, the AST printer only works up through chapter 7. After that, it will no longer satisfy the visitor interface unless you add the missing functions to AstPrinter.h. The associated files are still included in every chapter for completeness.

Tests can be run with a variant of `make test-X` to pass test-X.lox as input to the program and diff the output with test-X.lox.expected. Most of the test code comes directly from the book. The provided tests are:

| Command            | Input                  | Expected                        | Chapters |
| ------------------ | ---------------------- | ------------------------------- | -------- |
| test-lexing        | test-lexing.lox        | test-lexing.lox.expected        | 4-5      |
| test-lexing2       | test-lexing2.lox       | test-lexing2.lox.expected       | 4-5      |
| test-parsing       | test-parsing.lox       | test-parsing.lox.expected       | 6        |
| test-expressions2  | test-expressions2.lox  | test-expressions2.lox.expected  | 7        |
| test-statements    | test-statements.lox    | test-statements.lox.expected    | 8-13     |
| test-statements2   | test-statements2.lox   | test-statements2.lox.expected   | 8-13     |
| test-statements3   | test-statements3.lox   | test-statements3.lox.expected   | 8-13     |
| test-statements4   | test-statements4.lox   | test-statements4.lox.expected   | 8-13     |
| test-statements5   | test-statements5.lox   | test-statements5.lox.expected   | 8-13     |
| test-statements6   | test-statements6.lox   | test-statements6.lox.expected   | 8-13     |
| test-control-flow  | test-control-flow.lox  | test-control-flow.lox.expected  | 9-13     |
| test-control-flow2 | test-control-flow2.lox | test-control-flow2.lox.expected | 9-13     |
| test-functions     | test-functions.lox     | test-functions.lox.expected     | 10-13    |
| test-functions2    | test-functions2.lox    | test-functions2.lox.expected    | 10-13    |
| test-functions3    | test-functions3.lox    | test-functions3.lox.expected    | 10-13    |
| test-functions4    | test-functions4.lox    | test-functions4.lox.expected    | 10-13    |
| test-resolving     | test-resolving.lox     | test-resolving.lox.expected     | 11-13    |
| test-classes       | test-classes.lox       | test-classes.lox.expected       | 12-13    |
| test-classes2      | test-classes2.lox      | test-classes2.lox.expected      | 12-13    |
| test-classes3      | test-classes3.lox      | test-classes3.lox.expected      | 12-13    |
| test-classes4      | test-classes4.lox      | test-classes4.lox.expected      | 12-13    |
| test-classes5      | test-classes5.lox      | test-classes5.lox.expected      | 12-13    |
| test-classes6      | test-classes6.lox      | test-classes6.lox.expected      | 12-13    |
| test-classes7      | test-classes7.lox      | test-classes7.lox.expected      | 12-13    |
| test-classes8      | test-classes8.lox      | test-classes8.lox.expected      | 12-13    |
| test-classes9      | test-classes9.lox      | test-classes9.lox.expected      | 12-13    |
| test-classes12     | test-classes12.lox     | test-classes12.lox.expected     | 12-13    |
| test-inheritance2  | test-inheritance2.lox  | test-inheritance2.lox.expected  | 13       |
| test-inheritance3  | test-inheritance3.lox  | test-inheritance3.lox.expected  | 13       |
| test-inheritance5  | test-inheritance5.lox  | test-inheritance5.lox.expected  | 13       |

The following tests diff the contents of stderr rather than of stdout.

| Command           | Input                 | Expected                       | Chapters |
| ----------------- | --------------------- | ------------------------------ | -------- |
| test-expressions  | test-expressions.lox  | test-expressions.lox.expected  | 7        |
| test-resolving2   | test-resolving2.lox   | test-resolving2.lox.expected   | 11-13    |
| test-resolving3   | test-resolving3.lox   | test-resolving3.lox.expected   | 11-13    |
| test-resolving4   | test-resolving4.lox   | test-resolving4.lox.expected   | 11-13    |
| test-classes10    | test-classes10.lox    | test-classes10.lox.expected    | 12-13    |
| test-classes11    | test-classes11.lox    | test-classes11.lox.expected    | 12-13    |
| test-classes13    | test-classes13.lox    | test-classes13.lox.expected    | 12-13    |
| test-inheritance  | test-inheritance.lox  | test-inheritance.lox.expected  | 13       |
| test-inheritance4 | test-inheritance4.lox | test-inheritance4.lox.expected | 13       |
| test-inheritance6 | test-inheritance6.lox | test-inheritance6.lox.expected | 13       |
| test-inheritance7 | test-inheritance7.lox | test-inheritance7.lox.expected | 13       |

Starting in chapter 8 run `make test-all` to run all tests for the chapter. This might be useful if you are modifying the code.

The following tests cover challenges or changes they introduce and are found in the challenge's *tests* subfolder.

| Command           | Input                 | Expected                       | Chapter | Stream |
| ----------------- | --------------------- | ------------------------------ | ------- | ------ |
| test-challenge2   | test-challenge2.lox   | test-challenge2.lox.expected   | 8       | stderr |
| test-statement3   | test-statements3.lox  | test-statements3.lox.expected  | 8       | stderr |


# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.
