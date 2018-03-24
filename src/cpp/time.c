#include<stdio.h>
#include<time.h>
#include <unistd.h>

int main()
{
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  sleep(1);
  clock_gettime(CLOCK_MONOTONIC, &end);

  long delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

  printf("CLOCK_MONOTONIC: start time: %ldns\n", start.tv_nsec);
  printf("CLOCK_MONOTONIC: end time: %ldns\n", end.tv_nsec);
  printf("CLOCK_MONOTONIC: delta us: %ldus\n", delta_us);

  clock_gettime(CLOCK_REALTIME, &start);
  sleep(1);
  clock_gettime(CLOCK_REALTIME, &end);

  delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

  printf("CLOCK_REALTIME: start time: %ldns\n", start.tv_nsec);
  printf("CLOCK_REALTIME: end time: %ldns\n", end.tv_nsec);
  printf("CLOCK_REALTIME: delta us: %ldus\n", delta_us);
  return 0;
}
