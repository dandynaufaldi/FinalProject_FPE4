#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main (int argc, char *argv[])
{
  if (argc <= 2) {
	printf (1, "use : mv [file] [filedst]\n");
	exit();
  }

  int fd0,fd1;
  fd0= link(argv[1],argv[2]); // membuat link ke file
  fd1= unlink(argv[1]); // hapus file
  if ((fd0 < 0) || (fd1 < 0)) {
	printf(1, "mv [%s] to [%s] failed\n", argv[1], argv[2]);
  }
  exit();
}
