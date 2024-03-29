CXX      := g++
CXXFLAGS := -ggdb -std=c++17
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := AstPrinterDriver.cpp GenerateAst.cpp Lox.cpp
DEPS     := $(SRCS:.cpp=.d)


jlox: Expr.h Stmt.h Lox.o
	@$(COMPILE) Lox.o -o $@


ast_printer: Expr.h Stmt.h AstPrinterDriver.o
	@$(COMPILE) AstPrinterDriver.o -o $@


generate_ast: GenerateAst.o
	@$(COMPILE) $< -o $@


Expr.h Stmt.h: generate_ast
	@./generate_ast .


.PHONY: clean
clean:
	rm -f *.d *.o ast_printer generate_ast jlox


-include $(DEPS)


define make_test
.PHONY: $(1)
$(1):
	@make jlox >/dev/null
	@echo "testing jlox with $(1).lox ..."
	@./jlox tests/$(1).lox | diff -u --color tests/$(1).lox.expected -;
endef


define make_test_error
.PHONY: $(1)
$(1):
	@make jlox >/dev/null
	@echo "testing jlox with $(1).lox ..."
	@./jlox tests/$(1).lox 2>&1 | diff -u --color tests/$(1).lox.expected -;
endef


TESTS = \
test-statements \
test-statements2 \
test-statements4 \
test-statements5 \
test-statements6 \


TEST_ERRORS = \
test-challenge2 \
test-statements3 \


$(foreach test, $(TESTS), $(eval $(call make_test,$(test))))
$(foreach test, $(TEST_ERRORS), $(eval $(call make_test_error,$(test))))


.PHONY: test-all
test-all:
	@for test in $(TESTS) $(TEST_ERRORS); do \
		make -s $$test; \
	done
