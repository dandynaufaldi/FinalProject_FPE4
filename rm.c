#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  int i;
  // kalau char kurang dari 2 (hanya tulis rm)
  if(argc < 2){
    printf(2, "Cannot use, try rm [file] \n");
    exit();
  }
  // kalau filenya tidak berhubungan (tidak ada)
  for(i = 1; i < argc; i++) {
      if(unlink(argv[i]) < 0){
      printf(2, "rm: %s failed to delete\n", argv[i]);
      break;
    }
  }

  exit();
}
