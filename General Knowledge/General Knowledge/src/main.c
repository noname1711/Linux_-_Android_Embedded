#include <stdio.h>
#include "sqrt_lib.h"

int main(){
	double num;
	scanf("%lf", &num);

	if (num < 0 ){
		printf("Number must be > 0");
	}else{
		printf("sqrt %.2lf is %.2lf\n", num, my_sqrt(num));
	}

	return 0; 
}
