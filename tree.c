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
lsfile(char *path, int level, int fd1)
{
  char buf[512], *p;
  int fd;
  char a[4] = "|--";
  char n[2] = "\n";
  char s[3] = "  ";
  char b[2] = "|";
  int mode = 0;
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
    if(mode != 1)
    {
	    char* name;
	    name = fmtname(path);
	    if(level == 0)
	    {
		write(fd1, a, sizeof(a));
		write(fd1, fmtname(path), strlen(name));
		write(fd1, n, sizeof(n));
//		printf(1, "|--%s\n", fmtname(path));
	    }
	    else
	    {
		int count=0;
		write(fd1, b, sizeof(b));
//		printf(1, "|");
		for(count=0;count<level;count++)
		{
			write(fd1, s, sizeof(s));
//		    printf(1, "  ");
		}
		write(fd1, a, sizeof(a));
		write(fd1, fmtname(path), strlen(name));
		write(fd1, n, sizeof(n));
//		printf(1, "|--%s\n", fmtname(path));
	    }
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
	    char *name = fmtname(buf);
	    if(level == 0)
	    {
//		printf(1, "|--");
		write(fd1, a, sizeof(a));
		write(fd1, fmtname(buf), strlen(name));
		write(fd1, n, sizeof(n));
//		printf(1, "%s\n", fmtname(buf));
	    }
	    else
	    {
		int count=0;
		write(fd1, b, sizeof(b));
//		printf(1, "|");
		for(count=0;count<level;count++)
		{
		    write(fd1, s, sizeof(s));
//		    printf(1, "  ");
		}
		write(fd1, a, sizeof(a));
		write(fd1, fmtname(buf), strlen(name));
		write(fd1, n, sizeof(n));
//		printf(1, "|--");
//		printf(1, "%s\n", fmtname(buf));
	    }
	    if(name[0] != '.')
	    lsfile(buf, level+1, fd1);
      }
      else if(mode != 1)
      {
	    char *name;
	    name = fmtname(buf);
	    if(level == 0)
	    {
		write(fd1, a, sizeof(a));
		write(fd1, fmtname(buf), strlen(name));
		write(fd1, n, sizeof(n));
	//	printf(1, "|--");
	//	printf(1, "%s%s\n",NORMAL_COLOR, fmtname(buf));
	    }
	    else
	    {
		int count=0;
		write(fd1, b, sizeof(b));
//		printf(1, "|");
		for(count=0;count<level;count++)
		{
		    write(fd1, s, sizeof(s));
//		    printf(1, "  ");
		}
		write(fd1, a, sizeof(a));
		write(fd1, fmtname(buf), strlen(name));
		write(fd1, n, sizeof(n));
//		printf(1, "|--");
//		printf(1, "%s%s\n",NORMAL_COLOR, fmtname(buf));
	    }
	    
      }
    }
    break;
  }
  close(fd);
}

void
ls(char *path, int level, int mode)
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
    if(mode != 1)
    {
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
	    ls(buf, level+1, mode);
      }
      else if(mode != 1)
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
  int fd=-1;

  if(argc < 2){
    ls(".", 0, 0);
    exit();
  }
	else
	{
		if(argv[1][0] == '-')
		{
			if(argv[1][1] == 'd')
			{
				if(argc == 2)
				ls(".", 0, 1);
				else
				{
					for(i=2;i<argc;i++)
					{
						ls(argv[i], 0, 1);
					}
				}
			}
			else if(argv[1][1] == 'o')
			{
				if(argc < 3)
				{
					printf(1, "error: usage tree -o <filename> <path>\n");
				}
				fd = open(argv[2], O_CREATE | O_WRONLY);
				if(fd == -1)
				printf(1, "Error: cannot open %s\n", argv[2]);
				else
				{
					if(argc == 3)
					lsfile(".", 0, fd);
					else
					{
						for(i=3;i<argc;i++)
						{
							lsfile(argv[i], 0, fd);
						}
					}
				}
				close(fd);
			}
			else
			{
				printf(1, "error: undefined mode\n");
			}
		}
		else
		{
			int i;
			for(i=1;i<argc;i++)
			{
				ls(argv[i], 0, 0);
			}
		}
	}
  exit();
}
