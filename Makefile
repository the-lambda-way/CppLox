CXX      := g++
INCLUDES :=
CXXFLAGS := -ggdb -std=c++17 $(INCLUDES)
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := AstPrinterDriver.cpp GenerateAst.cpp Lox.cpp
DEPS     := $(SRCS:.cpp=.d)


jlox: Lox.o
	@$(COMPILE) $< -o $@


.PHONY: test-lex
test-lex: jlox test-lex.lox test-lex.lox.expected
	@echo "testing jlox with test-lex.lox ..."
	@./jlox test-lex.lox | diff -u --color test-lex.lox.expected -;

test-lex.lox:
test-lex.lox.expected:


.PHONY: test-parse
test-parse: jlox test-parse.lox test-parse.lox.expected
	@echo "testing jlox with test-parse.lox ..."
	@./jlox test-parse.lox | diff -u --color test-parse.lox.expected -;

test-parse.lox:
test-parse.lox.expected:


generate_ast: GenerateAst.o
	@$(COMPILE) $< -o $@


Expr.h: generate_ast
	@./generate_ast .


ast_printer: AstPrinterDriver.o
	@$(COMPILE) $< -o $@


.PHONY: clean
clean:
	rm -f *.d *.o ast_printer generate_ast jlox


.SILENT : $(DEPS)
-include $(DEPS)
