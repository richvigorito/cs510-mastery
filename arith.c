#include <stdio.h>
#include <math.h>

int div(int a, int b){ //div(a,b) = a/b

	int quotient = 0 ;

	if (b == 1)
		return a;

	else if (0 == b) {
		printf("\nraise divsion by 0 error\n");
		return 0;	
	}

	else if (a < 0 && b < 0)
		return div(-a,-b);
	else if (a < 0)
		return -div(-a,b);
	else if (b < 0)
		return -div(a,-b);
	else if (b == a)
		return 1;
	else if (a < b)
		return 0;
	else {
		quotient = add(++quotient,div(sub(a,b),b));
		return quotient;
	}
}

int _pow(int a, int b){
// pow(a,b) ==  a ^ b
	if (-1 == a)  
		return ( b%2 ) ? -1 : 1;
	else if (0 == b) 
		return 1;
	else if (1 == a)  
		return 1;
	else if (0 == a)  
		return 0;
	else if (0 > b){
		printf("\nraise negative exponent error\n");
		return 0;
	}else 
		return mult(_pow(a,--b),a);
}

int mult(int a, int b){

	if (1 == b)
		return a;
	else if (1 == a)
		return b;
	else if (0 == b || 0 == a)
		return 0;
	else if (a < 0 && b < 0)	
		return mult(-a,-b);
	else if (a < 0)
		return -mult(-a,b);
	else if (b < 0)
		return -mult(a,-b);
	else 
		return add(mult(a,--b),a); // 

}


int sub(int a, int b){ 
	return add(a,-b); // a - b == a + - b
}

int add(int a, int b){
	if (0 == b)
		return a;
	else if ( b < 0 ) // b is less than 0, work to get b to base case via increment
		return add(--a,++b); 
	else 		  // b is more than 0, work to get b to base case via decrement
		return add(++a,--b);

}

void test_pow(int lower,int upper){
	int a,b,c;	
	int failed = 0;
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = _pow(a,b);
			if (c != pow(a,b)){
				printf("%d ^ %d = %d \n", a,b,c);
				printf("%f != %d \n", pow(a,b),c);
				failed = 1;	
			}
		}
	}
	if (failed){
		printf("\ntest pow function failed\n");
	} else {
		printf("\ntest pow function passed\n");
	}
}

void test_div(int lower,int upper){
	int a,b,c;	
	int failed = 0;
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			if(0 == b)
				continue;
			c = div(a,b);
			if (c != ceil(a / b)){
				printf("%d / %d = %d \n", a,b,c);
				failed = 1;	
			}
		}
	}
	if (failed){
		printf("\ntest div function failed\n");
	} else {
		printf("\ntest div function passed\n");
	}
}

void test_mult(int lower,int upper){
	int a,b,c;	
	int failed = 0;
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = mult(a,b);
			if (c != (a*b)){
				printf("%d * %d = %d \n", a,b,c);
				failed = 1;	
			}
		}
	}
	if (failed){
		printf("\ntest mult function failed\n");
	} else {
		printf("\ntest mult function passed\n");
	}
}

void test_sub(int lower,int upper){
	int a,b,c;	
	int failed = 0;
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = sub(a,b);
			if (c != (a-b)){
				printf("%d - %d = %d \n", a,b,c);
				failed = 1;	
			}
		}
	}
	if (failed){
		printf("\ntest sub function failed\n");
	} else {
		printf("\ntest sub function passed\n");
	}
}

void test_add(int lower,int upper){
	int a,b,c;	
	int failed = 0;
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = add(a,b);
			if (c != (a+b)){
				printf("%d + %d = %d \n", a,b,c);
				failed = 1;	
			}
		}
	}
	if (failed){
		printf("\ntest add function failed\n");
	} else {
		printf("\ntest add function passed\n");
	}
}

int main()
{
	test_add(-100,100);
	test_sub(-100,100);
	test_mult(-100,100);
	test_div(-100,100);
	test_pow(-1,5);

}
