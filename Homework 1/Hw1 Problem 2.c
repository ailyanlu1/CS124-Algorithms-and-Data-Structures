// Harvard ID 80977816

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int fib_rec(int x);
int fib_iter(int n);
int fib_matrix(int n);
int* matrix_mult(int a_row, int a_col, unsigned int a[a_row][a_col], 
	             int b_row, int b_col, unsigned int b[b_row][b_col]);
int timer(float time, int (*f)(int));

int main(void){
	
	printf("n, F_n, time_elapsed\n");
	timer(60.0, &fib_rec);
	for(int i = 0; i < 80; i++) {printf("#");} printf("\n");
	timer(60.0, &fib_iter);
	for(int i = 0; i < 80; i++) {printf("#");} printf("\n"); 
	timer(60.0, &fib_matrix);
 
	return 0;
	
}

int fib_rec(int n){
	if(n <= 0)
		return 0;
	else if (n == 1)
		return 1;
	else
		return (fib_rec(n - 1) + fib_rec(n - 2)) % 65536 ;
}

int fib_iter(int n){
	int x = 0;
	int y = 1;

	for(int i = 0; i < n; i++){
		int tmp = x + y;
		x = y % 65536;
		y = tmp % 65536;
	}
	return x;
}

int fib_matrix(int n){
	/* 
	 * These are unsigned because during the multiplication we sometimes get numbers whose product
	 * is larger than 2^31 but which are each less than 2^16, so even taking mod 65536
	 * doesn't prevent overflow
	 */
	unsigned int matrix[2][2] = {{0,1},{1,1}};
	unsigned int result[2][2] = {{1,0},{0,1}};
	unsigned int fibs[2][1] = {0,1};

	while(n > 0){
		if((n % 2) == 1){
			int *tmp = matrix_mult(2,2, matrix, 2,2, result);
			for(int i = 0; i < 2; i++){
				for(int j = 0; j < 2; j++){
					result[i][j] = *(tmp + i * 2 + j);
				}
			}
			free(tmp);
		}

		int *tmp = matrix_mult(2, 2, matrix, 2, 2, matrix);
		// Copy result matrix
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 2; j++){
				matrix[i][j] = *(tmp + i * 2 + j);
			}
		}
		free(tmp);

		n = n >> 1;
	}

	return (matrix_mult(2, 2, result, 2, 1, fibs))[0];
	
}

int* matrix_mult(int a_row, int a_col, unsigned int a[a_row][a_col], 
				 int b_row, int b_col, unsigned int b[b_row][b_col]){
	
	int* res = malloc(sizeof(int) * a_row * b_col);
	
	if(a_col != b_row){
		return NULL;
	}
	
	for(int i = 0; i < a_row; i++){
		for(int k = 0; k < b_col; k++){
			int sum = 0;
			for(int j = 0; j < a_col; j++){
				sum += a[i][j] * b[j][k] % 65536;
			}
			*(res + i * a_row + k) = sum % 65536;
		}
	}

	return res;
}

int timer(float time, int (*f)(int)){

	double time_elapsed = 0.0;
	int result;
	int n = 0;

	while (time_elapsed < time){
		
		clock_t begin = clock();
		result = f(n);
		clock_t end = clock();

		time_elapsed += (double)(end - begin) / CLOCKS_PER_SEC;
		if (*f == &fib_rec)
			printf("%d, %d, %f\n", n, result, time_elapsed);

		else if (n!= 0){
			if(*f == &fib_iter && ((n % 10000) == 0))
				printf("%d, %d, %f\n", n, result, time_elapsed);
			else if (*f == &fib_matrix && ((n % 1000000) == 0))
				printf("%d, %d, %f\n", n, result, time_elapsed);
		}

		
		n++;
	}
	printf("%d, %d, %f\n", n - 1, result, time_elapsed);
	return result;
}