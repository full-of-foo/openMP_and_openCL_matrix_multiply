UTILS_DIR=matrix_utils/
UTILS_LIB_DIR=$(UTILS_DIR)src
OPENMP_DIR=openMP_app/

CC=gcc-4.9
AR=ar
CFLAGS=-g -Wall -DNDEBUG -fopenmp $(OPTFLAGS)
CLIBFLAGS=-I$(CURDIR)/$(UTILS_LIB_DIR)

DEP_SOURCES=$(wildcard $(UTILS_DIR)src/**/*.c $(UTILS_DIR)src/*.c)
DEP_OBJECTS=$(patsubst %.c,%.o,$(DEP_SOURCES))
TEST_SRC=$(wildcard */tests/*_test.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))
SOURCES=$(wildcard $(OPENMP_DIR)src/**/*.c $(OPENMP_DIR)src/*.c)

AR_TARGET=$(UTILS_LIB_DIR)/libmatrixutils.a


all: $(AR_TARGET) tests

$(AR_TARGET): CFLAGS += -fPIC $(CLIBFLAGS)
$(AR_TARGET): build $(DEP_OBJECTS)
	$(AR) rcs $@ $(DEP_OBJECTS)
	ranlib $@
	$(CC) $(CFLAGS) openMP_app/src/app.c $(AR_TARGET) -o openMP_app/bin/app

build:
	@mkdir -p $(OPENMP_DIR)bin

.PHONY: tests
tests: CFLAGS += $(AR_TARGET)
tests: CFLAGS += $(CLIBFLAGS)
tests: $(TESTS) clean-logs
	sh ./runtests.sh

clean-logs:
	$(RM)r **/tests/tests.log

clean: clean-logs
	$(RM)r $(AR_TARGET) $(DEP_OBJECTS) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	$(RM)r `find . -name "*.dSYM" -print`

BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@egrep $(BADFUNCS) $(SOURCES) $(DEP_SOURCES) $(TEST_SRC) || true
