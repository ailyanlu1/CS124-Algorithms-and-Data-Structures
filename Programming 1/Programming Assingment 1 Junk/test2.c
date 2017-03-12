#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

typedef struct {
	int dim;
	float* xs;
} point;

typedef struct node{
	int vertex; 
	float dist; 
	struct node* next; 
	struct node* prev; 
} node; 


float dist(point p1, point p2);
float MSTfind(int numpoints, float threshold, float* edges[numpoints]);
node* insert(int vertex, float distance, node** tail);
node* deleteMin(node* root, node** tail);
void test(void);
float get_threshold(int numpoints, int dimension);
float largest_edge = 0;
int flag;

int main(int argc, char* argv[]){
	test();
	if(argc != 5){
		printf("Usage: ./randmst flag numpoints numtrials dimension\n");
		return 1;
	}
	// Initialize Constants
	flag = atoi(argv[1]);
	int numpoints = atoi(argv[2]);
	int numtrials = atoi(argv[3]);
	int dimension = atoi(argv[4]);
	
	largest_edge = 0;
	float threshold = get_threshold(numpoints, dimension);
	printf("Threshold: %f\n", threshold);
	// Initialize PRNG
	srand(time(NULL));
	clock_t begin = clock();
	
	float* edges[numpoints];
	point points[numpoints];

	for(int i = 0; i < numpoints; i++){
		edges[i] = malloc(sizeof(float)*(i + 1));
		if(edges[i] == NULL){
			printf("Malloc failed on edges[%d]\n", i);
		}
	}

	int count = 0;
	float total_weight = 0.0;

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
			printf("Generated points\n");
		
		// Calculate Edge Lengths
		for(int i = 0; i < numpoints; i++){
			for(int j = 0; j < i + 1; j++){
				if(i == j){
					edges[i][j] = INT_MAX;
				}
				else {
					if(dimension > 0)
						edges[i][j] = dist(points[i], points[j]);
					else
						edges[i][j] = (float)rand() / (float)RAND_MAX;
				}
			}
		}

		if(flag > 0.0)
			printf("Generated Edges\n");

		// Print Edge Lengths
		if(flag > 2.0){
			for(int i = 0; i < numpoints; i++){
				for(int j = 0; j < numpoints; j++){
					if(i == j){
						printf("      ");
					}
					else {
						printf("%2.2f  ", edges[i][j]);
					}
				}
				printf("\n");
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
		float weight = MSTfind(numpoints, threshold, edges);

		total_weight += weight;

		count += 1;
	}
	for(int i = 0; i < numpoints; i++){
		free(edges[i]);
	}
	clock_t end = clock();
	float time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Avg. weight, Avg. largest edge, numpoints, numtrials, dimension: \n%f, %f, %d, %d, %d\nTime elapsed: %f\n", 
		    (total_weight / numtrials), largest_edge / numtrials, numpoints, numtrials, dimension, time_elapsed);
	//printf("%d, %f\n", numpoints, largest_edge / numtrials);

	return 0;
}

float dist(point p1, point p2){
	int n = p1.dim; 
	float distance = 0.0;
	for(int i = 0; i < n; i++){
		distance += (p1.xs[i] - p2.xs[i]) * (p1.xs[i] - p2.xs[i]); 
	}
	return sqrt(distance);
}

node* insert(int vertex, float distance, node** tail){
	
	node* current = *tail; 
	node* newNode = (node*) malloc(sizeof(node));
	if(newNode == NULL){
		printf("newNode malloc failed\n");
	}

	newNode->vertex = vertex;
	newNode->dist = distance; 
	newNode->prev = current; 
	current->next = newNode; 
	newNode->next = NULL; 

	*tail = newNode;
	return newNode;
	
}

node* deleteMin(node* root, node** tail){

	int min_vertex;
	float min_dist = INT_MAX;
	node* min_node = NULL;

	node* current = root->next;

	while(current != NULL){
		if(current->dist < min_dist){
			min_vertex = current->vertex; 
			min_dist = current->dist; 
			min_node = current;
		}
		current = current->next;
	}

	// min_node is the head
	if(min_node->prev == root){
		if(min_node->next != NULL){
			min_node->next->prev = root;
		}
		root->next = min_node->next;
	}
	else{
		min_node->prev->next = min_node->next;
	}

	// min_node is the tail
	if(min_node->next == NULL){
		min_node->prev->next = NULL; 
		*tail = min_node->prev;
	}
	else{
		min_node->next->prev = min_node->prev;
	}

	return min_node;
}

void print_node(node* n){
	printf("%d, %f\n", n->vertex, n->dist);
}

void print_lst(node* root){
	node* current = root->next;
	while(current != NULL){
		print_node(current);
		current = current->next;
	}
}

void free_lst(node* head){
	node* tmp = head;
	while(head != NULL){
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

float get_threshold(int numpoints, int dimension){
	if(numpoints < 1023){
		if(dimension == 0){
			return 1.0;
		}
		return sqrt(dimension);
	} 
	if(dimension == 0)
		return powf(numpoints, -1.0 / 1.5497);
	else if(dimension == 2)
		return powf(numpoints, -1.0 / 2.7087);
	else if(dimension == 3)
		return powf(numpoints, -1.0 / 3.8381);
	else if(dimension == 4)
		return powf(numpoints, -1.0 / 5.077);
	else
		return INT_MAX;
}

float MSTfind(int numpoints, float threshold, float* edges[numpoints]){
	node* root = malloc(sizeof(node));
	node* tail = root;
	root->next = insert(0,0.0, &tail);

	node* head = root->next;
	node* s = tail;
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
	edges[0][0] = 0;

	int count = 0;
	int size = 1;
	while(root->next != NULL){
		node* min_node = deleteMin(root, &tail);

		size--;

		int v = min_node->vertex;
		V_minus_S[v] = 0;
		for(int w = 0; w < numpoints; w++){
			float tmp_dst = (v < w) ? edges[w][v] : edges[v][w];
			if(V_minus_S[w] && (tmp_dst < dist[w]) && (tmp_dst < threshold)){
				dist[w] = tmp_dst;
				prev[w] = v;
				insert(w, dist[w], &tail);

				size++;
			}
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
				return -1;
			}
		float tmp_dst = (i < prev[i]) ? edges[prev[i]][i] : edges[i][prev[i]];

		tree_weight += tmp_dst;
		if (tmp_dst > largest_edge_used){
			if (prev[i] < 0){
				printf("Vertex %d was not part of the MST\n", i);
			}
			largest_edge_used = tmp_dst;
			largest_index = i;
		}
	}
	free(root);
	
	largest_edge += largest_edge_used;
	printf("Largest edge: %f\n", largest_edge_used);
	return tree_weight;
}



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

	float* edges[9];
	for(int i = 0; i < 9; i++){
		edges[i] = malloc(sizeof(float)*(i + 1));
	}
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < i + 1; j++){
			edges[i][j] = tmp[i][j];
		}
	}
	float threshold = get_threshold(9, 500);
	printf("Test Weight: %f\n", MSTfind(9,threshold,edges));
	for(int i = 0; i < 9; i++){
		free(edges[i]);
	}
}

