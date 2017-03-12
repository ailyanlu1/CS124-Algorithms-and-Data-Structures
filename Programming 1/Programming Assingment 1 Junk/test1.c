/*
 * Matt Leifer
 * February 26, 2017
 * Programming Assignment 1
 * CS124
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

typedef struct {
	int dim;
	float* xs;
} point;

typedef struct node {
	int vertex; 
	float dist; 
	struct node* next;  
} node; 


// Queue will manage the (ordered) priority queue in Prim's Algorithm
node*  insertQueue(int vertex, float distance, node* root);
node* deleteMinQueue(node* root);
node* createQueue();
// Edges will be stored as (unordered) adjacency lists
node*  insertLinkedList(int vertex, float distance, node* root);
node* createLinkedList();
node* ListFind(int vertex, node* root);

// List and Queue Debugging
void print_node(node* n);
void print_lst(node* root);
void free_lst(node* head);
void ctr(void);
int COUNT = 0;

// MST Calculation
float MSTfind(int numpoints, node* edges[numpoints]);
float get_threshold(int numpoints, int dimension);
float dist(point p1, point p2);
void test(void);
float largest_edge = 0;
int flag;
int numtrials;
int failures; 

// Heap Management
typedef struct heap_node{
	int vertex;
	float dist; 
} heap_node;

typedef struct heap {
	int size;
	int capacity;
	heap_node** elements;
} heap;

int getLeftChild(int x);
int getRightChild(int x);
int getParent(int x);
heap* createHeap();
void insertHeap(int vertex, float distance, heap* root);
heap_node* deleteHeapNode(heap* root);
void fix(heap *root, int i);
void print_heap(heap* root);

int main(int argc, char* argv[]){
	if(argc != 5){
		printf("Usage: ./randmst flag numpoints numtrials dimension\n");
		return 1;
	}
	// Initialize Constants
	flag = atoi(argv[1]);
	int numpoints = atoi(argv[2]);
	numtrials = atoi(argv[3]);
	int dimension = atoi(argv[4]);

	if(dimension == 1 || dimension < 0 ){
		printf("Dimension cannot equal %d\n", dimension);
		return 1;
	}

//	while(numpoints < 131072 * 2){
//	if(numpoints > 4096){
//		numtrials = 5;
//	}
	if(flag > 1.0)
		test();
	
	largest_edge = 0;
	float threshold = get_threshold(numpoints, dimension);

	if(flag > 0)
		printf("Threshold: %f\n", threshold);

	srand(time(NULL));
	clock_t begin = clock();

	// Edges is an array of adjacency lists
	node* edges[numpoints];
	point points[numpoints];
	for(int i = 0; i < numpoints; i++){
		edges[i] = createLinkedList();
		if(edges[i] == NULL){
			printf("Malloc failed on edges[%d]\n", i);
		}
	}

	int count = 0;
	float total_weight = 0.0;
	failures = 0;
	while(count < numtrials){
		// Generate numpoints random points
		if(dimension > 0.0){ 
			for(int i = 0; i < numpoints; i++){

				float* coordinates = malloc(sizeof(float)*dimension);
				
				if(coordinates == NULL){ 
					printf("Coordinate array could not be allocatd. i = %d\n", i);
				}

				for(int j = 0; j < dimension; j++){
					coordinates[j] = (float)rand() / (float)RAND_MAX;
				}
				points[i].dim = dimension;
				points[i].xs = coordinates;	
			}
		}

		if(flag > 0)
			printf("Generated Points\n");

		for(int i = 0; i < numpoints; i++){
			for(int j = 0; j < numpoints; j++){
				float tmp_dst = 0.0;
				node* tmp_node;
				if(i == j){
					tmp_dst = INT_MAX;
				}
				else if (dimension > 0){
					tmp_dst = dist(points[i], points[j]);
				}
				else if (i < j){
					tmp_dst = (float)rand() / (float)RAND_MAX;
				}
				else{
					tmp_dst = INT_MAX;
				}

				if(tmp_dst < threshold){
					insertLinkedList(j, tmp_dst, edges[i]);
					insertLinkedList(i, tmp_dst, edges[j]);
				}

			}
		}

		if(flag > 0.0)
			printf("Generated Edges\n");
		
		// Print Edge Lengths
		if(flag > 2.0){
			for(int i = 0; i < numpoints; i++){
				printf("Edges from %d: ", i);
				print_lst(edges[i]);
			}
		}

		// Free points
		if(dimension > 0){
			for(int i = 0; i < numpoints; i++){
				free(points[i].xs);
			}
		}

		if(flag > 0.0)
			printf("Freed Points\n");
		if(flag > 0.0)
			printf("Calculating MST...\n");
		float weight = MSTfind(numpoints, edges);

		// In case an MST could not be found
		if(weight > 0){
			total_weight += weight;
			//printf("%f, %d, %d\n", weight, numpoints, dimension);
		}

		for(int i = 0; i < numpoints; i++){
			while(edges[i]->next != NULL){
				free(deleteMinQueue(edges[i]));
			}
		}

		count += 1;
	}
	for(int i = 0; i < numpoints; i++){
		free(edges[i]);
	}
	clock_t end = clock();
	float time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Avg. weight, Avg. largest edge, numpoints, numtrials, dimension: \n%f, %f, %d, %d, %d\nTime elapsed: %f\n", 
		    (total_weight / (numtrials - failures)), largest_edge / (numtrials - failures), numpoints, numtrials - failures, dimension, time_elapsed);
	// Numpoints (an int may cause overflow when multiplied by itself - 1)

//	printf("%f %d %d %d\n", (total_weight / (numtrials - failures)), numpoints, numtrials - failures, dimension);
//	numpoints *= 2;
//}
	return 0;

}
/******************************* MST Management ******************************/
float dist(point p1, point p2){
	int n = p1.dim; 
	float distance = 0.0;
	for(int i = 0; i < n; i++){
		distance += (p1.xs[i] - p2.xs[i]) * (p1.xs[i] - p2.xs[i]); 
	}
	return sqrt(distance);
}

