#define MAX 1000000000000ULL
#define N 100
#define MAX_ITER 25000

// Heap Management
typedef struct heap {
    int size;
    int capacity;
    long long* elements;
} heap;

int getLeftChild(int x);
int getRightChild(int x);
int getParent(int x);
heap* createHeap();
void insertHeap(long long el, heap* root);
long long deleteMax(heap* root);
void fix(heap *root, int i);
void print_heap(heap* root);

// Random Things
float rand01();
int* random_ones();
int* random_partition();
long long gen_rand(void);


// Partition management
long long kk(long long a[]);
long long rep_rand_no_part(long long a[], int* s_initial);
long long hill_climb_no_part(long long a[], int* s_initial);
long long annealing_no_part(long long a[], int* s_initial);
long long residue(int s[], long long a[]);
int* gen_neigh_sign(int s[]);
float T(int iter);

void combine(long long* a, long long* a_, int p[]);
long long rep_rand_part(long long a[], int* p_initial);
long long hill_climb_part(long long a[], int* p_initial);
long long annealing_part(long long a[], int* p_initial);
int* gen_neigh_part(int p[]);
