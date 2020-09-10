CXX      := /usr/local/gcc-10.2.0/bin/g++-10.2
INCLUDES := -I/usr/local/gcc-10.2.0/include/c++/10.2.0/
CXXFLAGS := -ggdb -std=c++20 $(INCLUDES)
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := AstPrinterDriver.cpp GenerateAst.cpp Lox.cpp
DEPS     := $(SRCS:.cpp=.d)


.SILENT  : $(SRCS:.cpp=.o)


lox: Lox.o
	@$(COMPILE) $< -o $@


.PHONY: test-lex
test-lex: lox test-lex.lox test-lex.lox.expected
	@echo "testing lox with test-lex.lox ..."
	@./lox test-lex.lox | diff -u --color test-lex.lox.expected -;

test-lex.lox:
test-lex.lox.expected:


.PHONY: test-parse
test-parse: lox test-parse.lox test-parse.lox.expected
	@echo "testing lox with test-parse.lox ..."
	@./lox test-parse.lox | diff -u --color test-parse.lox.expected -;

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
	rm -f *.d *.o ast_printer generate_ast lox


-include $(DEPS)