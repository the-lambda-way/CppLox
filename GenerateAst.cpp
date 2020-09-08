#include <algorithm>       // std::find_if_not
#include <cctype>          // std::tolower, std::isspace
#include <fstream>
#include <iostream>
#include <ranges>          // defineType
#include <sstream>
#include <string>
#include <string_view>
#include <vector>


std::vector<std::string_view> split (std::string_view str, std::string_view delim)
{
     std::vector<std::string_view> out;

     std::string_view::size_type start;
     std::string_view::size_type end = 0;

     while (true)
     {
          start = end;
          end = str.find(delim, start);

          out.push_back(str.substr(start, end - start));

          if (end >= str.length())     break;

          end += delim.length();
     }

     return out;
}


std::string_view trim (std::string_view str)
{
     auto f = [] (auto c) { return std::isspace(c); };

     auto start = std::find_if_not(str.begin(), str.end(), f);
     auto end   = std::find_if_not(str.rbegin(), str.rend(), f).base();

     return {start, std::string_view::size_type(end - start)};
}


std::string toLowerCase (std::string_view str)
{
     std::string copy;

     for (char c : str)
          copy.push_back(std::tolower(c));

     return copy;
}


void defineVisitor (std::ofstream& writer, std::string_view baseName, const std::vector<std::string_view>& types)
{
     writer << "struct Visitor\n"
               "{\n";

     for (std::string_view type : types)
     {
          std::string_view typeName = trim(split(type, ":")[0]);

          writer << "     virtual std::any visit" << typeName << baseName << "(const "
                 << typeName << "* " << toLowerCase(baseName) << ") const = 0;\n";
     }

     writer << "};\n";
}


void defineType (std::ofstream& writer, std::string_view baseName,
                 std::string_view className, std::string_view fieldList)
{
     writer << "struct " << className << " : " << baseName << "\n"
            << "{\n";

     // Constructor.
     writer << "     " << className << " (" << fieldList << ")\n"
            << "          : ";

     // Store parameters in fields.
     std::vector<std::string_view> fields = split(fieldList, ", ");

     std::string_view name = split(fields[0], " ")[1];
     writer << name << " {" << name << "}";

     for (std::string_view field : fields | std::views::drop(1))
     {
          std::string_view name = split(field, " ")[1];
          writer << ", " << name << " {" << name << "}";
     }

     writer << "\n"
            << "     {}\n";

     // Visitor pattern.
     writer << "\n"
               "     std::any accept (const Visitor* visitor) const override\n"
               "     {\n"
               "          return visitor->visit" << className << baseName << "(this);\n"
               "     }\n";

     // Fields.
     writer << "\n";
     for (std::string_view field : fields)
          writer << "     const " << field << ";\n";

     writer << "};\n\n";
}


void defineAst (std::string_view outputDir, std::string_view baseName, const std::vector<std::string_view>& types)
{
     std::string path;
     path.append(outputDir).append("/").append(baseName).append(".h");
     std::ofstream writer {path};

     writer << "#include <any>\n"
               "#include \"Token.h\"\n"
               "\n";

     // Forward declare the AST classes.
     for (std::string_view type : types)
     {
          std::string_view className = trim(split(type, ":")[0]);
          writer << "struct " << className << ";\n";
     }

     writer << "\n";

     defineVisitor(writer, baseName, types);

     writer << "\n"
               "struct " << baseName << "\n"
               "{\n"

               // The base accept() method.
               // C++ does not allow virtual methods to be templated. That means multiple accept signatures are out --
               // at least if we don't want to over complicate things. An alternative is to use std::any, which holds
               // values of any type in a type-safe way. Member functions of the base class and the Visitor class will
               // return std::any, and the class implementing Visitor is required to cast the return value to the
               // expected type inside its member functions.
               "     virtual std::any accept (const Visitor* visitor) const = 0;\n"
               "};\n\n";

     // The AST classes.
     for (std::string_view type : types)
     {
          std::string_view className = trim(split(type, ":")[0]);
          std::string_view fields    = trim(split(type, ":")[1]);

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
          "Binary   : Expr* left, Token op, Expr* right",
          "Grouping : Expr* expression",
          "Literal  : LiteralVal value",
          "Unary    : Token op, Expr* right"
     });
}

