#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sched.h>

main()
{
  // Normal scheduler priorities -> CVS
  printf ("SCHED_OTHER min priority -> %d\n", sched_get_priority_min(SCHED_OTHER));
  printf ("SCHED_OTHER max priority -> %d\n", sched_get_priority_max(SCHED_OTHER));
  // FIFO scheduler priorities -> FIFO
  printf ("SCHED_FIFO min priority -> %d\n", sched_get_priority_min(SCHED_FIFO));
  printf ("SCHED_FIFO max priority -> %d\n", sched_get_priority_max(SCHED_FIFO));
  // Round Robin scheduler priorities -> RR
  printf ("SCHED_RR min priority -> %d\n", sched_get_priority_min(SCHED_RR));
  printf ("SCHED_RR max priority -> %d\n", sched_get_priority_max(SCHED_RR));
  
  int lFd = -1;
  char lBuf[20];
  // Show runtime execution time -> sched_rt_runtime_us
  lFd = open("/proc/sys/kernel/sched_rt_runtime_us", O_RDONLY);
  if (lFd < 0)
  {
    printf("failed to open file -> %d", errno); 
  }
  else
  {
    int lLen = read(lFd, lBuf, sizeof(lBuf));
    if ( lLen < 0)
    {
      printf("error reading file -> %d", errno);
    }
    else
    {
      char lStr[lLen];
      strncpy(lStr, lBuf, lLen);
      printf("/proc/sys/kernel/sched_rt_runtime_us -> %s", lStr);
    }
  }
  close(lFd);
  lFd = -1;
  
  // Show runtime periode time -> sched_rt_period_us
  lFd = open("/proc/sys/kernel/sched_rt_period_us", O_RDONLY);
  if (lFd < 0)
  {
    printf("failed to open file -> %d", errno); 
  }
  else
  {
    int lLen = read(lFd, lBuf, sizeof(lBuf));
    if (lLen < 0)
    {
      printf("error reading file -> %d\n", errno);
    }
    else
    {
      char lStr[lLen];
      strncpy(lStr, lBuf, lLen);
      printf("/proc/sys/kernel/sched_rt_period_us -> %s\n", lStr);
    } 
  }
  close(lFd);
}