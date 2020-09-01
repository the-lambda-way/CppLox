// An implementation of the Lox language in C++
// Implemented through chapter 4
// http://www.craftinginterpreters.com/scanning.html
// http://www.craftinginterpreters.com/representing-code.html
// https://github.com/munificent/craftinginterpreters/tree/master/java/com/craftinginterpreters/lox


#include <fstream>      // get_file_contents
#include <iomanip>      // std::setw
#include <iostream>     // getline
#include <map>          // scanner keywords
#include <memory>       // unique_ptr
#include <string>
#include <sstream>
#include <vector>

#include "Scanner.h"
#include "TokenType.h"
#include "Token.h"


bool had_error         = false;
bool had_runtime_error = false;


void report (int line, std::string_view where, std::string_view message)
{
     std::cerr << "[line " << line << "] Error" << where << ": " << message;
     had_error = true;
}


void error (int line, std::string_view message)
{
     report(line, "", message);
}


void error (Token token, std::string message)
{
     if (token.type == TokenType::END_OF_FILE)     report(token.line, " at end", message);
     else                                          report(token.line, " at '" + to_string(token.lexeme) + "'", message);
}


void run (std::string source)
{
     Scanner scanner {source};
     std::vector<Token> tokens = scanner.scanTokens();

     for (auto&& token : tokens)
          std::cout << to_string(token) << "\n";
}


void runPrompt ()
{
     for (std::string line;;)
     {
          std::cout << "> ";
          if (!std::getline(std::cin, line))     break;

          run(line);
          had_error = false;
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
     if (had_error)     std::exit(65);
}


int main (int argc, char* argv[])
{
     try
     {
          if (argc >  2)
          {
               std::cout << "Usage: lox [script]";
               std::exit(64);
          }
          else if (argc == 2)     run(readFile(argv[1]));
          else                    runPrompt();
     }
     catch (std::exception& e)
     {
          std::cerr << e.what();
     }
}
