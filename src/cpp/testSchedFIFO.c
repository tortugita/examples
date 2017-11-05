#include <stdio.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>

#define NO_OF_THREADS 3
#define DURATION_OF_THREAD 10

/***********************************************
 * 
 * Compile and build:
 * gcc -o schedFifo testSchedFIFO.c -lpthread
 * 
 * *********************************************/

static struct sched_param sParam[NO_OF_THREADS];
static pthread_attr_t sAttr[NO_OF_THREADS];

void *process_thread(void *aNo_p)
{
  int lCount;
  int lNo = (int)aNo_p;
  
  for (lCount = 0; lCount < DURATION_OF_THREAD; lCount++)
  {
    printf("hello from thread %d\n", lNo);
    sleep(1);
  }
  
  pthread_exit(NULL);
}

main()
{
  int lPolicy = SCHED_FIFO;
  
  // FIFO scheduler priorities -> FIFO
  printf ("SCHED_FIFO min priority -> %d\n", sched_get_priority_min(lPolicy));
  printf ("SCHED_FIFO max priority -> %d\n", sched_get_priority_max(lPolicy));
  
  // Thread
  pthread_t lThread[NO_OF_THREADS];
  
  int lNo;
  for (lNo = 0; lNo < NO_OF_THREADS; lNo++)
  {
    sParam[lNo].sched_priority = sched_get_priority_max(lPolicy);
    
    int lRet = pthread_attr_init(&sAttr[lNo]);
    if (lRet != 0) 
    {
       // Error
       printf("error in initializing thread attribute -> %d\n", lRet);
       return -1;     
    }
    
    lRet = pthread_attr_setschedpolicy(&sAttr[lNo], lPolicy);
    if (lRet != 0) 
    {
       // Error
       printf("error in setting thread realtime policy -> %d\n", lRet);
       return -1;     
    }
    
    lRet = pthread_attr_setinheritsched(&sAttr[lNo], PTHREAD_EXPLICIT_SCHED);
    if (lRet != 0) 
    {
       // Error
       printf("error in setting thread realtime policy -> %d\n", lRet);
       return -1;     
    }
    
    lRet = pthread_attr_setschedparam(&sAttr[lNo], &sParam[lNo]);
    if (lRet != 0) 
    {
       // Error
       printf("error in setting thread realtime prio -> %d\n", lRet);
       return -1;     
    }
    
    lRet = pthread_attr_getschedparam(&sAttr[lNo], &sParam[lNo]);
    if (lRet != 0)
    {
      // Error
      printf("error in getting thread realtime prio -> %d\n", lRet);
      return -1;
    }
    
    if (pthread_create(&lThread[lNo], &sAttr[lNo], process_thread, (void *)lNo))
    {
      printf("error creating thread %d\n", lNo);
      return -1;
    }
    printf("start thread %d\n", lNo);
  }
  
  pthread_exit(NULL);
  
}