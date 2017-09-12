#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include "mt19937-64.c"

/************************* Karmarkar-Karp Variations *************************/
long long kk(long long a[]){
    heap* root = createHeap();
    for(int i = 0; i < N; i++)
        insertHeap(a[i], root);

    while(root->size > 1){
        long long max1 = deleteMax(root);
        long long max2 = deleteMax(root);
        insertHeap(max1 - max2, root); 
    }
    long long residual = deleteMax(root);
    free(root->elements);
    free(root);
    return residual;
}

long long rep_rand_no_part(long long a[], int* s_initial){
    int* s = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){ s[i] = s_initial[i]; }

    for(int i = 0; i < MAX_ITER; i++){
        int* s_ = random_ones();
        if(residue(s_, a) < residue(s, a)){
            free(s);
            s = s_;
        }
        else{
            free(s_);
        }
    }
    long long res = residue(s, a);
    free(s);
    return res;
}

long long hill_climb_no_part(long long a[], int* s_initial){
    int* s = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){ s[i] = s_initial[i]; }

    for(int i = 0; i < MAX_ITER; i++){
        int* s_ = gen_neigh_sign(s);

        if(residue(s_, a) < residue(s, a)){
            free(s);
            s = s_;
        }
        else{
            free(s_);
        }
    }

    long long res = residue(s, a);
    free(s);
    return res;
}

long long annealing_no_part(long long a[], int* s_initial){
    int* s = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){ s[i] = s_initial[i]; }

    int s__[N];
    for(int i = 0; i < N; i++)
        s__[i] = s[i];
    long long best_so_far = residue(s__, a);

    for(int i = 0; i < MAX_ITER; i++){
        int* s_ = gen_neigh_sign(s);
        long long res_s_ = residue(s_, a);
        long long res_s = residue(s, a);
        float threshold = exp(-(res_s_ - res_s) / T(i+1));

        if(res_s_ < res_s || rand01() <= threshold){
            free(s);
            s = s_;
        }
        else{
            free(s_);
        }
        if(residue(s, a) < residue(s__, a)){
            for(int i = 0; i < N; i++)
                s__[i] = s[i];
            best_so_far = residue(s__, a);
           // printf("%llu\n", best_so_far);
        }
    }

    long long res = residue(s__, a);
    //printf("%llu\t%llu\n", res, best_so_far);
    free(s);
    return res;
}

/**************************** Prepartitioning ********************************/
long long rep_rand_part(long long a[], int* p_initial){
    int* p = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){ p[i] = p_initial[i]; }


    for(int i = 0; i < MAX_ITER; i++){
        int* p_ = random_partition();

        // Faster to do this on the stack than allocate memory on the heap
        // by approximately a factor of 4.  Same holds for hill and annealing
        
        long long a1[N];
        long long a2[N];

        combine(a, a1, p);
        combine(a, a2, p_);

        if(kk(a2) < kk(a1)){
            free(p);
            p = p_;
        }
        else{
            free(p_);
        }
    }

    long long a_[N];
    combine(a, a_, p);
    long long res = kk(a_);

    free(p);
    return res;
}

long long hill_climb_part(long long a[], int* p_initial){
    int* p = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){ p[i] = p_initial[i]; }

    for(int i = 0; i < MAX_ITER; i++){
        int* p_ = gen_neigh_part(p);

        long long a1[N];
        long long a2[N];

        combine(a, a1, p);
        combine(a, a2, p_);

        if(kk(a2) < kk(a1)){
            free(p);
            p = p_;
        }
        else{
            free(p_);
        }
    }

    long long a_[N];
    combine(a, a_, p);
    long long res = kk(a_);

    free(p);
    return res;
}

long long annealing_part(long long a[], int* p_initial){
    int* p = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){ p[i] = p_initial[i]; }

    int p__[N];

    for(int i = 0; i < N; i++)
        p__[i] = p[i];
    long long best_so_far = kk(a);

    for(int i = 0; i < MAX_ITER; i++){
        int* p_ = gen_neigh_part(p);

        long long a1[N];
        long long a2[N];

        combine(a, a1, p);
        combine(a, a2, p_);

        long long res1 = kk(a1);
        long long res2 = kk(a2);
        float threshold = exp(-(res2 - res1) / T(i+1));

        if(res2 < res1 || rand01() <= threshold){
            free(p);
            p = p_;
        }
        else{
            free(p_);
        }

        long long a3[N];
        combine(a, a3, p);
 
        long long test = kk(a3);
        if(test < best_so_far){
            for(int i = 0; i < N; i++)
                p__[i] = p[i];
                best_so_far = test;
        }
    }

    long long a_[N];
    combine(a, a_, p__);
    long long res = kk(a_);

    free(p);
    return res;
}

