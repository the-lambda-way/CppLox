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


.PHONY: test-expressions
test-expressions: jlox test-expressions.lox test-expressions.lox.expected
	@echo "testing jlox with test-expressions.lox ..."
	@./jlox test-expressions.lox | diff -u --color test-expressions.lox.expected -;

test-expressions.lox:
test-expressions.lox.expected:


.PHONY: test-statements
test-statements: jlox test-statements.lox test-statements.lox.expected
	@echo "testing jlox with test-statements.lox ..."
	@./jlox test-statements.lox | diff -u --color test-statements.lox.expected -;

test-statements.lox:
test-statements.lox.expected:


.PHONY: test-statements2
test-statements2: jlox test-statements2.lox test-statements2.lox.expected
	@echo "testing jlox with test-statements2.lox ..."
	@./jlox test-statements2.lox | diff -u --color test-statements2.lox.expected -;

test-statements2.lox:
test-statements2.lox.expected:


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
