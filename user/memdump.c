#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len) {
  int fmt_index = 0;
  int i = 0;
  char* data_start = data;

  while (i < len && fmt[fmt_index] != '\0') {
    char c = fmt[fmt_index];

    if (c == 'i') {

      if ((len - i) < 4) {
        printf("memdump: not enough data for 'i'\n");
        return;
      }

      printf("%d\n", *((int*)(data)));
      data += 4;
      i += 4;
    }
    if (c == 'p') {

      if ((len - i) < 8) {
        printf("memdump: not enough data for 'p'\n");
        return;
      }

      printf("%llx\n", *(unsigned long long *)data);
      data += 8;
      i += 8;
    }
    if (c == 'h') {

      if ((len - i) < 2) {
        printf("memdump: not enough data for 'h'\n");
        return;
      }

      printf("%u\n", *(unsigned short *)data);
      data += 2;
      i += 2;
    }
    if (c == 'c') {

      if ((len - i) < 1) {
        printf("memdump: not enough data for 'c'\n");
        return;
      }

      printf("%c\n", *data);
      data += 1;
      i += 1;
    }
    if (c == 's') {

      if ((len - i) < 8) {
        printf("memdump: not enough data for 's'\n");
        return;
      }
      printf("%s\n", (char *)(*(uint64 *)(data)));      
      data += 8;
      i += 8;
    }
    if (c == 'S') {

      while (i < len) {
        printf("%c", data_start[i]);
        i++;
      }

      printf("\n");

    }

    fmt_index++;
  }

}

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Example 1:\n");
    int a[2] = {61810, 2026};
    memdump("ii", (char *)a, sizeof(a));

    printf("Example 2:\n");
    memdump("S", "a string", sizeof("a string"));

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *)&s, sizeof(s));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char *)&example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char *)&example, sizeof(example));
  } else if (argc == 2) {
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while (n < sizeof(data)) {
      int nn = read(0, data + n, sizeof(data) - n);
      if (nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, n);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}