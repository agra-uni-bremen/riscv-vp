// 06/17/21, RD: variable_arguments.c, example expanded from
// https://www.tutorialspoint.com/cprogramming/c_variable_arguments.htm

#include <stdio.h>
#include <stdarg.h>

int char_average(int num,...) {
   va_list valist;
   char sum = 0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);
   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, int); // 'char' is promoted to 'int' when passed through '...'
   }
   /* clean memory reserved for valist */
   va_end(valist);
   return sum/num;
}

int short_average(int num,...) {
   va_list valist;
   short sum = 0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);
   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, int); // 'short' is promoted to 'int' when passed through '...'
   }
   /* clean memory reserved for valist */
   va_end(valist);
   return sum/num;
}

int int_average(int num,...) {
   va_list valist;
   int sum = 0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);
   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, int);
   }
   /* clean memory reserved for valist */
   va_end(valist);
   return sum/num;
}

int l_average(int num,...) {
   va_list valist;
   long sum = 0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);
   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, long);
   }
   /* clean memory reserved for valist */
   va_end(valist);
   return sum/num;
}

int ll_average(int num,...) {
   va_list valist;
   long long sum = 0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);
   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, long long);
   }
   /* clean memory reserved for valist */
   va_end(valist);
   return sum/num;
}

int f_average(int num,...) {
   va_list valist;
   float sum = 0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);
   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, double); // 'float' is promoted to 'double' when passed through '...'
   }
   /* clean memory reserved for valist */
   va_end(valist);
   return sum/num;
}

int d_average(int num,...) {
   va_list valist;
   double sum = 0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);
   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, double);
   }
   /* clean memory reserved for valist */
   va_end(valist);
   return sum/num;
}

int ld_average(int num,...) {
   va_list valist;
   long double sum = 0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);
   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, long double);
   }
   /* clean memory reserved for valist */
   va_end(valist);
   return sum/num;
}

int main() {
   printf("Char:%u Average of 2, 3, 4, 5 = %d\n", sizeof(char),
			char_average(4, '\02','\03','\04','\05'));
   printf("Char:%u Average of 5, 10, 15  = %d\n", sizeof(char),
			char_average(3, '\05','\012','\017'));
   printf("Shrt:%u Average of 2, 3, 4, 5 = %d\n", sizeof(short),
			short_average(4, (short)2,(short)3,(short)4,(short)5));
   printf("Shrt:%u Average of 5, 10, 15  = %d\n", sizeof(short),
			short_average(3, (short)5,(short)10,(short)15));
   printf("Int: %u Average of 2, 3, 4, 5 = %d\n", sizeof(int),
			int_average(4, 2,3,4,5));
   printf("Int: %u Average of 5, 10, 15  = %d\n", sizeof(int),
			int_average(3, 5,10,15));
   printf("Long:%u Average of 2, 3, 4, 5 = %d\n", sizeof(long),
			l_average(4, 2l,3l,4l,5l));
   printf("Long:%u Average of 5, 10, 15  = %d\n", sizeof(long),
			l_average(3, 5l,10l,15l));
   printf("LL:  %u Average of 2, 3, 4, 5 = %d\n", sizeof(long long),
			ll_average(4, 2ll,3ll,4ll,5ll));
   printf("LL:  %u Average of 5, 10, 15  = %d\n", sizeof(long long),
			ll_average(3, 5ll,10ll,15ll));
   printf("F:   %u Average of 2, 3, 4, 5 = %d\n", sizeof(float),
			f_average(4, 2.f,3.f,4.f,5.f));
   printf("F:   %u Average of 5, 10, 15  = %d\n", sizeof(float),
			f_average(3, 5.f,10.f,15.f));
   printf("D:   %u Average of 2, 3, 4, 5 = %d\n", sizeof(double),
			d_average(4, 2.0,3.0,4.0,5.0));
   printf("D:   %u Average of 5, 10, 15  = %d\n", sizeof(double),
			d_average(3, 5.0,10.0,15.0));
   printf("LD: %u Average of 2, 3, 4, 5 = %d\n", sizeof(long double),
			ld_average(4, 2.0L,3.0L,4.0L,5.0L));
   printf("LD: %u Average of 5, 10, 15  = %d\n", sizeof(long double),
			ld_average(3, 5.0L,10.0L,15.0L));
   return 0;
}

// EOF
