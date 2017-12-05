#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

int strint(char* a)
{
	int result=0;
	int i=0;
	for(i=0; a[i] != '\0'; i++)
	{
		result = result*10 + a[i] - '0';
	}
	return result;
}

void tailbyte(char* path, int bytestoprint, int pflag)
{
	int fd;
	char line[1];
	int status;
	int size=0;
	fd = open(path, 0);
	while((status = read(fd, line, sizeof(line))) > 0)
	{
		size++;
	}
	if(status<0)
	{
		printf(1, "Error read file\n");
	}
	int skip = size - bytestoprint;
	if(skip < 0)
	{
		skip = 0;
		if(pflag)
		{
			printf(1, "==> %s <==\n", path);
		}
	}
	close(fd);
	fd = open(path, 0);
	while(skip)
	{
		status = read(fd, line, sizeof(line));
		skip--;
		
	}
	while((status = read(fd, line, sizeof(line))) > 0)
	{
		printf(1, "%c", line[0]);
	}
	close(fd);
}

void tailline(char* path, int linestoprint, int pflag)
{
	int fd;
	char line[1];
	int status;
	int totline=0;
	fd = open(path, 0);
	while((status = read(fd, line, sizeof(line))) > 0)
	{
		if(line[0] == '\n')
		totline++;
	}
	if(status<0)
	{
		printf(1, "Error read file\n");
	}
	int skip = totline - linestoprint;
	if(skip < 0)
	{
		skip = 0;
		if(pflag)
		{
			printf(1, "==> %s <==\n", path);
		}
	}
	close(fd);
	fd = open(path, 0);
	while(skip)
	{
		status = read(fd, line, sizeof(line));
		if(line[0] == '\n')
		{
			skip--;
		}
		
	}
	while((status = read(fd, line, sizeof(line))) > 0)
	{
		printf(1, "%c", line[0]);
	}
	close(fd);
}


int main(int argc, char* argv[])
{
	int a;
	int fd = -1;
	if(argc <=1)
	printf(1, "Usage: tail <mode> <filename>");
	else if(argc > 1)
	{
		if(argv[1][0] == '-')
		{
			if(argv[1][1] == 'n')
			{
				if(argc < 4)
				{
					printf(1, "Error: Usage tail -n <NUM> <filename>\n");
				}
				else
				{
					int i;
					a=strint(argv[2]);
					for(i=3;i<argc;i++)
					{
						fd = open(argv[i], 0);
						if(fd < 0)
						printf(1, "Error: cannot open %s\n", argv[i]);
						else
						{
							close(fd);
							tailline(argv[i], a, 1);
						}
					}
				}
			}
			else if(argv[1][1] == 'c')
			{
				if(argc < 4)
				{
					printf(1, "Error: Usage tail -c <NUM> <filename>\n");
				}
				else
				{
					int i;
					a=strint(argv[2]);
					for(i=3;i<argc;i++)
					{
						fd = open(argv[i], 0);
						if(fd < 0)
						printf(1, "Error: cannot open %s\n", argv[i]);
						else
						{
							close(fd);
							tailbyte(argv[i], a, 1);
						}
					}
				}
			}
			else if(argv[1][1] == 'q')
			{
				int i;
				for(i=2;i<argc;i++)
				{
					fd = open(argv[i], 0);
					if(fd < 0)
					printf(1, "Error: cannot open %s\n", argv[i]);
					else
					{
						close(fd);
						tailline(argv[i], 10, 0);
					}
				}
			}
			else if(argv[1][1] == 'v')
			{
				int i;
				for(i=2;i<argc;i++)
				{
					fd = open(argv[i], 0);
					if(fd < 0)
					printf(1, "Error: cannot open %s\n", argv[i]);
					else
					{
						close(fd);
						printf(1, "==> %s <==\n", argv[i]);
						tailline(argv[i], 10, 0);
					}
				}
				
			}
		}
		else
		{
			int i;
			for(i=1;i<argc;i++)
			{
				fd = open(argv[i], 0);
				if(fd < 0)
				printf(1, "Error: cannot open %s\n", argv[i]);
				else
				{
					close(fd);
					tailline(argv[i], 10, 1);
				}
			}
			
		}
		
	}
	return 0;
}
