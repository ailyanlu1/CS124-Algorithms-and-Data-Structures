#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include "mt19937-64.c"

int part1(void);
int part2(void);
int N;


int main(int argc, char* argv[]){

    N = 1000000000;
	
	if(argc != 2){
		printf("Usage: ./p7 part\n");
        return 1;
	}

	bool is_part_a;
	if(atoi(argv[1]) == 1)
		is_part_a = true;
	else if(atoi(argv[1]) == 2)
		is_part_a = false;
	else{
		return 1;
    }

	srand(time(NULL));
	
	
	unsigned long long init[4]={rand(), rand(), rand(), rand()}, length=4;
    init_by_array64(init, length);

    int trials = 1;
    float avg_bin = 0.0;
    printf("N = %d\n", N);
    printf("Trials = %d\n", trials);
    for(int j = 0; j < trials; j++){
    	int* counts = calloc(25, sizeof(int));
    	int max_bin = -5;
    	counts[0] = N;

    	for(int i = 0; i < N; i++){

    		int b;
    		if(is_part_a)
    			b = part1();
    		else
    			b = part2();

    		int load = 0;
    		int bins_seen = 0;
    		while(bins_seen < b){
    			bins_seen += counts[load];
    			load++;
    		}
    		counts[load - 1]--;
    		counts[load]++;
    	}

    	for(int i = 0; i < 25; i++){
    		if (counts[i] > 0){
    			printf("%d  %d\n", i, counts[i]);
    			max_bin = i;
    		}
    	}

    	//printf("Max bin: %d\n", max_bin);
		avg_bin += max_bin; 
		free(counts);
    }

	avg_bin /= trials;
	printf("%f\n", avg_bin);

    return 0;
}

int part1(void){
	return genrand64_int64() % N;
}

int part2(void){
	int b1 = genrand64_int64() % N; 
	int b2 = genrand64_int64() % N;

	while(b1 == b2){
		b1 = genrand64_int64() % N;
		b2 = genrand64_int64() % N;
	}

	if(b1 < b2)
		return b1;
	else
		return b2;

}