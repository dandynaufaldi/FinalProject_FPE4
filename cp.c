#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void cp_reg(char *arg1, char *arg2){
  char buf[512];
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
    if (FPE4[strlen(FPE4)-1] !='/') strcat("/", FPE4);
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

void cp_all(char *dest){
  char buf[512];
  int cur = open(".", O_RDONLY);
  if (cur<0) {
    printf(1, "cp: cannot open current dir\n");
    exit();
  }
  char *tmp = (char*)malloc(sizeof(char)*1024);
  strcpy(tmp, dest);
  if (tmp[strlen(tmp)-1]=='/'){
    tmp[strlen(tmp)-1] = 0;
  }
  int fdest = open(tmp, O_RDONLY);
    if (fdest<0) {        //folder not exist
    close(fdest);
    mkdir(tmp);
    fdest = open(tmp, O_RDONLY);
  }
  struct dirent de;
  struct stat st;
  char *p;

  strcpy(buf, ".");
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(cur, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    if(stat(buf, &st) < 0){
      printf(1, "cp: cannot stat %s\n", buf);
      continue;
    }

    if (st.type==T_DIR){
      if (fmtname(buf)[0]=='.') continue;
      printf(1, "cp : omitting %s\n", fmtname(buf));
    }
    else{
      cp_reg(buf, tmp);
    }
  }
}

int
main(int argc, char *argv[])
{
  if(argc <= 2){
    printf(1, "cp : should be 2 arguments\n");
    exit();
  }
  if (argv[1][0]=='*'){
    //printf(1, "cp : cp * not yet implemented\n");
    cp_all(argv[2]);
    exit();
  }
  if (argv[1][0]=='-'){
    printf(1, "cp : cp -R not yet implemented\n");
    exit();
  }
  cp_reg(argv[1], argv[2]);
  
  exit();
}
