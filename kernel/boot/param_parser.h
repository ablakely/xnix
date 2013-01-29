#ifndef BOOT_PARAM_PARSER_H
#define BOOT_PARAM_PARSER_H

#define PARAM_MAX 200
#define PARAM_LEN 500

typedef struct
{
	int  argc;
	char argv[PARAM_MAX][PARAM_LEN];
} bootparams_t;

bootparams_t *parse(char* cmdline);

#endif