float get_threshold(int numpoints, int dimension){
	if(numpoints < 2048){
		if(dimension == 0){
			return 1.0;
		}
		return sqrt(dimension);
	}  
	if(dimension == 0)
		return powf(numpoints, -1.0 / 1.6614);
	else if(dimension == 2)
		return powf(numpoints, -1.0 / 2.6197);
	else if(dimension == 3)
		return powf(numpoints, -1.0 / 3.8692);
	else if(dimension == 4)
		return powf(numpoints, -1.0 / 5.4464);
	else
		return INT_MAX;
}

float MSTfind(int numpoints, node* edges[numpoints]){
	heap* root = createHeap();//createQueue();
	insertHeap(0,0.0,root);

	float dist[numpoints];
	int prev[numpoints];
	uint8_t V_minus_S[numpoints];

	// Initialize arrays.  
	for(int i = 0; i < numpoints; i++){
		dist[i] = INT_MAX;
		prev[i] = INT_MIN;
		V_minus_S[i] = 1;
	}
	prev[0] = 0;
	dist[0] = 0;
	insertLinkedList(0,0.0,edges[0]);

	int count = 0;
	int size = 1;
	while(root->size > 0){
		heap_node* min_node = deleteHeapNode(root);
		size--;
		
		int v = min_node->vertex;
		V_minus_S[v] = 0;
		node* current = edges[v]->next;

		while(current != NULL){
			int w = current->vertex;
			if(V_minus_S[w] && (current->dist < dist[w])){
				dist[w] = current->dist;
				prev[w] = v;
				insertHeap(w, dist[w], root);
				size++;
			}
			current = current -> next;
		}
		count++;
		if(flag > 1.0){
			if(count % 1000 == 0)
				printf("%d, %d\n", count, size);
		}
		free(min_node);
	}

	float tree_weight = 0.0;
	float largest_edge_used = 0.0;
	int largest_index = -3;
	for(int i = 0; i < numpoints; i++){
		if (prev[i] < 0){
			printf("Vertex %d was not part of the MST\n", i);
			free(root);
			failures++;
			return -1;
		}

		node* temp = ListFind(prev[i], edges[i]);
		float tmp_dst = temp->dist;

		tree_weight += tmp_dst;

		if (tmp_dst > largest_edge_used){
			largest_edge_used = tmp_dst;
			largest_index = i;
		}
	}
	free(root);
	
	largest_edge += largest_edge_used;
	if (flag > 0)
		printf("Largest edge: %f\n", largest_edge_used);
	return tree_weight;
}

/************************* List and Queue Debugging **************************/
void print_node(node* n){
	if(n == NULL)
		printf("This node was NULL\n");
	else
		printf("%d, %f\n", n->vertex, n->dist);
}

void print_lst(node* root){
	node* current = root->next;
	while(current != NULL){
		print_node(current);
		current = current->next;
	}
	printf("\n");
	return;
}

