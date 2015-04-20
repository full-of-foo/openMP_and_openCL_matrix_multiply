CC=gcc-4.9
CFLAGS=-g -O2 -Wall -Wextra -Isrc -DNDEBUG -fopenmp $(OPTFLAGS)

UTILS_DIR=matrix_utils/
OPENMP_DIR=openMP_app/

DEP_SOURCES=$(wildcard $(UTILS_DIR)src/**/*.c $(UTILS_DIR)src/*.c)
DEP_OBJECTS=$(patsubst %.c,%.o,$(DEP_SOURCES))
DEP_TEST_SRC=$(wildcard $(UTILS_DIR)tests/*_test.c)
DEP_TESTS=$(patsubst %.c,%,$(DEP_TEST_SRC))
SOURCES=$(wildcard $(OPENMP_DIR)src/**/*.c $(OPENMP_DIR)src/*.c)
EXES=$(patsubst %.c,%,$(SOURCES))

TARGET=$(UTILS_DIR)build/libmatrixutils.a


all: $(TARGET) tests

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra -fopenmp $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(DEP_OBJECTS)
	ar rcsv $@ $(DEP_OBJECTS)
	ranlib $@
	$(CC) $(CFLAGS) openMP_app/src/app.c $@ -o openMP_app/bin/app

build:
	@mkdir -p $(UTILS_DIR)build
	@mkdir -p $(OPENMP_DIR)bin

.PHONY: tests
tests: CFLAGS += $(TARGET)
tests: $(DEP_TESTS) clean-logs
	sh ./runtests.sh

clean-logs:
	$(RM)r **/tests/tests.log

clean: clean-logs
	$(RM)r $(UTILS_DIR)build $(DEP_OBJECTS) $(EXES) $(DEP_TESTS)
	find . -name "*.gc*" -exec rm {} \;
	$(RM)r `find . -name "*.dSYM" -print`

BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@egrep $(BADFUNCS) $(SOURCES) $(DEP_SOURCES) $(DEP_TEST_SRC) || true
