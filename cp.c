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
  if (argv[1][0]=='*'){
    printf(1, "cp : cp * not yet implemented\n");
    exit();
  }
  if (argv[1][0]=='-'){
    printf(1, "cp : cp -R not yet implemented\n");
    exit();
  }
  int fd0 = open(argv[1], O_RDONLY);
  if (fd0<0) {
    printf(1, "cp: cannot open %s\n", argv[1]);
    exit();
  }
  
  int fd1 = open(argv[2], O_RDONLY);
  //printf(1, "fd1 = %d\n", fd1);
  if(fd1<0){
    close(fd1);
    fd1 = open(argv[2], O_RDWR|O_CREATE);
    if (fd1<0){
      printf(1, "cp: cannot open %s\n", argv[2]);
      close(fd1);
      exit();
    }
  }
  struct stat st;
  if(fstat(fd1, &st) < 0){
    printf(2, "cp: cannot stat %s\n", argv[2]);
    close(fd1);
    exit();
  }

  switch(st.type){
  case T_FILE:
    //printf(1, "open file %s\n", argv[2]);
    close(fd1);
    fd1 = open(argv[2], O_RDWR);
    
    break;

  case T_DIR:
    //printf(1, "open directory %s\n", argv[2]);
    close(fd1);
    char FPE4[1000];
    strcpy(FPE4, argv[2]);
    strcat("/", FPE4);
    int lensrc = strlen(argv[1]);
    int lendst = strlen(FPE4);
    if(FPE4[lendst-1]=='/'){ //copy ke directory
      int i = lensrc-1;
      while(i--){
        if (argv[1][i]=='/') break;
      }
      i++;
      strcpy(&FPE4[lendst], &argv[1][i]);
    }
    int fd1 = open(FPE4, O_CREATE|O_RDWR);
    if (fd1<0) {
      printf(1, "cp: cannot open %s\n", FPE4);
      exit();
    }
    break;
  }
  int n;
    while((n=read(fd0, buf, sizeof(buf)))>0){
      write(fd1, buf, n);
  }
  close(fd0);
  close(fd1);
  exit();
}
