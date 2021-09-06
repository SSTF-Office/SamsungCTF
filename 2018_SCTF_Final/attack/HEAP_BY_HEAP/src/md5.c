#include "md5.h"

char * HashFunction( char * v1 )
{
	MD5_CTX lctx;
	unsigned char digest[16];
	char *md5msg;
	int i;
	struct stat status;

	MD5_Init(&lctx);

	MD5_Update(&lctx, v1, strlen(v1));
	MD5_Final(digest, &lctx);

	md5msg = (char *)malloc(0x32);

	for (i = 0; i < sizeof(digest); ++i)
	{
		sprintf(md5msg+(i*2), "%02x", digest[i]);
	}
	return md5msg;
}
