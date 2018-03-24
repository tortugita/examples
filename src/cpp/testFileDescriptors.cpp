#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

static void blockingWrite(const char* aFilename_p, const char* aFilecontent_p, const char* aFork_p)
{
  printf("example for blocking write\n");
  int lFd = open(aFilename_p, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (strcmp(aFork_p, "y") == 0)
  {
    printf("fork\n");
    fork();
  }
  write(lFd, aFilecontent_p, sizeof(aFilecontent_p));
  printf("descriptor %d, file name %s, content %s, file length %ld\n", lFd, aFilename_p, aFilecontent_p, lseek(lFd, 0, SEEK_CUR));
  close(lFd);
}

static int readWithPoll(const char* aFilename_p)
{
  printf("example for read with poll and a pipe\n");
  /*
   * requires a pipe, therefore make a pipe with mknod mypipe p
   */

  int lRet;
  char lBuf[1024];
  struct pollfd pfds[2];
  int lFd = open(aFilename_p, O_RDONLY);

  if (lFd < 0)
  {
    printf("an error occured, errno %d\n", errno);
    return -1;
  }
 
  printf("file open with file descriptor %d\n", lFd);
  
  while (1) 
  {
    /*
     * file descriptors:
     * 0 = stdin
     * 1 = stdout
     * 2 = stderr
     *
     * events (defined in poll.h):
     * POLLIN   = data to read
     * POLLOUT  = writing possible
     * POLLPRI  = some exceptional condition (see man poll)
     * POLLERR  = error condition
     * POLLHUP  = hang up
     * POLLNVAL = invalid request (fd not open or EBADF)
     */
    pfds[0].fd = 0;
    pfds[0].events = POLLIN;

    pfds[1].fd = lFd;
    pfds[1].events = POLLIN;

    /* 
     * int poll(struct pollfd *ufds, unsigned int nfds, int timeout)
     * *ufds   = file descriptor structure
     * nfds    = total file descriptors in ufds
     * timeout = blocking time (-1 = infinite, 0 = zero, >0 = timeout in ms)
     *
     * struct pollfd {int fd; short events; short revents; }
     * fd      = file descriptor
     * events  = requested events
     * revents = returned events
     *
     * poll() vs ppoll(), ppoll is for I/O multiplexing and atomic test
     */
    lRet = poll(pfds, 2, -1);

    /*
    * return of poll:
    * > 0 for ready (number of structures with nonzero revents)
    *   0 for not ready
    * < 0 for an error
    */
    if (lRet < 0)
    {
      printf("an error occured, errno %c\n", errno);
      return -1;
    }

    if (pfds[0].revents & POLLIN) 
    {
      printf("read on file descriptor %d\n", pfds[0].fd);
      lRet = read(0, lBuf, 1024);
      
      if (!lRet) 
      {
        printf("stdin closed\n");
        return 0;
      }
      printf("read (descriptor %d), return %d\n", pfds[0].fd, lRet);
      // write to stdout
      write(1, lBuf, lRet);
      printf("\n");
    }

    if (pfds[1].revents & POLLIN) 
    {
      printf("read on file descriptor %d\n", pfds[1].fd);
      lRet = read(lFd, lBuf, 1024);
      
      if (!lRet)
      {
        printf("file closed\n");
        return 0;
      }
      printf("read (descriptor %d), return %d\n", pfds[1].fd, lRet);
      // write to stdout
      write(1, lBuf, lRet);
      printf("\n");
    }
  }
  return 0;
}

static int readWithSelect(const char* aFilename_p)
{
  printf("example for read with select and a pipe\n");
  /*
   * requires a pipe, therefore make a pipe with mknod mypipe p
   */

  int lRet;
  char lBuf[1024];
  int lFd = open(aFilename_p, O_RDONLY);
  
  if (lFd < 0)
  {
    printf("an error occured, errno %d\n", errno);
    return -1;
  }
 
  printf("file open with file descriptor %d\n", lFd);
  
  fd_set lReadFds, lWriteFds;

  while (1) 
  {
    /*
     * file descriptors:
     * 0 = stdin
     * 1 = stdout
     * 2 = stderr
     */
    FD_ZERO(&lReadFds);
    FD_ZERO(&lWriteFds);
    FD_SET(0, &lReadFds);
    FD_SET(lFd, &lReadFds);

    /* 
     * int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
     * nfds       = highest numbered file descriptor
     * *readfds   = file descriptors listed to be ready for reading
     * *writefds  = file descriptors listed to be ready for write
     * *exceptfds = file descriptors listed to be ready for exceptional conditions
     * timeout    = interval that select() should block waitig for a file descriptor to become ready
     *
     * select() vs pselect(), pselect is for I/O multiplexing and atomic test
     */
    lRet = select(100, &lReadFds, &lWriteFds, NULL, NULL);

    /*
    * return of select:
    * > 0 for ready (number of file descriptors -> total number of bits that are set in readfds, writefds and exceptfds)
    *   0 for not ready
    * < 0 for an error
    */
    if (lRet < 0)
    {
      printf("an error occured, errno %c\n", errno);
      return -1;
    }

    if (FD_ISSET(0, &lReadFds))
    {
      printf("read on file descriptor 0\n");
      lRet = read(0, lBuf, 1024);
      FD_CLR(0, &lReadFds);
      
      if (lRet < 0)
      {
        printf("an error occured, errno %c\n", errno);
        return -1;
      }
      else if (lRet == 0)
      {
        printf("stdin closed\n");
        return 0;
      }
      printf("read (descriptor 0), return %d\n", lRet);
      // write to stdout
      write(1, lBuf, lRet);
      printf("\n");
    }

    if (FD_ISSET(lFd, &lReadFds))
    {
      printf("read on file descriptor %d\n", lFd);
      lRet = read(lFd, lBuf, 1024);
      FD_CLR(lFd, &lReadFds);

      if (lRet < 0)
      {
        printf("an error occured, errno %c\n", errno);
        return -1;
      }
      else if (lRet == 0)
      {
        printf("file closed\n");
        return 0;
      }
      printf("read (descriptor %d), return %d\n", lFd, lRet);
      // write to stdout
      write(1, lBuf, lRet);
      printf("\n");
    }
  }
  return 0;
}

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("invalid arguments\n");
    printf("argument 1: file name\n");
    printf("argument 2: file content\n");
    printf("argument 3: fork [y/n]\n");
    printf("argument 4: execution no\n 0 = blocking write (with/without fork)\n1 = read and write with poll through a pipe\n2 = read and write with select through a pipe\n");
    return -1;
  }

  printf("start %s\n", argv[0]);
  const char* lFilename_p = argv[1];
  const char* lFilecontent_p = argv[2];

  printf("manipulating file is %s\n", argv[1]);
  if (strcmp(argv[4], "0") == 0)
  {
    blockingWrite(lFilename_p, lFilecontent_p, argv[3]);
  }
  else if (strcmp(argv[4], "1") == 0)
  {
    readWithPoll(lFilename_p);
  }
  else if (strcmp(argv[4], "2") == 0)
  {
    readWithSelect(lFilename_p);
  }


  

  return 0;
}
