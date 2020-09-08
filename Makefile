CXX      := /usr/local/gcc-10.2.0/bin/g++-10.2
CXXFLAGS := -O3 -ggdb
CPPFLAGS := -std=c++20

INCLUDES := -I/usr/local/gcc-10.2.0/include/c++/10.2.0/

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES)

DEPS     := Scanner.h Token.h TokenType.h


lox: Lox.cpp $(DEPS)
	@$(COMPILE) $< -o $@


.PHONY: test
test: lox test.lox test.lox.expected
	@echo "testing test.lox ..."
	@./lox test.lox | diff -u --color test.lox.expected -;


test.lox:
test.lox.expected:


generate_ast: GenerateAst.cpp
	@$(COMPILE) $< -o $@


ast_printer: AstPrinter.cpp
	@$(COMPILE) $< -o $@


.PHONY: clean
clean:
	rm -f lox generate_ast ast_printer
