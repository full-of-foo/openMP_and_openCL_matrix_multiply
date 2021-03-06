#include <dbg.h>
#include <stdlib.h>
#include <stdio.h>


void test_debug()
{
    debug("test_debug for string working");
    debug("test_debug for formatting (%d) working", 100);
}

void test_log_err()
{
    log_err("log_err for string working");
    log_err("log_err for formatting (%d) working", 100);
}

void test_log_warn()
{
    log_warn("log_warn for string working");
    log_warn("log_warn for formatting (%d) working", 100);
}

void test_log_info()
{
    log_info("log_info for string working");
    log_info("log_info for formatting (%d) working", 100);
}

int test_sentinel(int code)
{
    char *temp = malloc(100);
    check_mem(temp);

    switch(code) {
        case 1:
            log_info("It worked.");
            break;
        default:
            sentinel("I shouldn't run.");
    }

    free(temp);
    return 0;

error:
    if(temp) free(temp);
    return -1;
}

int test_check_mem()
{
    char *test = NULL;
    check_mem(test);

    free(test);
    return 1;

error:
    return -1;
}

int test_check_debug()
{
    int i = 0;
    check_debug(i != 0, "Oops, I was 0.");

    return 0;
error:
    return -1;
}

int main(int argc, char *argv[])
{
    // TODO - pass this in test runner
    argc = 2;
    argv[1] = 'test';

    check(argc == 2, "Need an argument.");

    log_info(" Error outputs are expected");
    test_debug();
    test_log_err();
    test_log_warn();
    test_log_info();

    check(test_sentinel(1) == 0, "test_sentinel failed.");
    check(test_sentinel(100) == -1, "test_sentinel failed.");
    check(test_check_mem() == -1, "test_check_mem failed.");
    check(test_check_debug() == -1, "test_check_debug failed.");

    return 0;

error:
    return 1;
}
