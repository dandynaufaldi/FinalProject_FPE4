#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
	int i;
	for(i=1;i<argc;i++){
		int fd;
		fd = open(argv[i], O_CREATE);
		if(fd < 0){
		    printf(2, "cannot touch %s\n", argv[i]);
		   	exit();
  		}
  		printf(1, "Opening %s\n", argv[i]);
	}
	return 0;
}