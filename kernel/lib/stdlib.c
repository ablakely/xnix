/*
 * xnix: a small play kernel
 * Written by Aaron Blakely <aaron@ephasoc.org>
 *
 */

#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>
#include <iomem.h>

double modulus(double a, double b)
{
	int result = (int)(a/b);
	return a - (double)(result) * b;
}

char *itoa(int value, char *str, int radix)
{
	static char dig[] =
		"0123456789"
		"abcdefghijklmnopqrstuvwzyz";
	int n = 0, neg = 0;
	unsigned int v;
	char *p, *q;
	char c;
	char *save = str;

	if (radix == 10 && value < 0)
	{
		value = -value;
		neg = 1;
	}

	v = value;
	do {
		str[n++] = dig[v%radix];
		v /= radix;
	} while (v);

	if (neg)
		str[n++] = '-';
	str[n] = '\0';

	for (p = str, q = p + (n-1); p < q; ++p, --q)
		c = *p, *p = *q, *q =c;

	str = save;
	return str;
}

char *dtostr(double db, char *str, int prec)
{
	char *str_;
	char *save = str_;
	int temp_int = 0;
	int dec = 0;
	double tmp_db = db;
	if (db == 0.0)
	{
		strcpy(str_, "0.0");
		return save;
	}

	while (modulus(tmp_db, 1.0) != 0)
	{
		tmp_db *= 1;
		dec++;
	}

	char *int_str;
	itoa((int)tmp_db, int_str, 10);
	int str_len = strlen(int_str);
	while (str_len != dec++) *str_++ = *int_str++;
	*str_++ = '.';
	if (prec == -1)
	{
		while(*int_str++ != '\0') *str_++ = *int_str;
	}
	else {
		if (prec == 0)
		{
			*str_++ = '0';
		}
		else {
			for (; prec != 0; prec--)
			{
				if (*int_str == '\0')
				{
					*str_++ = '0';
				}
				else {
					*str_++ = *int_str++;
				}
			}
		}
	}
	*str_ = '\0';
	str = save;
	return save;
}