/********************************** Helpers **********************************/

int* gen_neigh_sign(int s[]){
    int* s_ = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){
        s_[i] = s[i];
    }

    int i = rand() % N;
    int j = rand() % N;
    while(i == j){
        i = rand() % N;
        j = rand() % N;
    }
    s_[i] *= -1;
    if(rand01() < 0.5){
        s_[j] *= -1; 
    }

    return s_;
}

long long residue(int s[], long long a[]){
    long long total = 0;
    for(int i = 0; i < N; i++){
        total += a[i] * s[i];
    }
    if(total < 0)
        total *= -1;
    return total;
}


float T(int iter){
    return powf(10, 10) * powf(0.8, floor(iter / 300.0));
}

void combine(long long* a, long long* a_, int p[]){
    for(int i = 0; i < N; i++){ 
        a_[i] = 0;
    }
    for(int i = 0; i < N; i++){
        a_[p[i]] += a[i];
    }
}

int* gen_neigh_part(int p[]){
    int* p_ = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){
        p_[i] = p[i];
    }

    int i = rand() % N;
    int j = rand() % N;
    while(p[i] == j){
        i = rand() % N;
        j = rand() % N;
    }

    p_[i] = j;

    return p_;
}

/************************* Things w/ Random Numbers **************************/
long long gen_rand(void){
  //return a number between 1 and MAX inclusive
    return (genrand64_int63() % MAX) + 1;
}

// Generate a random number on [0,1]
float rand01(){
    return (float)rand() / (float)RAND_MAX;
}

// Generate a random array consisting of +1's and -1's
int* random_ones(){
    int* s = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){
        float v = rand01();
        s[i] = v > 0.5 ? 1: -1;
    }

    return s; 
}

// Generate a random partition
int* random_partition(){
    int* p = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++){
        p[i] = (genrand64_int63() % N);
    }
    return p;
}

/****************************** Heap Management ******************************/
int getLeftChild(int x){
    return 2 * x + 1;
}
int getRightChild(int x){
    return 2 * x + 2;
}
int getParent(int x){
    return (x - 1) / 2;
}

heap* createHeap(){
    heap* new_heap = malloc(sizeof(heap)); 
    new_heap->size = 0;
    new_heap->capacity = 0;
    return new_heap;
}

void insertHeap(long long el, heap* root){
    // If heap is full, double the amount of space it has
    if(root->capacity == root->size && root->capacity != 0){
        root->elements = realloc(root->elements, (root->capacity * 2) * sizeof(long long));
        root->capacity *= 2;
        root->size++;
    }
    else if(root->size == 0){
        root->elements = malloc(sizeof(long long));
        root->size += 1;
        root->capacity += 1;
    }
    else{
        root->size++;
    }

    int i = (root->size) - 1;
    while(i > 0 && el > root->elements[getParent(i)]){
        root->elements[i] = root->elements[getParent(i)]; 
        i = getParent(i);
    }

    root->elements[i] = el;
}

long long deleteMax(heap* root){
    if(root->size > 0){
        long long max_value = root->elements[0];
        root->size--;
        root->elements[0] = root->elements[root->size];
        fix(root, 0);
        
        if(root->size <= root->capacity / 2){
            root->elements = realloc(root->elements, (root->capacity / 2) * sizeof(long long));
            root->capacity /= 2;
        }
        return max_value;
    }
    else{
        free(root->elements);
        return 0;
    }
}

void fix(heap *root, int i){
    int left = getLeftChild(i);
    int right = getRightChild(i);
    int largest = i;


    if(left < root->size && root->elements[left] > root->elements[largest]){
        largest = left;
    }
    if(right < root->size && root->elements[right] > root->elements[largest]){
        largest = right; 
    }

    if(largest != i){
        long long temp = root->elements[i];
        root->elements[i] = root->elements[largest];
        root->elements[largest] = temp;
        fix(root, largest);
    }
}

void print_heap(heap* root){
    for(int i = 0; i < root->size; i++){
        printf("%d\t%d\t%d\t%llu\n", i, root->size, root->capacity, root->elements[i]);
    }
    printf("\n");
}
