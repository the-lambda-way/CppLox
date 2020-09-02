// An implementation of the Lox language in C++
// Implemented through chapter 4
// http://www.craftinginterpreters.com/scanning.html
// http://www.craftinginterpreters.com/representing-code.html
// https://github.com/munificent/craftinginterpreters/tree/master/java/com/craftinginterpreters/lox


#include <fstream>      // readFile
#include <iostream>     // std::getline
#include <string>
#include <vector>

#include "Scanner.h"
#include "TokenType.h"
#include "Token.h"


bool hadError = false;


void report (int line, std::string_view where, std::string_view message)
{
     std::cerr << "[line " << line << "] Error" << where << ": " << message;
     hadError = true;
}


void error (int line, std::string_view message)
{
     report(line, "", message);
}


void error (Token token, std::string message)
{
     if (token.type == TokenType::END_OF_FILE)     report(token.line, " at end", message);
     else                                          report(token.line, " at '" + toString(token.lexeme) + "'", message);
}


void run (std::string source)
{
     Scanner scanner {source};
     std::vector<Token> tokens = scanner.scanTokens();

     for (auto&& token : tokens)
          std::cout << toString(token) << "\n";
}


void runPrompt ()
{
     for (std::string line;;)
     {
          std::cout << "> ";
          if (!std::getline(std::cin, line))     break;

          run(line);
          hadError = false;
     }
}


// source: http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string readFile (const char* path)
{
     // Open file
     std::ifstream file (path, std::ios::in | std::ios::binary | std::ios::ate);
     if (!file)    throw (errno);

     // Create string of sufficient size
     std::string contents;
     contents.resize(file.tellg());

     // Read complete file into string
     file.seekg(0, std::ios::beg);
     file.read(&contents[0], contents.size());

     return contents;
}


void runFile (const char* path)
{
     run(readFile(path));
     if (hadError)     std::exit(65);
}


int main (int argc, char* argv[])
{
     if (argc >  2)
     {
          std::cout << "Usage: lox [script]";
          std::exit(64);
     }
     else if (argc == 2)     runFile(argv[1]);
     else                    runPrompt();
}
