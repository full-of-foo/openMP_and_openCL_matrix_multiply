UTILS_DIR=matrix_utils/
UTILS_LIB_DIR=$(UTILS_DIR)src
OPENMP_DIR=openMP_app/

CC=gcc-4.9
AR=ar
CFLAGS=-std=c99 -Wall -g -DNDEBUG -fopenmp $(OPTFLAGS)
CLIBFLAGS=-I$(CURDIR)/$(UTILS_LIB_DIR)

AR_TARGET=$(UTILS_LIB_DIR)/libmatrixutils.a
OPENMP_TARGET=$(OPENMP_DIR)bin/app

DEP_SOURCES=$(wildcard $(UTILS_DIR)src/**/*.c $(UTILS_DIR)src/*.c)
DEP_OBJECTS=$(patsubst %.c,%.o,$(DEP_SOURCES))
TEST_SRC=$(wildcard */tests/*_test.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))
SOURCES=$(wildcard $(OPENMP_DIR)src/**/*.c $(OPENMP_DIR)src/*.c)
OBJECTS=$(AR_TARGET) $(OPENMP_TARGET) $(DEP_OBJECTS) $(TESTS)

all: build openMP tests

build-deps: CFLAGS += -fPIC $(CLIBFLAGS)
build-deps: $(DEP_OBJECTS)
	$(AR) rcs $(AR_TARGET) $(DEP_OBJECTS)
	ranlib $(AR_TARGET)

build: build-deps
	@mkdir -p $(OPENMP_DIR)bin

openMP: build
	$(CC) $(CFLAGS) $(CLIBFLAGS) $(SOURCES) $(AR_TARGET) -o $(OPENMP_TARGET)

.PHONY: tests
tests: CFLAGS += $(AR_TARGET)
tests: CFLAGS += $(CLIBFLAGS)
tests: $(TESTS) clean-logs
	sh ./runtests.sh

profile: openMP
	instruments -t "Time Profiler" $(OPENMP_TARGET)

clean-logs:
	$(RM)r **/tests/tests.log

clean: clean-logs
	$(RM)r $(OBJECTS)
	find . -name "*.gc*" -exec rm {} \;
	$(RM)r `find . -name "*.dSYM" -print`

BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@egrep $(BADFUNCS) $(SOURCES) $(DEP_SOURCES) $(TEST_SRC) || true
