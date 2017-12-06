#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
void sort(char* path, int mode, char* dest)
{
	int fd;
	fd = open(path, 0);
	char line[1];
	int status;
	long long int totline=0;
	fd = open(path, 0);
	if(fd < 0)
	{
		printf(1, "Error: cannot open %s\n", path);
		return;
	}
	while((status = read(fd, line, sizeof(line))) > 0)
	{
		if(line[0] == '\n')
		totline++;
	}
	if(status<0)
	{
		printf(1, "Error: cannot read %s\n", path);
		return;
	}
	char** arr;
	arr = (char**)malloc(totline * sizeof(char*));
	close(fd);
	int i=0;
	for(i=0;i<totline;i++)
	{
		arr[i] = (char*)malloc(256 * sizeof(char));
	}
	i=0;
	int j=0;
	int fd3;
	fd3 = open(path, O_RDWR);
//	printf(1, "Done counting\n");
//	printf(1, "lines = %lld\n", totline);
	while((status = read(fd3, line, sizeof(line))) > 0)
	{
//		printf(1, "take char\n");
		arr[i][j] = line[0];
		j++;
		if(line[0] == '\n')
		{
			
			arr[i][j] = '\0';
			i++;
			j=0;
//			printf(1, "take in\n");
		}
	}
	if(status<0)
	{
		printf(1, "Error: cannot read %s\n", path);
		return;
	}
	printf(1, "Done Taking\n");
	char temp[256];
	for(i=0;i<totline-1;i++)
	{
		for(j=0;j<(totline-i-1);j++)
		{
	//		printf(1, "j=%d j+1=%d\n",j, j+1);
			int k=j+1;
			if(strcmp(arr[j],arr[k]) > 0)
			{
	//			printf(1, "swap\n");
				strcpy(temp, arr[j]);
				strcpy(arr[j], arr[k]);
				strcpy(arr[k], temp);
			}
		}
	}
	printf(1, "Done sorting\n");
	if(mode == 0)
	{
		printf(1, "printing\n");
		for(i=totline;i>0;i--)
		{
			printf(1, "%s", arr[i-1]);
		}
	}
	else if(mode == 1)
	{
		printf(1, "printing\n");
		for(i=0;i<totline;i++)
		{
			printf(1, "%s", arr[i]);
		}
	}
	else if(mode == 2)
	{
		printf(1, "printing\n");
		int fd2=-1;
		fd2 = open(dest, O_CREATE | O_WRONLY);
		if(fd2 < 0)
		{
			printf(1, "Error: cannot open %s\n", dest);
			close(fd);
			return;
		}
		for(i=0;i<totline;i++)
		{
			write(fd2, arr[i], strlen(arr[i]));
		}
		close(fd2);
	}
	close(fd3);
	return;
}
int main(int argc, char* argv[])
{
	if(argc < 2)
	printf(1, "usage: sort <mode> <filename>");
	else
	{
		if(argv[1][0] == '-')
		{
			if(argv[1][1] == 'o')
			{
				sort(argv[3], 2, argv[2]);
			}
			else if(argv[1][1] == 'r')
			{
				sort(argv[2], 0, argv[1]);
			}
			else
			{
				printf(1, "Error: mode undefined\n");
			}
		}
		else if(argc == 2)
		{
			sort(argv[1], 1, argv[1]);
		}
	}
	exit();
}
