#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"



void tailline(char* path, int linestoprint)
{
	int fd;
	char line[1];
	int status;
	int totline=1;
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
	int fd = -1;
	if(argc <=1)
	printf(1, "Usage: tail <mode> <filename>");
	else if(argc == 2)
	{
		fd = open(argv[1], 0);
		if(fd < 0)
		printf(1, "Error: cannot open %s\n", argv[1]);
		else
		{
			close(fd);
			tailline(argv[1], 10);
		}
	}
	return 0;
}
