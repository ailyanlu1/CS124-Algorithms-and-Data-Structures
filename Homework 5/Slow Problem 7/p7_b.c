#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include "mt19937-64.c"

int main(void){

	int N = 1000000000;
	int B = 1000000000;

	srand(time(NULL));
	

	unsigned long long init[4]={rand(), rand(), rand(), rand()}, length=4;
    init_by_array64(init, length);

    float avg_bin = 0.0;

    int trials = 100;
    printf("N = %d\n", N);
    for(int j = 0; j < trials; j++){
    	uint8_t* bins = calloc(N, sizeof(uint8_t));
    	int* counts = calloc(50, sizeof(int));

		for(int i = 0; i < N; i++){
			int b1 = genrand64_int64() % N; 
			int b2 = genrand64_int64() % N;
			while(b1 == b2){
				b1 = genrand64_int64() % N;
				b2 = genrand64_int64() % N;
			}
			
			if(bins[b1] < bins[b2]){
				bins[b1]++;
			}
			else{
				bins[b2]++;
			}

			if(i % 100000000 == 0){
			 	//printf("%d\n", i);
			}
		}

		
		for(int i = 0; i < N; i++){
			counts[bins[i]]++;
		}

		int max_bin = -5;
		for(int j = 0; j < 50; j++){
			if(counts[j] > 0){
				max_bin = j;
				//printf("%d, %d\n", j, counts[j]);
			}
		}
		//printf("Max bin: %d\n", max_bin);
		avg_bin += max_bin;
		free(bins);
		free(counts);
	}

	avg_bin /= trials; 
	printf("avg_bin = %f\n", avg_bin);

	return 0;
}