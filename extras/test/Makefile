# Native (host) tests. Run with: make -C extras/test
#
# These compile the hardware-independent parts of the library with a normal
# host compiler and actually execute them, which the AVR build cannot do.
#
# -MMD -MP emits a .d file per binary listing every header it pulled in, and the
# -include below feeds those back as prerequisites, so a change to a header in
# src/ forces a rebuild. Without it make only watched the .cpp files, and a
# header change left a stale binary that still ran and still reported a pass.
#
# The recipes filter $^ down to %.cpp because those .d prerequisites otherwise
# land on the command line as extra inputs, which the compiler rejects with
# "cannot specify -o when generating multiple output files".

CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -I ../../src -I stub
DEPFLAGS := -MMD -MP

TESTS := test_RenderManager test_utils

.PHONY: all clean
all: $(TESTS)
	@for t in $(TESTS); do echo "--- $$t"; ./$$t || exit 1; done

test_RenderManager: test_RenderManager.cpp ../../src/RenderManager.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -o $@ $(filter %.cpp,$^)

test_utils: test_utils.cpp ../../src/utils.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -o $@ $(filter %.cpp,$^)

-include $(TESTS:=.d)

clean:
	rm -f $(TESTS) $(TESTS:=.d)
