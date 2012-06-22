#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (!argv[1]) {
		printf("usage: %s <device>\n\n", argv[0]);
		printf("NOTE: Don't run this on a partition.  Use it on the device itself.\n");
		printf("The xnix team is not responable for any damage done to your computer or storage devices\n");
		printf("by incorrect usage of this tool.\n");
		exit(-1);
	}

	printf("%s: creating bootable USB of xnix (device=%s)\n", argv[0], argv[1]);
	printf("%s: building the floppy image.", argv[0]);
	system("cd ..;make floppy");
	char *syscmd;
	sprintf(syscmd, "cd ..;dd if=build/kernel.bin of=%s", argv[1]);
	printf("%s: copying image to device with dd.", argv[0]);
	system(syscmd);
	printf("%s: done.\n", argv[0]);
	return 0;
}
