/* simple program to hold file opend */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	FILE* fd = 0;
	fd = fopen("/proc/sleep", "r");	
	while(1) {
		printf("%s:File opened\n", __func__);
		sleep(30);
	}
	return 0;
}
