#include "kbbuffer.h"
#include <io/kb/kb.h>

char kbbuffer[KB_BUF_LEN];

void init_kbbuffer()
{
	memset(&kbbuffer, 0, sizeof(char)*KB_BUF_LEN);
}

void put_kbbuf(char c)
{
	int count = sizeof(kbbuffer) / sizeof(kbbuffer[0]);
	count++;

	kbbuffer[count] = c;
}
