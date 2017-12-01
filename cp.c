#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

int
main(int argc, char *argv[])
{
  if(argc <= 2){
    printf(1, "cp : should be 2 arguments\n");
    exit();
  }
  int fd0 = open(argv[1], O_RDONLY);
  if (fd0<0) {
    printf(1, "cp: cannot open %s\n", argv[1]);
    exit();
  }
  int fd1 = open(argv[2], O_CREATE|O_RDWR);
  if (fd1<0) {
    printf(1, "cp: cannot open %s\n", argv[2]);
    exit();
  }
  int n;
  while((n=read(fd0, buf, sizeof(buf)))>0){
    write(fd1, buf, n);
  }
  close(fd0);
  close(fd1);
  exit();
}
