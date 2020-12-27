CXX      := g++
CXXFLAGS := -ggdb -std=c++17
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := AstPrinterDriver.cpp GenerateAst.cpp Lox.cpp
DEPS     := $(SRCS:.cpp=.d)


.PHONY: jlox
jlox: Lox.cpp
	@$(COMPILE) $^ -o $@


.PHONY: test-lex
test-lex:
	@make jlox >/dev/null
	@echo "testing jlox with test-lex.lox ..."
	@./jlox test-lex.lox | diff -u --color test-lex.lox.expected -;


.PHONY: test-parse
test-parse:
	@make jlox >/dev/null
	@echo "testing jlox with test-parse.lox ..."
	@./jlox test-parse.lox | diff -u --color test-parse.lox.expected -;


.PHONY: test-expressions
test-expressions:
	@make jlox >/dev/null
	@echo "testing jlox with test-expressions.lox ..."
	@./jlox test-expressions.lox | diff -u --color test-expressions.lox.expected -;


.PHONY: test-statements
test-statements:
	@make jlox >/dev/null
	@echo "testing jlox with test-statements.lox ..."
	@./jlox test-statements.lox | diff -u --color test-statements.lox.expected -;


.PHONY: test-statements2
test-statements2:
	@make jlox >/dev/null
	@echo "testing jlox with test-statements2.lox ..."
	@./jlox test-statements2.lox | diff -u --color test-statements2.lox.expected -;


.PHONY: test-control-flow
test-control-flow:
	@make jlox >/dev/null
	@echo "testing jlox with test-control-flow.lox ..."
	@./jlox test-control-flow.lox | diff -u --color test-control-flow.lox.expected -;


.PHONY: test-functions
test-functions:
	@make jlox >/dev/null
	@echo "testing jlox with test-functions.lox ..."
	@./jlox test-functions.lox | diff -u --color test-functions.lox.expected -;


.PHONY: test-functions2
test-functions2:
	@make jlox >/dev/null
	@echo "testing jlox with test-functions2.lox ..."
	@./jlox test-functions2.lox | diff -u --color test-functions2.lox.expected -;


.PHONY: test-functions3
test-functions3:
	@make jlox >/dev/null
	@echo "testing jlox with test-functions3.lox ..."
	@./jlox test-functions3.lox | diff -u --color test-functions3.lox.expected -;


.PHONY: test-resolving
test-resolving:
	@make jlox >/dev/null
	@echo "testing jlox with test-resolving.lox ..."
	@./jlox test-resolving.lox | diff -u --color test-resolving.lox.expected -;


.PHONY: test-resolving2
test-resolving2:
	@make jlox >/dev/null
	@echo "testing jlox with test-resolving2.lox ..."
	@-./jlox test-resolving2.lox 2>&1 | diff -u --color test-resolving2.lox.expected -;


.PHONY: test-resolving3
test-resolving3:
	@make jlox >/dev/null
	@echo "testing jlox with test-resolving3.lox ..."
	@./jlox test-resolving3.lox 2>&1 | diff -u --color test-resolving3.lox.expected -;


.PHONY: test-resolving4
test-resolving4:
	@make jlox >/dev/null
	@echo "testing jlox with test-resolving4.lox ..."
	@./jlox test-resolving4.lox 2>&1 | diff -u --color test-resolving4.lox.expected -;


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
