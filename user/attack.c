#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define DATASIZE (40 * 4096)

int
main(int argc, char *argv[])
{
  // Your code here.
  char *data = sbrk(DATASIZE);
  unsigned long offset = 0;
  char secret[256];
  int i = 0;
  char marker[] = "Here it is: ";

  // unsigned long align = ((unsigned long) data) % 4096;

  // printf("%d - %d\n", (int)align, offset);
  // printf("attack: %ld", offset);

  while(offset < DATASIZE){
    if(data[offset] == 'H'){
      int found = 1;
      for(int j=0 ; j<strlen(marker) ; j++){
        if(data[offset + j] != marker[j]){
          found = 0;
          break;
        }
      }

      if(found){
        offset += strlen(marker);

        while(data[offset] != '\0'){
          secret[i++] = data[offset];
          offset++;
        }
        secret[i] = '\0';
        break;
      }
    }
    offset++;
  }
  printf("%s\n", secret);

  exit(0);
}