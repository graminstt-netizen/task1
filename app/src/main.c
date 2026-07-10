/*
main.c - главный модуль программы. 

Бабурин Дмитрий Сергеевич
*/

#include <lib_main.h>
 
#include <stdio.h>


int main (int argc, char *argv[], char *envp[]) {

int a = 2;
int b = 3;

	printf ("%d + %d = %d\n", a, b, LibAddFunction(a, b));
	return 0;
}