void free_lst(node* head){
	node* tmp = head;
	while(head != NULL){
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

void ctr(void){
	COUNT++;
	printf("%d ", COUNT);
}

/***************************** Queue Management ******************************/
node* insertQueue(int vertex, float distance, node* root){	
	node* current = root;
	node* new_node = malloc(sizeof(node));

	if(new_node == NULL){
		printf("Malloc on insertion failed\n");
		return NULL;
	}

	new_node->vertex = vertex;
	new_node->dist = distance;

	while(current->next != NULL && current->next->dist <= distance){
		current = current->next;
	}
	new_node->next = current->next;
	current->next = new_node;
	return new_node;	
}

node* createQueue(){
	node* root = malloc(sizeof(node));
	if(root == NULL){
		printf("Queue couldn't be created\n");
		return root;
	}
	root->next = NULL;
	root->dist = INT_MIN;
	return root;
}

node* deleteMinQueue(node* root){
	if(root->next == NULL){
		printf("Queue was Empty!\n");
		return NULL;
	}
	else{
		node* minNode = root->next;
		root->next = minNode->next;
		return minNode;
	}
}

/************************* Adjacency List Management *************************/
node* createLinkedList(){
	return createQueue();
}

node* insertLinkedList(int vertex, float distance, node* root){
	node* new_node = malloc(sizeof(node));
	if(new_node == NULL){
		printf("Malloc on insertion failed\n");
		return NULL;
	}
	new_node->vertex = vertex;
	new_node->dist = distance;

	new_node->next = root->next;
	root->next = new_node;
	return new_node;
}

node* ListFind(int vertex, node* root){
	node* current = root->next;
	while(current != NULL && current->vertex != vertex){
		current = current->next;
	}
	if(current == NULL){
		//printf("Vertex %d was not in the list.\n", vertex);
		return NULL;
	}
	return current;
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

void insertHeap(int vertex, float distance, heap* root){
	// If heap is full, double the amount of space it has
	if(root->capacity == root->size && root->capacity != 0){
		root->elements = realloc(root->elements, (root->capacity * 2) * sizeof(heap_node));
		root->capacity *= 2;
		root->size += 1;
	}
	else if(root->size == 0){
		root->elements = malloc(sizeof(heap_node));
		root->size += 1;
		root->capacity += 1;
	}
	else{
		root->size++;
	}

	heap_node* new_node = malloc(sizeof(heap_node));
	new_node->vertex = vertex;
	new_node->dist = distance;

	int i = (root->size) - 1;
	while(i > 0 && new_node->dist < root->elements[getParent(i)]->dist){
		root->elements[i] = root->elements[getParent(i)]; 
		i = getParent(i);
	}

	root->elements[i] = new_node;
}


heap_node* deleteHeapNode(heap* root){
	if(root->size > 0){
		heap_node* min_node = root->elements[0];
		root->size--;
		root->elements[0] = root->elements[root->size];
		fix(root, 0);
		return min_node;
	}
	else{
		free(root->elements);
		return NULL;
	}
}

void fix(heap *root, int i){
	int left = getLeftChild(i);
	int right = getRightChild(i);
	int smallest = i;


	if(left < root->size && root->elements[left]->dist < root->elements[smallest]->dist){
		smallest = left;
	}
	if(right < root->size && root->elements[right]->dist < root->elements[smallest]->dist){
		smallest = right; 
	}


	if(smallest != i){
		heap_node* temp = root->elements[i];
		root->elements[i] = root->elements[smallest];
		root->elements[smallest] = temp;
		fix(root, smallest);
	}
}

void print_heap(heap* root){
	for(int i = 0; i < root->size; i++){
		//	printf("%d, %p\n", i, root->elements[i]);
		printf("%d, %p, %d, %d, %d, %f\n", i, root->elements[i], root->size, root->capacity, root->elements[i]->vertex, root->elements[i]->dist);
	}
	printf("\n");
}

/*********************************** Test ************************************/
void test(void){
	float tmp[9][9] = {
		{0,1,INT_MAX,3,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
		{1,INT_MAX,5,INT_MAX,5,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
		{INT_MAX,5,INT_MAX,INT_MAX,INT_MAX,2,INT_MAX,INT_MAX,INT_MAX},
		{3,INT_MAX,INT_MAX,INT_MAX,4,INT_MAX,2,INT_MAX,INT_MAX},
		{INT_MAX,5,INT_MAX,4,INT_MAX,1,INT_MAX,5,INT_MAX},
		{INT_MAX,INT_MAX,2,INT_MAX,1,INT_MAX,INT_MAX,INT_MAX,7},
		{INT_MAX,INT_MAX,INT_MAX,2,INT_MAX,INT_MAX,INT_MAX,3,INT_MAX},
		{INT_MAX,INT_MAX,INT_MAX,INT_MAX,5,INT_MAX,3,INT_MAX,6},
		{INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,7,INT_MAX,6,INT_MAX}
	};

	node* edges[9];
	for(int i = 0; i < 9; i++){
		edges[i] = createLinkedList();
	}
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(tmp[i][j]<200){
				insertLinkedList(i, tmp[i][j], edges[j]);
			}
		}
	}

	printf("Test Weight: %f\n", MSTfind(9,edges));
	for(int i = 0; i < 9; i++){
		free_lst(edges[i]);
	}
}