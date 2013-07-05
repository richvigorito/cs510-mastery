#include <stdio.h>
#include <math.h>

int modulo(int a, int b){ 
//modulo(a,b) = a/b, ie a = divident, b = divisor

	if (0 == b) {
		printf("Zero Divide\n");
		return 0;
	}	
	else if (a < 0 && b < 0)
		return -modulo(-a,-b);
	else if (a < 0)
		return -modulo(-a,b);
	else if (b < 0)
		return modulo(a,-b);
	else if (a == b)
		return 0;
	else if (1 == b)
		return 0;
	else if (a < b)
		return a;
	else 
		return modulo(sub(a,b),b);
}


int div(int a, int b){ 
//div(a,b) = a/b, ie a = divident, b = divisor

	int q = 0 ; // quotient

	if (b == 1)
		return a;

	else if (0 == b) {
		printf("Zero Divide\n");
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
		q = add(++q,div(sub(a,b),b));
		return q;
	}
}

int expo(int a, int b){
// expo(a,b) ==  a ^ b

	if (-1 == a)
		return (modulo(b,2)) ? -1 : 1;
	else if (0 == b) 
		return 1;
	else if (1 == a)  
		return 1;
	else if (0 == a)  
		return 0;
	else if (0 > b)
		return 0;
	else 
		return mult(expo(a,--b),a);
}

int mult(int a, int b){
// mult(a,b) == a*b

	if (1 == b)				// 1*b = b
		return a;
	else if (1 == a)			// 1*a = a
		return b;
	else if (0 == b || 0 == a) 		// 0 * a = 0 OR 0 * b = 0
		return 0;
	else if (a < 0 && b < 0)		// -a * -b  = a*b
		return mult(-a,-b);
	else if (a < 0)				// -a*b = -(a*b)
		return -mult(-a,b);
	else if (b < 0)				// a*-b = -(a*b)
		return -mult(a,-b);
	else 
		return add(mult(a,--b),a);     // 

}

int sub(int a, int b){ 
// sub(a,b) == a-b
	return add(a,-b); // a - b == a + - b
}

int add(int a, int b){
// add(a,b) == a+b
	if (0 == b)
		return a;
	else if ( b < 0 ) 			// b is less than 0, work to get b to base case via increment
		return add(--a,++b); 
	else 		  			// b is more than 0, work to get b to base case via decrement
		return add(++a,--b);

}

void test_expo(int lower,int upper){
	int a,b,c;	
	printf("Power: %d..%d ** %d..%d\n", lower,upper,lower,upper);
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = expo(a,b);
			printf("%d ** %d = %d \n", a,b,c);
			if(b<0)
				continue;
			if (c != pow(a,b))
				printf("ERROR: %f != %d \n", pow(a,b),c);
		}
	}
	printf("\n");
}

void test_modulo(int lower,int upper){
	int a,b,c;	
	printf("Modulus: %d..%d %% %d..%d\n", lower,upper,lower,upper);
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			if(0 == b)
				continue;
			c = modulo(a,b);
			printf("%d %% %d != %d \n", a,b,c);
			if (c != (a % b))
				printf("ERROR %d != %d \n", c,a%b);
		}
	}
	printf("\n");
}

void test_div(int lower,int upper){
	int a,b,c;	
	printf("Division: %d..%d / %d..%d\n", lower,upper,lower,upper);
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = div(a,b);
			printf("%d / %d = %d \n", a,b,c);
			if(0 == b)
				continue;
			if (c != ceil(a / b))
				printf("ERROR %d / %d != %f \n", a,b,ceil(a / b));
		}
	}
	printf("\n");
}

void test_mult(int lower,int upper){
	int a,b,c;	
	printf("Multiplication: %d..%d * %d..%d\n", lower,upper,lower,upper);
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = mult(a,b);
			printf("%d * %d = %d \n", a,b,c);
			if (c != (a*b))
				printf("ERROR %d * %d != %d \n", a,b,a*b);
		}
	}
	printf("\n");
}

void test_sub(int lower,int upper){
	int a,b,c;	
	int failed = 0;
	printf("Subtraction: %d..%d - %d..%d\n", lower,upper,lower,upper);
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = sub(a,b);
			printf("%d - %d = %d \n", a,b,c);
			if (c != (a-b))
				printf("%d - %d != %d \n", a,b,a-b);
		}
	}
	printf("\n");
}

void test_add(int lower,int upper){
	int a,b,c;	
	printf("Addition: %d..%d + %d..%d \n", lower,upper,lower,upper);
 	for (a = lower; a <= upper; a++){
 		for (b = lower; b <= upper; b++){
			c = add(a,b);
			printf("%d + %d = %d \n", a,b,c);
			if (c != (a+b))
				printf("%d + %d != %d \n", a,b,a+b);
		}
	}
	printf("\n");
}

int main()
{
	test_add(-6,5);
	test_sub(-6,5);
	test_mult(-6,5);
	test_div(-6,5);
	test_expo(-6,5);
//	test_modulo(-6,5);
	return 0;
}
