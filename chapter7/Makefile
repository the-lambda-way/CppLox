CXX      := g++
CXXFLAGS := -ggdb -std=c++17
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := AstPrinterDriver.cpp GenerateAst.cpp Lox.cpp
DEPS     := $(SRCS:.cpp=.d)


jlox: Expr.h Lox.o
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


.PHONY: test-expressions
test-expressions:
	@make jlox >/dev/null
	@echo "testing jlox with test-expressions.lox ..."
	@./jlox tests/test-expressions.lox 2>&1 | diff -u --color tests/test-expressions.lox.expected -;


.PHONY: test-expressions2
test-expressions2:
	@make jlox >/dev/null
	@echo "testing jlox with test-expressions2.lox ..."
	@./jlox tests/test-expressions2.lox | diff -u --color tests/test-expressions2.lox.expected -;