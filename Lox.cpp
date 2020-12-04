// An implementation of the Lox language in C++
// http://www.craftinginterpreters.com
// https://github.com/munificent/craftinginterpreters/tree/master/java/com/craftinginterpreters/lox

#include <fstream>      // readFile
#include <iostream>     // std::getline
#include <string>
#include <vector>
#include "Error.h"
#include "AstPrinter.h"
#include "Interpreter.h"
#include "Parser.h"
#include "Scanner.h"

const Interpreter interpreter{};

void run(std::string_view source) {
  Scanner scanner {source};
  std::vector<Token> tokens = scanner.scanTokens();

  // Scanning - Chapter 4
  // For now, just print the tokens.
  // for (Token token : tokens) {
  //   std::cout << token.toString() << "\n";
  // }

  Parser parser {tokens};
  Expr* expression = parser.parse();

  // Stop if there was a syntax error.
  if (hadError) return;

  // Parsing Expressions - Chapter 6
  // std::cout << AstPrinter{}.print(expression) << "\n";

  interpreter.interpret(expression);
}

// source: http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string readFile(const char* path) {
  // Open file
  std::ifstream file (path, std::ios::in | std::ios::binary |
                            std::ios::ate);
  if (!file) throw (errno);

  // Create string of sufficient size
  std::string contents;
  contents.resize(file.tellg());

  // Read complete file into string
  file.seekg(0, std::ios::beg);
  file.read(&contents[0], contents.size());

  return contents;
}

void runFile(const char* path) {
  std::string contents = readFile(path);
  run(contents);

  // Indicate an error in the exit code.
  if (hadError) std::exit(65);
  if (hadRuntimeError) std::exit(70);
}

void runPrompt() {
  for (;;) {
    std::cout << "> ";
    std::string line;
    if (!std::getline(std::cin, line)) break;

    run(line);
    hadError = false;
  }
}

int main(int argc, char* argv[]) {
  if (argc > 2) {
    std::cout << "Usage: jlox [script]";
    std::exit(64);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}