CXX      := /usr/local/gcc-10.2.0/bin/g++-10.2
CXXFLAGS := -O3 -ddbg
CPPFLAGS := -std=c++20

INCLUDES := -I/usr/local/gcc-10.2.0/include/c++/10.2.0/

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES)

# Set the display of the time command. See https://man7.org/linux/man-pages/man1/time.1.html
TIME_FORMAT="%E elapsed  (%U user  %S system)  |  %P CPU  |  %Xk text  %Dk data  %Mk max  |  %I inputs  %O outputs  |  %F major + %R minor pagefaults  |  %W swaps\n"


lox: Lox.cpp
	@$(COMPILE) -O3 $< -o $@


.PHONY: test
test: lox
	@echo "testing test.lox ..."
	@./lox test.lox | diff -u --color test.lox.expected -;


test.lox:
test.lox.expected:


.PHONY: clean
clean:
	rm -f lox

