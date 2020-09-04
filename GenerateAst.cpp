#include <cstring>         // std::strtok
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>         // std::move
#include <vector>



void defineType (std::ofstream& writer, std::string_view baseName,
                 std::string_view className, const std::string& fieldList)
{
     writer << "struct " << className << " : " << baseName << "\n"
            << "{\n";

     // Fields.
     std::istringstream is {fieldList};
     std::string field;

     while (!is.eof())
     {
          is >> std::ws;
          std::getline(is, field, ',');
          writer << "     " << field << ";\n";
     }

     writer << "};\n\n";
}


void defineAst (std::string_view outputDir, std::string_view baseName, const std::vector<std::string>& types)
{
     std::string path;
     path.append(outputDir).append("/").append(baseName).append(".h");
     std::ofstream writer {path};

     writer << "#include <vector>\n"
            << "#include \"Token.h\"\n"
            << "\n"
            << "class " << baseName << "\n"
            << "{\n"
            << "};\n\n";

     for (std::string type : types)
     {
          // C++ has no built-in string split or trim functions. Input streams skip whitespace, and provide a fairly
          // terse alternative.

          std::istringstream is {type};
          std::string className, fields;

          is >> className;
          is.ignore(type.length(), ':');
          std::getline(is, fields, '\0');

          defineType(writer, baseName, className, fields);
     }
}


int main (int argc, char* argv[])
{
     if (argc != 2)
     {
          std::cout << "Usage: generate_ast <output directory>";
          std::exit(64);
     }

     std::string_view outputDir = argv[1];

     defineAst(outputDir, "Expr", {
          "Binary   : Expr left, Token operator, Expr right",
          "Grouping : Expr expression",
          "Literal  : Object value",
          "Unary    : Token operator, Expr right"
     });
}

