#include <dbg.h>
#include <sys/time.h>
#include <time.h>
#include "matrixutils.h"
#include "mp_mm.h"

typedef unsigned long long timestamp_t;

static timestamp_t get_timestamp()
{
  struct timeval now;
  gettimeofday(&now, NULL);

  return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

int main(int argc, char *argv[])
{
    struct MATRIX *mA = matrix_create(1000, 1000, 2);
    struct MATRIX *mB = matrix_create(1000, 1000, 2);

    log_info("Beginning benchmarks...");
    timestamp_t t0 = get_timestamp();
    struct MATRIX *mC = matrix_multiply(mA, mB);
    timestamp_t t1 = get_timestamp();

    float secs = (t1 - t0) / 1000000.0L;
    log_info("Exection time: %g us (%g secs)", secs,  (secs/1000000));
}
