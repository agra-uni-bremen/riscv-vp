// printf.c: (05/09/21, RD)
// experiments (extended from printf/main.c)

#include "stdio.h"
#include "errno.h"
#include "string.h"
#include "unistd.h"

int main(int argc, char **argv) {
	int n = printf("ABCDEFX %s\n", "Done");
	int e = errno;
	printf("n = %d\n", n);
	puts(strerror(e));

	puts("Hello World!");
	putchar('a');
	putchar('b');
	putchar('c');
	putchar('\n');
	printf("Hello World!\n");
	printf("printf(%%c, 'x') %c\n", 'x');
	printf("printf(%%d, 12) %d\n", 12);
	printf("printf(%%f, 1.2f) %f\n", 1.2f);
	printf("printf(%%f, 1.2) %f\n", 1.2);
	printf("printf(%%Lf, 1.2) %Lf\n", 1.2L);

	float f = 1.2f;
	double d = 1.2;
	long double ld = 1.2L;
	printf("printf(%%f, f) %f\n", f);
	printf("printf(%%f, d) %f\n", d);
	printf("printf(%%Lf, ld) %Lf\n", ld);

	f = 3.1415f;
	d = 3.1415;
	ld = 3.1415L;
	printf("printf(%%f, f) %f\n", f);
	printf("printf(%%f, d) %f\n", d);
	printf("printf(%%Lf, ld) %Lf\n", ld);

	return 0;
}
