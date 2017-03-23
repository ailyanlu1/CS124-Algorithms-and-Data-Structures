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

typedef struct matrix {
	int rows; 
	int cols;
	int** data;
} matrix;  

void print_matrix(matrix* m);


int main(int argc, char* argv[]){
	if(argc != 4){
		printf("Usage: ./strassen_with_structs flag dimension inputfile\n");
		return 1;
	}
	int flag = atoi(argv[1]);
	int dim = atoi(argv[2]);
	char* inputfile = argv[3];
	matrix* a = malloc(sizeof(matrix));
	matrix* b = malloc(sizeof(matrix));
	a->rows = dim;
	a->cols = dim;
	b->rows = dim;
	b->cols = dim;
	printf("got here\n\n");
	for(int i = 0; i < dim; i++){
		printf("got here");
		a->data = malloc(sizeof(int*) * dim);
		b->data = malloc(sizeof(int*) * dim);
		for(int j = 0; j < dim; j++){
			a->data[i] = malloc()
		}
	}
	srand(time(NULL));
	FILE* file = fopen(inputfile, "r");
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
 
	if (file == NULL)
		exit(EXIT_FAILURE);
 	
 	int i = 0;
 	int j = 0;
 	int count = 0;
 	int* data1;
 	matrix* current_matrix = a;
	while ((read = getline(&line, &len, file)) != -1) {
		int value = atoi(line);
		printf("count, i, j = %d, %d, %d", count, i,j);
		if(count < dim * dim){
			a->data[i][j] = value;
		}
		else{
			b->data[i][j] = value;
		}
		j++;
		if(j % dim == 0){
			i++;
			j = 0;
		}
		if(i == dim){
			i = i % dim;
		}
		count++;
		
		/*
		if((i % dim) == 0 && (j % dim) == 0){
			if(count == 1){
				printf("got here\n");
				current_matrix = b;
			}
			for(int k = 0; k < dim; k++){
				int* tmp = malloc(sizeof(int) * dim);
				current_matrix->data[k] = tmp;
				if(k == 1)
					data1 = tmp;
			}
			count++;
		}
		if(current_matrix == b)
			current_matrix->data[1] = data1;
		printf("data[%d][%d] = %d; matrix = %p\n", i, j, value, current_matrix);
		// printf("data[0] = %p\n", current_matrix->data[0]);
		// printf("data[1] = %p\n", current_matrix->data[1]);
		// printf("data[2] = %p\n", current_matrix->data[2]);
		current_matrix->data[i][j] = value;
		j++;
		if(j % dim == 0){
			i++;
			j = 0;
		}
		if(i == dim){
			i = i % dim;
		}
		*/
	}
	printf("made it\n");
	print_matrix(a);
	//print_matrix(b);

	return 0;
}

void print_matrix(matrix* m){
	int rows = m->rows;
	int cols = m->cols;
	printf("rows: %d\ncols: %d\n", rows, cols);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			printf("%d ", m->data[i][j]);
		}
		printf("\n");
	}
}



		