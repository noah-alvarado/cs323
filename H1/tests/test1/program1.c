#include <stdio.h>
#include "CAT.h"

void my_execution (void){
	CATData	d1;
	CATData	d2;
	CATData	d3;

	d1	= CAT_new(5);
	printf("H1: 	Value 1 = %ld\n", CAT_get(d1));
	d2	= CAT_new(8);
	printf("H1: 	Value 2 = %ld\n", CAT_get(d2));

	d3	= CAT_new(0);
	CAT_add(d3, d1, d2);
	printf("H1: 	Result = %ld\n", CAT_get(d3));

	return ;
}

int main (int argc, char *argv[]){
	printf("H1: Begin\n");
	my_execution();
	printf("H1: End\n");

	return 0;
}
