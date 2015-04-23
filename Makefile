UTILS_DIR=matrix_utils/
UTILS_LIB_DIR=$(UTILS_DIR)src
OPENMP_DIR=openMP_app/
OPENMP_LIB_DIR=$(OPENMP_DIR)src

CC=gcc-4.9
AR=ar
CFLAGS=-std=c99 -Wall -g -DNDEBUG -fopenmp $(OPTFLAGS)
UTILS_CLIBFLAGS=-I$(CURDIR)/$(UTILS_LIB_DIR)
OPENMP_CLIBFLAGS=-I$(CURDIR)/$(OPENMP_LIB_DIR)

UTILS_AR_TARGET=$(UTILS_LIB_DIR)/libmatrixutils.a
OPENMP_AR_TARGET=$(OPENMP_LIB_DIR)/openmp_app.a
OPENMP_TARGET=$(OPENMP_DIR)bin/openmp_app

UTILS_SOURCES=$(wildcard $(UTILS_DIR)src/**/*.c $(UTILS_DIR)src/*.c)
UTILS_OBJECTS=$(patsubst %.c,%.o,$(UTILS_SOURCES))
UTILS_TEST_SOURCES=$(wildcard $(UTILS_DIR)tests/*_test.c)
UTILS_TESTS=$(patsubst %.c,%,$(UTILS_TEST_SOURCES))

OPENMP_SOURCES=$(wildcard $(OPENMP_DIR)src/**/*.c $(OPENMP_DIR)src/*.c)
OPENMP_OBJECTS=$(patsubst %.c,%.o,$(OPENMP_SOURCES))
OPENMP_TEST_SOURCES=$(wildcard $(OPENMP_DIR)tests/*_test.c)
OPENMP_TESTS=$(patsubst %.c,%,$(OPENMP_TEST_SOURCES))

SOURCES=$(UTILS_SOURCES) $(OPENMP_SOURCES)
OBJECTS=$(UTILS_AR_TARGET) $(OPENMP_AR_TARGET) $(OPENMP_TARGET) $(OPENMP_OBJECTS) $(UTILS_OBJECTS) $(UTILS_TESTS)

all: build tests

build: build-utils build-openMP

build-utils: CFLAGS += -fPIC $(UTILS_CLIBFLAGS)
build-utils: $(UTILS_OBJECTS)
	$(AR) rcs $(UTILS_AR_TARGET) $(UTILS_OBJECTS)
	ranlib $(UTILS_AR_TARGET)

build-openMP: build-utils
build-openMP: CFLAGS += -fPIC $(UTILS_CLIBFLAGS)
build-openMP: $(OPENMP_OBJECTS)
	$(AR) rcs $(OPENMP_AR_TARGET) $(OPENMP_OBJECTS)
	ranlib $(OPENMP_AR_TARGET)

tests: build tests-utils tests-openMp clean-logs
	sh ./runtests.sh

tests-utils: CFLAGS += $(UTILS_CLIBFLAGS) $(UTILS_AR_TARGET)
tests-utils: $(UTILS_TESTS)

tests-openMp: CFLAGS += $(OPENMP_CLIBFLAGS) $(UTILS_CLIBFLAGS) $(OPENMP_AR_TARGET) $(UTILS_AR_TARGET)
tests-openMp: $(OPENMP_TESTS)

profile: build-openMP
	instruments -t "Time Profiler" $(OPENMP_TARGET)

clean: clean-logs
	$(RM)r $(OBJECTS)
	find . -name "*.gc*" -exec rm {} \;
	$(RM)r `find . -name "*.dSYM" -print`

clean-logs:
	$(RM)r **/tests/tests.log

BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@egrep $(BADFUNCS) $(SOURCES) || true
