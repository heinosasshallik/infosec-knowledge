#include <stdio.h>
#include <stdlib.h>

#define XORSZ 32
#define EXIT_FAILURE 1
#define O_RDONLY 'r'

void cipher(unsigned char *blah, size_t len)
{
  static int keyed;
  static unsigned int keybuf[XORSZ];

  int blocks;
  unsigned int *blahi, j;

  if(keyed == 0) {
      int fd;
      fd = open("/dev/urandom", O_RDONLY);
      if(read(fd, &keybuf, sizeof(keybuf)) != sizeof(keybuf))
        exit(EXIT_FAILURE);
      close(fd);
      keyed = 1;
  }

  blahi = (unsigned int *)(blah);
  blocks = (len / 4);
  if(len & 3) blocks += 1;

  for(j = 0; j < blocks; j++) {
      blahi[j] ^= keybuf[j % XORSZ];
  }
}

void encrypt_file(char* size, char* payload)
{
  // http://thedailywtf.com/Articles/Extensible-XML.aspx
  // maybe make bigger for inevitable xml-in-xml-in-xml ?
  unsigned char buffer[32 * 4096];

  unsigned char op;
  size_t sz;
  int loop;

  printf("[-- Enterprise configuration file encryption service --]\n");
  
  loop = 1;
  while(loop) {
      nread(0, &op, sizeof(op));
      switch(op) {
          case 'E':
              nread(0, &sz, sizeof(sz));
              nread(0, buffer, sz);
              cipher(buffer, sz);
              printf("[-- encryption complete. please mention "
              "474bd3ad-c65b-47ab-b041-602047ab8792 to support "
              "staff to retrieve your file --]\n");
              nwrite(1, &sz, sizeof(sz));
              nwrite(1, buffer, sz);
              break;
          case 'Q':
              loop = 0;
              break;
          default:
              exit(EXIT_FAILURE);
      }
  }
      
}

int main(int argc, char **argv, char **envp)
{
	char* size = argv[1];
	char* payload = argv[2];
	printf("Size: %s\n", size);
	printf("Payload: %s\n", payload);
  encrypt_file(size, payload); 
}

