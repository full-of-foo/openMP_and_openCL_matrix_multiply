UTILS_DIR=matrix_utils/
UTILS_LIB_DIR=$(UTILS_DIR)src
OPENMP_DIR=openMP_app/
OPENMP_LIB_DIR=$(OPENMP_DIR)src
OPENMP_PROFILING_DIR=$(OPENMP_LIB_DIR)/profiling/

CC=gcc-4.9
AR=ar
CFLAGS=-std=c99 -Wall -g -DNDEBUG -fopenmp $(OPTFLAGS)
UTILS_CLIBFLAGS=-I$(CURDIR)/$(UTILS_LIB_DIR)
OPENMP_CLIBFLAGS=-I$(CURDIR)/$(OPENMP_LIB_DIR)

UTILS_AR_TARGET=$(UTILS_LIB_DIR)/libmatrixutils.a
OPENMP_AR_TARGET=$(OPENMP_LIB_DIR)/mp_mm.a
OPENMP_TARGET=$(OPENMP_DIR)bin/mp_mm

UTILS_SOURCES=$(wildcard $(UTILS_DIR)src/**/*.c $(UTILS_DIR)src/*.c)
UTILS_OBJECTS=$(patsubst %.c,%.o,$(UTILS_SOURCES))
UTILS_TEST_SOURCES=$(wildcard $(UTILS_DIR)tests/*_test.c)
UTILS_TESTS=$(patsubst %.c,%,$(UTILS_TEST_SOURCES))

OPENMP_SOURCES=$(wildcard $(OPENMP_DIR)src/*.c)
OPENMP_OBJECTS=$(patsubst %.c,%.o,$(OPENMP_SOURCES))
OPENMP_TEST_SOURCES=$(wildcard $(OPENMP_DIR)tests/*_test.c)
OPENMP_TESTS=$(patsubst %.c,%,$(OPENMP_TEST_SOURCES))
OPENMP_PROFILING_SOURCES=$(wildcard $(OPENMP_PROFILING_DIR)*.c)
OPENMP_PROFILING_OBJECTS=$(patsubst %.c,%,$(OPENMP_PROFILING_SOURCES))

SOURCES=$(UTILS_SOURCES) $(UTILS_TEST_SOURCES) $(OPENMP_SOURCES) $(OPENMP_TEST_SOURCES) $(OPENMP_PROFILING_SOURCES)
OBJECTS=$(UTILS_AR_TARGET) $(OPENMP_AR_TARGET) $(OPENMP_TARGET) $(OPENMP_OBJECTS) $(OPENMP_TESTS) $(UTILS_OBJECTS)
OBJECTS+=$(UTILS_TESTS) $(OPENMP_PROFILING_OBJECTS)

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

build-optimised: CFLAGS += -O3
build-optimised: build

build-openMP-profiling: CFLAGS += -O3 -g3 $(OPENMP_CLIBFLAGS) $(UTILS_CLIBFLAGS) $(OPENMP_AR_TARGET) $(UTILS_AR_TARGET)
build-openMP-profiling: $(OPENMP_PROFILING_OBJECTS)

profile: build-optimised build-openMP-profiling
	$(OPENMP_PROFILING_OBJECTS) > openMP_results.csv

tests: build tests-utils tests-openMp clean-logs
	sh ./runtests.sh

tests-utils: CFLAGS += $(UTILS_CLIBFLAGS) $(UTILS_AR_TARGET)
tests-utils: $(UTILS_TESTS)

tests-openMp: CFLAGS += $(OPENMP_CLIBFLAGS) $(UTILS_CLIBFLAGS) $(OPENMP_AR_TARGET) $(UTILS_AR_TARGET)
tests-openMp: $(OPENMP_TESTS)

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
