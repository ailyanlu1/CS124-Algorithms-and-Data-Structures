#include "kk_helper.h"
#include "kk_helper.c"

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Usage: ./kk inputfile\n");
		return 1;
	}

	// Seed Random Number Generators 
	srand(time(NULL));
	unsigned long long init[4]={rand(), rand(), rand(), rand()}, length=4;
    init_by_array64(init, length);

    // Read from file
    char* inputfile = argv[1];
	FILE* file = fopen(inputfile, "r");
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	if (file == NULL)
		exit(EXIT_FAILURE);
 	
 	int count = 0;
 	long long* a = malloc(sizeof(long long) * N);

	while ((read = getline(&line, &len, file)) != -1) {
		long long value = strtoll(line, NULL, 10);
		*(a + count) = value;
		count++;
	}

	free(line);
	fclose(file);
	
	printf("\nKarmarkar-Karp Residual on Input File = %llu\n\n", kk(a));

	long long (*funcs[6])(long long a[], int* initial) = 
	{
		&rep_rand_no_part, &hill_climb_no_part, &annealing_no_part,
		&rep_rand_part, &hill_climb_part, &annealing_part,
	};
	char* func_names[7] = 
	{	
		"kk", "rep_rand_no_part", "hill_climb_no_part", "annealing_no_part",
		 "rep_rand_part", "hill_climb_part", "annealing_part"
	};

	// Timing
	int trials = 100;
	printf("Trials = %d\n", trials);
	printf("Iterations = %d\n", MAX_ITER);
	for(int i = 0; i < 7; i++){
		float time_elapsed = 0.0;
		double res = 0.0;
		for(int j = 0; j < trials; j++){
			// Generate a new array	
			for(int k = 0; k < N; k++){
				a[k] = gen_rand();
			} 
			int* s_init = random_ones();
			int* p_init = random_partition();
			clock_t start = clock();

			if(i == 0)
				res += kk(a);
			else if(i < 4)
				res += (double) funcs[i-1](a, s_init);
			else
				res += (double) funcs[i-1](a, p_init);

			clock_t end = clock();
			free(s_init);
			free(p_init);
			time_elapsed += (double)(end - start) / CLOCKS_PER_SEC;
			
		}
		printf("%s: Average Residual = %.2f with average time %f\n", 
				func_names[i], res / trials, time_elapsed / trials);
	}

	printf("\n\nEach trial uses the same array and initialization\n\n");
	
	// Print Data in Latex Table Format
	printf("Trial & KK & Rep_Rand_No_Part & Hill_Climb_No_Part & Annealing_No_Part & Rep_Rand_Part & Hill_Climb_Part & Annealing_Part \\\\ \n");
	for(int trial = 0; trial < 100; trial++){
		for(int i = 0; i < N; i++){
			a[i] = gen_rand();
		}
		int* s_init = random_ones();
		int* p_init = random_partition();

		printf("%d & %llu & %llu & %llu & %llu & %llu & %llu & %llu \\\\ \n", trial + 1,
				kk(a),rep_rand_no_part(a, s_init), hill_climb_no_part(a, s_init),
				annealing_no_part(a, s_init), rep_rand_part(a, p_init),
				hill_climb_part(a, p_init), annealing_part(a, p_init));
		free(s_init);
		free(p_init);
	}
	
	free(a);

    return 0; 
}