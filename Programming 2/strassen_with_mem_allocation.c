/*
 * Matt Leifer
 * March 10, 2017
 * Programming Assignment 2
 * CS124
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int* traditional(int a_row, int a_col, int* a, 
				 int b_row, int b_col, int* b);
int* traditional_square(int dimension, int* a, int* b);
int* strassen(int a_row, int a_col, int* a, 
			  int b_row, int b_col, int* b);
int* strassen_square(int dimension, int* a, int* b);
int* add(int a_row, int a_col, int* a, 
		 int b_row, int b_col, int* b, bool subtract);
int* add_square(int d, int* a, int* b);
int* sub_square(int d, int* a, int* b);
void print_matrix(int rows, int cols, int* m);
float timer(int n, int numtrials, int* (*f)(int, int*, int*));
int* gen_matrix(int n);
int cross_over;

int main(int argc, char* argv[]){
	if(argc != 4){
		printf("Usage: ./strassen flag dimension inputfile\n");
		return 1;
	}
	int flag = atoi(argv[1]);
	cross_over = flag;
	int dim = atoi(argv[2]);
	char* inputfile = argv[3];
	int* a = malloc(dim * dim * sizeof(int));
	int* b = malloc(dim * dim * sizeof(int));
	srand(time(NULL));
	FILE* file = fopen(inputfile, "r");
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
 
	if (file == NULL)
		exit(EXIT_FAILURE);
 	
 	int count = 0;
 	int* matrix = a;
	while ((read = getline(&line, &len, file)) != -1) {
		int value = atoi(line);
		if(count == dim * dim){
			count = 0;
			matrix = b;
		}
		*(matrix + count) = value;
		count++;
	}
	
	free(line);
	fclose(file);

	//print_matrix(dim, dim, a);
	//print_matrix(dim, dim, b);

	int* c = strassen(dim, dim, a, dim, dim, b);
	if(c == NULL){
		printf("Strassen failed\n");
		free(a);
		free(b);
		free(c);
		return 1;
	}
	print_matrix(dim, dim, c);

	int n = 1;
	printf("Crossover = %d\n", cross_over);
	int trials = 10;
	printf("Trials = %d\n", trials);
	printf("Size\tTraditional\tStrassen\n");
	while(n < 10000){
		if(n > 500)
			trials = 2;

		float trad_time = timer(n, trials, &traditional_square);
		float strassen_time = timer(n, trials, &strassen_square);

		printf("%d\t%f\t%f\n", n, trad_time, strassen_time);
		n *= 2;
	}

	free(a);
	free(b);
	free(c);
	return 0;
}

int* add(int a_row, int a_col, int* a, 
		 int b_row, int b_col, int* b, bool subtract){
	if(a_row != b_row || a_col != b_col){
		printf("Dimensions don't match\n");
		return NULL;
	}

	int* c = malloc(a_row * a_col * sizeof(int));
	for(int i = 0; i < a_row; i++){
		for(int j = 0; j < a_col; j++){
			if (subtract)
				*(c + i * a_row + j) = *(a + i * a_row + j) - *(b + i * a_row + j);
			else
				*(c + i * a_row + j) = *(a + i * a_row + j) + *(b + i * a_row + j);
		}
	}
	return c;
}

int* add_square(int d, int* a, int* b){
	return add(d, d, a, d, d, b, false);
}

int* sub_square(int d, int* a, int* b){
	return add(d, d, a, d, d, b, true);
}

int* traditional(int a_row, int a_col, int* a, 
				 int b_row, int b_col, int* b){
	
	int* res = malloc(sizeof(int) * a_row * b_col);
	
	if(a_col != b_row){
		return NULL;
	}
	
	for(int i = 0; i < a_row; i++){
		for(int k = 0; k < b_col; k++){
			int sum = 0;
			for(int j = 0; j < a_col; j++){
				sum += *(a + i * a_row + j) * *(b + j * b_row + k);
			}
			*(res + i * a_row + k) = sum;
		}
	}

	return res;
}

int* traditional_square(int d, int* a, int* b){
	return traditional(d, d, a, d, d, b);
}

int* strassen_square(int d, int* a, int* b){
	return strassen(d, d, a, d, d, b);
}

int* strassen(int a_row, int a_col, int* a, int b_row, int b_col, int* b){
	if(a_row != a_col || b_row != b_col || a_col != b_row){
		printf("Invalid matrix dimensions\n");
		return NULL;
	}

	int tmp = a_row;
	/*while(tmp / 2.0 == tmp >> 1 && tmp > 0){
		tmp = tmp / 2;
	}
	if(tmp != 1){
		printf("Strassen currently needs dimensions to be power of 2\n");
		return NULL;
	}*/

	if(a_row <= cross_over){
		int* c = traditional_square(a_row, a, b);
		return c;
	}

	bool is_odd = false;
	int d;
	if(a_row % 2 == 0){
		 d = a_row / 2;
	}
	else{
		d = a_row / 2 + 1;
		is_odd = true;
	}

	int offsets[4] = {0, d, a_row * d, a_row * d + d};
	
	int* block[8];
	for(int i = 0; i < 8; i ++){
		block[i] = malloc(sizeof(int) * d * d);
	}

	int* A = block[0];
	int* B = block[1]; //a + (a_col / 2);
	int* C = block[2]; //a + (a_row * a_col / 2);
	int* D = block[3]; //a + (a_row * a_col / 2 + a_col / 2);
	int* E = block[4];
	int* F = block[5]; // b + (b_col / 2);
	int* G = block[6]; // b + (b_row * b_col / 2);
	int* H = block[7]; // b + (b_row * b_col / 2 + b_col / 2);

	for(int bl = 0; bl < 4; bl++){
		for(int i = 0; i < d; i++){
			for(int j = 0; j < d; j++){
				if(is_odd && i == d - 1 && bl > 1){
					*(block[bl] + i * d + j) = 0;
					*(block[bl + 4] + i * d + j) = 0;
				}
				else if(is_odd && j == d - 1 && (bl == 1 || bl == 3)){
					*(block[bl] + i * d + j) = 0;
					*(block[bl + 4] + i * d + j) = 0;
				}
				else{
					*(block[bl] + i * d + j) = *(a + i * a_row + j + offsets[bl]);
					*(block[bl + 4] + i * d + j) = *(b + i * a_row + j + offsets[bl]);
				}
			}
		}
	}

	int* tmp1;
	int* tmp2;

	tmp1 = sub_square(d, F, H);
	int* P1 = strassen(d, d, A, d, d, tmp1);
	free(tmp1);

	tmp1 = add_square(d, A, B);
	int* P2 = strassen(d, d, tmp1, d, d, H);
	free(tmp1);

	tmp1 = add_square(d, C, D);
	int* P3 = strassen(d, d, tmp1, d, d, E);
	free(tmp1);

	tmp1 = sub_square(d, G, E);
	int* P4 = strassen(d, d, D, d, d, tmp1);
	free(tmp1);

	tmp1 = add_square(d, A, D);
	tmp2 = add_square(d, E, H);
	int* P5 = strassen(d, d, tmp1, d, d, tmp2);
	free(tmp1);
	free(tmp2);

	tmp1 = sub_square(d, B, D);
	tmp2 = add_square(d, G, H); 
	int* P6 = strassen(d, d, tmp1, d, d, tmp2);
	free(tmp1);
	free(tmp2);

	tmp1 = sub_square(d, A, C);
	tmp2 = add_square(d, E, F);
	int* P7 = strassen(d, d, tmp1, d, d, tmp2);
	free(tmp1);
	free(tmp2);

	tmp1 = add_square(d, P5, P4);
	tmp2 = sub_square(d, tmp1, P2);
	int* top_left = add_square(d, tmp2, P6);
	free(tmp1);
	free(tmp2);
	int* top_right = add_square(d, P1, P2);
	int* bottom_left = add_square(d, P3, P4);
	tmp1 = add_square(d, P5, P1);
	tmp2 = sub_square(d, tmp1, P3);
	int* bottom_right = sub_square(d, tmp2, P7);
	free(tmp1);
	free(tmp2);

	int* c = malloc(sizeof(int) * a_row * a_row);
	for(int i = 0; i < d; i++){
		for(int j = 0; j < d; j++){
			*(c + i * a_row + j) = *(top_left + i * d + j);
			if(!is_odd || (is_odd && j != d - 1))
				*(c + i * a_row + j + offsets[1]) = *(top_right + i * d + j);
			if(!is_odd || (is_odd && i != d - 1))
				*(c + i * a_row + j + offsets[2]) = *(bottom_left + i * d + j);
			if(!is_odd || (is_odd && i != d - 1 && j != d - 1))
				*(c + i * a_row + j + offsets[3]) = *(bottom_right + i * d + j);
		}
	}

	free(P1);
	free(P2);
	free(P3);
	free(P4);
	free(P5);
	free(P6);
	free(P7);
	free(top_left);
	free(top_right);
	free(bottom_left);
	free(bottom_right);
	for(int i = 0; i < 8; i++){
		free(block[i]);
	}

	return c;

}


float timer(int n, int numtrials, int* (*f)(int, int*, int*)){

	double time_elapsed = 0.0;
	int count = 0;
	while (count < numtrials){
		int* m1 = gen_matrix(n);
		int* m2 = gen_matrix(n);

		clock_t begin = clock();
		int* result = f(n, m1, m2);
		clock_t end = clock();

		free(result);
		free(m1);
		free(m2);
		time_elapsed += (double)(end - begin) / CLOCKS_PER_SEC;

		count++;
	}
	float avg_time = time_elapsed / (float) numtrials;
	return avg_time;
}


int* gen_matrix(n){
	int* m = malloc(sizeof(int) * n * n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			*(m + i * n + j) = rand() % 10;
			if(rand() % 10 > 5){
				*(m + i * n + j) *= -1;
			}
		}
	}
	return m;
}

void print_matrix(int rows, int cols, int* m){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			printf("%d ", *(m + i * rows + j));
		}
		printf("\n");
	}
	printf("\n");

}
