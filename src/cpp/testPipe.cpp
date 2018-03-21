#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

int sFdPipe[2];

/* 
 * This function continously reads fd[0] for any input data byte
 * If available, prints
 */

void *reader(void* aArg)
{
  while(1)
  {
    char lCh;
    int lResult;

    lResult = read (sFdPipe[0],&lCh,1);
    if (lResult < 0)
    {
      // for non-blocking read
      if (errno == EAGAIN)
      {
        printf("reader: no data available\n");
      }
    }
    else if (lResult != 1) 
    {
      perror("read");
      exit(3);
    }

    printf ("Reader: %c\n", lCh);   
  }
}

/* 
 * This function continously writes Alphabet into fd[1]
 * Waits if no more space is available
 */
void *writer(void* aArg)
{
  int lResult;
  char lCh='A';

  while(1)
  {
    lResult = write(sFdPipe[1], &lCh, 1);
    if (lResult != 1)
    {
      perror ("write");
      exit (2);
    }

    printf ("Writer: %c\n", lCh);
    if(lCh == 'Z')
    {
      lCh = 'A'-1;
    }

     lCh++;
     sleep(5);
   }
}

/* 
 * main
 * usage: g++ testPipe.cpp -lpthread
 */
int main()
{
  pthread_t lTid1, lTid2;
  int lResult;

  lResult = pipe(sFdPipe);

  if (lResult < 0)
  {
    perror("pipe ");
    exit(1);
  }

  // For non-blocking read
  if (fcntl(sFdPipe[0], F_SETFL, O_NONBLOCK) < 0)
  {
    exit(1);
  }

  pthread_create(&lTid1, NULL, reader, NULL);
  pthread_create(&lTid2, NULL, writer, NULL);

  pthread_join(lTid1, NULL);
  pthread_join(lTid2, NULL);
}
