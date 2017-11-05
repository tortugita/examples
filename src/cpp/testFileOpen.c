#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int sFd = -1;

main()
{
  // Opens a file
  sFd = open("/opt/mytest", O_CREAT | O_EXCL);
 
  // Test file descriptor
  if ( sFd < 0 ) 
  {
    printf("open failed\n");
    printf ("errno is -> %d\n", errno);
    printf("error description is -> %s\n", strerror(errno));
  }
  else
  {
    printf("open successed\n");
    printf ("there is no errno -> %d\n", errno);
    printf("there is no error description -> %s\n", strerror(errno));
  }
}