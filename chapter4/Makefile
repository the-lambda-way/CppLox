CXX      := g++
CXXFLAGS := -ggdb -std=c++17
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := Lox.cpp
DEPS     := $(SRCS:.cpp=.d)


jlox: Lox.o
	@$(COMPILE) $< -o $@


.PHONY: clean
clean:
	rm -f *.d *.o jlox


-include $(DEPS)


.PHONY: test-lexing
test-lexing:
	@make jlox >/dev/null
	@echo "testing jlox with test-lexing.lox ..."
	@./jlox tests/test-lexing.lox | diff -u --color tests/test-lexing.lox.expected -;


.PHONY: test-lexing2
test-lexing2:
	@make jlox >/dev/null
	@echo "testing jlox with test-lexing2.lox ..."
	@./jlox tests/test-lexing2.lox | diff -u --color tests/test-lexing2.lox.expected -;
