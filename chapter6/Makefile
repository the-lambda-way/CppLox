CXX      := g++
CXXFLAGS := -ggdb -std=c++17
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := AstPrinterDriver.cpp GenerateAst.cpp Lox.cpp
DEPS     := $(SRCS:.cpp=.d)


jlox: ast_printer Lox.o
	@$(COMPILE) Lox.o -o $@


ast_printer: Expr.h AstPrinterDriver.o
	@$(COMPILE) AstPrinterDriver.o -o $@


generate_ast: GenerateAst.o
	@$(COMPILE) $< -o $@


Expr.h: generate_ast
	@./generate_ast .


.PHONY: clean
clean:
	rm -f *.d *.o ast_printer generate_ast jlox


-include $(DEPS)


.PHONY: test-parsing
test-parsing:
	@make jlox >/dev/null
	@echo "testing jlox with test-parsing.lox ..."
	@./jlox tests/test-parsing.lox | diff -u --color tests/test-parsing.lox.expected -;
