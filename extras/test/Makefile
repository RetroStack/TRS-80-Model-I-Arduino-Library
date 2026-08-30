# Native (host) tests. Run with: make -C extras/test
#
# These compile the hardware-independent parts of the library with a normal
# host compiler and actually execute them, which the AVR build cannot do.

CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -I ../../src -I stub

TESTS := test_RenderManager

.PHONY: all clean
all: $(TESTS)
	@for t in $(TESTS); do echo "--- $$t"; ./$$t || exit 1; done

test_RenderManager: test_RenderManager.cpp ../../src/RenderManager.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TESTS)
