#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>



typedef struct heap_node{
	int vertex;
	float dist; 
} heap_node;

typedef struct heap {
	int size;
	int capacity;
	heap_node** elements;
} heap;


void fix(heap *root, int i);

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

int main(void){
	heap* my_heap = createHeap();

	insertHeap(0, 0.0, my_heap);
	insertHeap(1, 4.0, my_heap);
	insertHeap(2, 2.0, my_heap);	
	insertHeap(3, 3.0, my_heap);
	insertHeap(4, 5.0, my_heap);
	insertHeap(8, 12.0, my_heap);
	insertHeap(7, 16.0, my_heap);
	print_heap(my_heap);
	

	heap_node* min = malloc(sizeof(heap_node));
	while(my_heap->size > 0){
		min = deleteHeapNode(my_heap);
		printf("%d, %f\n", min->vertex, min->dist);
	}
	//print_heap(my_heap);

	return 0;

}