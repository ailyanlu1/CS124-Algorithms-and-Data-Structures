#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>


typedef struct tree_node {
	int vertex; 
	float dist; 
	struct tree_node* left;  
	struct tree_node* right;  
} tree_node; 

tree_node* deleteMin(tree_node* root);
tree_node* insert(int vertex, float dist, tree_node* root);

tree_node* insert(int vertex, float dist, tree_node* root){
	if(root == NULL){
		root = malloc(sizeof(tree_node));
		root->vertex = vertex;
		root->dist = dist;
		root->left = NULL;
		root->right = NULL;
	}
	else if(dist < root->dist){
		root->left = insert(vertex, dist, root->left);
	}
	else{
		root->right = insert(vertex, dist, root->right);
	}

	return root;
}

tree_node* deleteMin(tree_node* root){
	tree_node* current = root;
	tree_node* parent = NULL;
	while(current->left != NULL){
		parent = current;
		current = current->left;
	}
	if(current == root){

	}
	else{

	}
}

int main(void){
	tree_node* t = insert(1, 3.0, NULL);
	insert(2,4.0,t);
	insert(1,1,t);

	return 0;
}