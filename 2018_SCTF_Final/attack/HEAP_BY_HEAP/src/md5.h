#include <stdio.h>
#include <openssl/md5.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

char * HashFunction( char * v1 );
