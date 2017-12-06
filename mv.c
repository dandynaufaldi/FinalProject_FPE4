#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
int isparent(char *src, char *dest){
  int i=0, j=0;
  if (src[0]=='.' && src[1]=='/') i=2;
  if (dest[0]=='.' && dest[1]=='/') j=2;
  for(;i<strlen(src) && j<strlen(dest);i++, j++){
    if (src[i]!=dest[j]) return 0;
  }
  while(src[i] && src[i]!='/') i++;
  i++;
  while(src[i] && src[i]!='/') i++;
  if (src[i]=='/' && src[i+1]!='\0') return 0;
  return 1;
}

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
  *(buf + strlen(p)) = 0;
  //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void cp_reg(char *arg1, char *arg2){
  //printf(1, "cp: from %s to %s \n", arg1, arg2);
  char buf[512];
  int fd0 = open(arg1, O_RDONLY);
  if (fd0<0) {
    printf(1, "mv: cannot open source %s\n", arg1);
    exit();
  }
  int fd1 = open(arg2, O_RDONLY);
  if(fd1<0){
    close(fd1);
    fd1 = open(arg2, O_RDWR|O_CREATE);
    if (fd1<0){
      printf(1, "mv: cannot open dest %s\n", arg2);
      close(fd1);
      exit();
    }
  }
  struct stat st;
  if(fstat(fd1, &st) < 0){
    printf(2, "mv: cannot stat %s\n", arg2);
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
      printf(1, "mv: cannot open %s\n", FPE4);
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

void cp_r(char *src, char *dest){
  //printf(1, "cp: from %s to %s \n", src, dest);
  char buf[512];
  struct stat sts, std;
  if (stat(src, &sts)>=0 && stat(dest, &std)>=0 && sts.ino == std.ino){
    printf(1, "mv: %s and %s are the same\n", src, dest);
    exit();
  }
  else if (isparent(src, dest)){
    printf(1, "mv: cannot move, %s, to subdirectory itself, %s\n", src, dest);
    exit();
  }
  if (stat(src, &sts)<0){
    printf(1, "mv: no such source directory %s\n", src);
    exit();
  }
  
  //BUKAAAAA
  int cur = open(src, O_RDONLY);

  struct dirent de;
  struct stat st;
  char *p;

  strcpy(buf, src);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(cur, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, 512);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf(1, "mv: cannot stat %s\n", buf);
      continue;
    }

    if (st.type==T_DIR){
      if (fmtname(buf)[0]=='.') continue;
      char tempsrc[512], tempdest[512];
      strcpy(tempsrc, src);
      strcat("/", tempsrc);
      strcat(fmtname(buf), tempsrc);
      strcpy(tempdest, dest);
      strcat("/", tempdest);
      strcat(fmtname(buf), tempdest);
      if (isparent(tempsrc, tempdest)){
        printf(1, "mv: cannot move, %s, to subdirectory of itself, %s\n", tempsrc, tempdest);
        continue;
      }
      mkdir(tempdest);
      //printf(1, "cp: create dir %s\n", tempdest);
      cp_r(tempsrc, tempdest);
      unlink(tempsrc);
    }
    else{
      char tempsrc[512], tempdest[512];
      strcpy(tempsrc, src);
      strcat("/", tempsrc);
      strcat(fmtname(buf), tempsrc);
      strcpy(tempdest, dest);
      strcat("/", tempdest);
      strcat(fmtname(buf), tempdest);
      cp_reg(tempsrc, tempdest);
      unlink(tempsrc);
    }
  }
}

int main (int argc, char *argv[])
{
  if (argc <= 2) {
  printf (1, "use : mv [file] [filedst]\n");
  exit();
  }
  if (argv[1][0]!='*'){
    cp_reg(argv[1], argv[2]);
    int fd1;
    fd1= unlink(argv[1]); // hapus file
    if (fd1 < 0) {
      printf(1, "mv [%s] to [%s] failed\n", argv[1], argv[2]);
      exit();
    }
  }
  else if (argv[1][0]=='*'){
    cp_r(".", argv[2]);
  }
  exit();
}
