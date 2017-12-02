#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

void cp_reg(char *arg1, char *arg2){
  int fd0 = open(arg1, O_RDONLY);
  if (fd0<0) {
    printf(1, "cp: cannot open %s\n", arg1);
    exit();
  }
  int fd1 = open(arg2, O_RDONLY);
  if(fd1<0){
    close(fd1);
    fd1 = open(arg2, O_RDWR|O_CREATE);
    if (fd1<0){
      printf(1, "cp: cannot open %s\n", arg2);
      close(fd1);
      exit();
    }
  }
  struct stat st;
  if(fstat(fd1, &st) < 0){
    printf(2, "cp: cannot stat %s\n", arg2);
    close(fd1);
    exit();
  }

  switch(st.type){
  case T_FILE:
    close(fd1);
    fd1 = open(arg2, O_RDWR);
    break;

  case T_DIR:
    close(fd1);
    char FPE4[1000];
    strcpy(FPE4, arg2);
    strcat("/", FPE4);
    int lensrc = strlen(arg2);
    int lendst = strlen(FPE4);
    if(FPE4[lendst-1]=='/'){ //copy ke directory
      int i = lensrc-1;
      while(i--){
        if (arg1[i]=='/') break;
      }
      i++;
      strcpy(&FPE4[lendst], &arg1[i]);
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
}

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
  cp_reg(argv[1], argv[2]);
  
  exit();
}
