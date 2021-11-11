#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void errx(int exit_code, char* message) {
	printf("%s", message);
	exit(exit_code);
}

int fix_path(char *path)
{
  char resolved[128];
  
  if(realpath(path, resolved) == 0) return 1; 
  // can't access path. will error trying to open

  strcpy(path, resolved);
}

char *parse_http_request(char* bufferInput)
{
  char buffer[1024];
  char *path;
  char *q;


  if(strncpy(buffer, bufferInput, sizeof(buffer)) <= 0)
    errx(0, "Failed to read from remote host");
  if(memcmp(buffer, "GET ", 4) != 0) errx(0, "Not a GET request");

  path = &buffer[4];
  q = strchr(path, ' ');
  if(! q) errx(0, "No protocol version specified");


  *q++ = 0;
  if(strncmp(q, "HTTP/1.1", 8) != 0) errx(0, "Invalid protocol");

  fix_path(path);

  printf("trying to access %s\n", path);

  return path;
}

int main(int argc, char **argv, char **envp)
{
	char* payload = argv[1];
	printf("Payload: %s\n", payload);
  parse_http_request(payload); 
}
