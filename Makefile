CC=gcc-4.9
CFLAGS=-Wall -g -fopenmp

TEST_FILES=matrix_utils/test_dbg.c
TEST_EXC_FILES=$(patsubst %.c, %, $(TEST_FILES))

SRC_FILES=openMp_app/app.c
EXC_FILES=$(patsubst %.c, %, $(SRC_FILES))
DEBUG_FILES=$(patsubst %.c, %.dSYM, $(SRC_FILES) $(TEST_FILES))


all: clean $(EXC_FILES)

clean:
	$(RM)r $(EXC_FILES) $(TEST_EXC_FILES) $(DEBUG_FILES)

test: test-init test-dbg

test-init:
	cc -Wall -g -DNDEBUG $(TEST_FILES) -o $(TEST_EXC_FILES)

test-dbg:
	matrix_utils/test_dbg "test"


