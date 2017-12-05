#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

//color https://stackoverflow.com/revisions/41520005/2
#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"

char*
fmtname(char *path)
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

void
ls(char *path, int level)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    printf(1, "ls: cannot open %s\n", path);
    return;
  }
  //printf(1 ,"fd=%d\n", fd);
  if(fstat(fd, &st) < 0){
    printf(1, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  //printf(1, "path = %s\n", path);
  switch(st.type){
  case T_FILE:
    if(level == 0)
    {
	printf(1, "|--%s\n", fmtname(path));
    }
    else
    {
	int count=0;
	printf(1, "|");
	for(count=0;count<level;count++)
	{
	    printf(1, "  ");
	}
	printf(1, "|--%s\n", fmtname(path));
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      if (st.type==T_DIR)
      {
	    if(level == 0)
	    {
		printf(1, "|--");
		printf(1, "%s\n", fmtname(buf));
	    }
	    else
	    {
		int count=0;
		printf(1, "|");
		for(count=0;count<level;count++)
		{
		    printf(1, "  ");
		}
		printf(1, "|--");
		printf(1, "%s\n", fmtname(buf));
	    }
	    char *name = fmtname(buf);
	    if(name[0] != '.')
	    ls(buf, level+1);
      }
      else
      {
	    if(level == 0)
	    {
		printf(1, "|--");
		printf(1, "%s%s\n",NORMAL_COLOR, fmtname(buf));
	    }
	    else
	    {
		int count=0;
		printf(1, "|");
		for(count=0;count<level;count++)
		{
		    printf(1, "  ");
		}
		printf(1, "|--");
		printf(1, "%s%s\n",NORMAL_COLOR, fmtname(buf));
	    }
	    
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".", 0);
    exit();
  }
  for(i=1; i<argc; i++)
    ls(argv[i], 0);
  exit();
}
