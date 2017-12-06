#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
void tailbyte(char* path, long long int bytestoprint, int pflag)
{
	int fd;
	char line[1];
	int status;
	long long int size=0;
	fd = open(path, 0);
	while((status = read(fd, line, sizeof(line))) > 0)
	{
		size++;
	}
	if(status<0)
	{
		printf(1, "Error read %s\n", path);
		return;
	}
	long long int skip = size - bytestoprint;
	if(pflag == 3)
	{
		if(bytestoprint > size)
		skip = size;
		else
		skip = bytestoprint-1;
	}
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
	while(skip>0)
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

void tailline(char* path, long long int linestoprint, int pflag)
{
	int fd;
	char line[1];
	int status;
	long long int totline=0;
	fd = open(path, 0);
	while((status = read(fd, line, sizeof(line))) > 0)
	{
		if(line[0] == '\n')
		totline++;
	}
	if(status<0)
	{
		printf(1, "Error read %s\n", path);
		return;
	}
	long long int skip = totline - linestoprint;
	if(pflag == 3)
	{
		if(linestoprint > totline)
		skip = totline;
		else
		skip = linestoprint - 1;
	}
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
	while(skip>0)
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
	long long int a;
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
					printf(1, "Error: Usage tail -n (+)<NUM> <filename>\n");
				}
				else
				{
					int i;
					if(argv[2][0] == '+')
					{
						char* b = argv[2]+1;
						a=atoi(b);
						if(a==0)
						{
							printf(1, "Error: Usage tail -n <NUM> <filename>\n");
						}
						else
						{
							for(i=3;i<argc;i++)
							{
								fd = open(argv[i], 0);
								if(fd < 0)
								printf(1, "Error: cannot open %s\n", argv[i]);
								else
								{
									close(fd);
									tailline(argv[i], a, 3);
								}
							}
						}
					}
					else
					{
						a=atoi(argv[2]);
						if(a==0)
						{
							printf(1, "Error: Usage tail -n <NUM> <filename>\n");
						}
						else
						{
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
				}
			}
			else if(argv[1][1] == 'c')
			{
				if(argc < 4)
				{
					printf(1, "Error: Usage tail -c (+)<NUM> <filename>\n");
				}
				else
				{
					int i;
					if(argv[2][0] == '+')
					{
						char* b = argv[2]+1;
						a=atoi(b);
						if(a==0)
						{
							printf(1, "Error: Usage tail -c +<NUM> <filename>\n");	
						}
						else
						{
							for(i=3;i<argc;i++)
							{
								fd = open(argv[i], 0);
								if(fd < 0)
								printf(1, "Error: cannot open %s\n", argv[i]);
								else
								{
									close(fd);
									tailbyte(argv[i], a, 3);
								}
							}
						}
					}
					else
					{
						a=atoi(argv[2]);
						if(a==0)
						{
							printf(1, "Error: Usage tail -c <NUM> <filename>\n");	
						}
						else
						{
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
			else
			{
				printf(1, "error: mode undefined\n");
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
	exit();
}
