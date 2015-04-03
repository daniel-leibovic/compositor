#include <stdlib.h>
#include <stdio.h>
#include <string.h>	

struct node {
	char data;
	struct node* next;
	int x;
};

struct row {
	struct node* row;
	struct row* next;
};

//return pointer to the xth node if exists,
//else pointer to node immediately preceding 
//where the ith (xth) node of this row will be
struct node* findNode(int i, struct node* head) {
	//start at first node of row
	struct node* current = head;
	
	for(; current->x < i; current = current->next){ 
		//if at rowTail (last node)
		if(current->next == NULL)
			break; 
		//if x is between two existing nodes, return prior one
		else 						
			if(current->next->x > i)
				return current; 
	}
	
	return current;
}

//if 'space' character, no nodes created (but x value incremented  normally)
//else, creates new node and incorporates into the row
void insertNewNode(struct node** prevRef, char c, int x) {
	
	//don't do anything (skip this x value in the row) if 'space' or newline
	if(c == ' ') 
		return; 
	if(c == '\n')
		return;

	//else, instantiate new node on heap
	struct node* newNode;
	newNode = malloc(sizeof(struct node));
	
	//set the values of this node 
	newNode->data = c;
	newNode->next = (*prevRef)->next;
	newNode->x = x;	
	
	//set the previous node to now point to newNode
	(*prevRef)->next = newNode;
}

//if node on x coordinate of this row exists, update its value
//or do nothing if our character is ' ' or '\n'
void updateNode(struct node** xRef, char c) {
	
	//ORDERING OF THESE IFS MIGHT BE INCORRECT

	if(c == ' ') //TRY THIS AS FIRST 'IF'
		return;

	//special case: the first nodes on each newly created row are initialized
	//to store null character. 
	if((*xRef)->data == '\0') 
		(*xRef)->data = c;

	if(c == '\n')
		return;

	(*xRef)->data = c;
}

//if no x coordinate node exists on this line, create it;
//if exists, then update existing node
//insertNewNode and updateNode functions deal with 
//prioritization and completion of characters (no spaces, newlines)
void placeChar(char c, struct row* rowHead, int x){

	//get to the first node of row
	struct node* nodeHead;
	nodeHead = rowHead->row;

	//creates pointer to the node immediately preceding where 
	//the x coordinate node is or would be
	struct node* xRef;
	xRef = findNode(x, nodeHead); 	
	
	//if xth node DNE, insert xth node in this row	
	if(xRef->x < x){ 										
		
		insertNewNode(&xRef, c, x); 
	}        

	else 
		updateNode(&xRef, c);
	
}


int printPic(struct row* origin){

	struct row* rowHead;
	rowHead = origin;

	struct node* currentNode;
	

	while(1){

		//sets currentNode to nodeHead of row
		currentNode = rowHead->row;

		//print each node on row, including the spaces that follow it
		//if there exists a next node.
		//if we are on last node, go to next row
		for(;1; currentNode = currentNode->next){

			char c;
			c = currentNode->data;

			//when on last node of a row
			if(currentNode->next == NULL){

				//if this is last row, done. 
				if(rowHead->next->next == NULL){ 
					
					if(c == '\0')
						printf("\n");
					else
						printf("%c\n", currentNode->data);							
					// printf("%c(%d)", currentNode->data, currentNode->x);
					
					return 0;
				}

				//else, print new line and go to next node (the linked list 
				//isn't printing out the stored new lines, so I do manually)
				//and move to print the next row
				else{ 
					if(c == '\0')
						printf("\n");
					else
						printf("%c\n", currentNode->data);
					// printf("%c(%d)\n", currentNode->data, currentNode->x);

					rowHead = rowHead -> next;
					break;
				}
			}

			//else, on a non-end node, print data, print spaces until next 
			//x coordinate node on row
			//THIS IS WHY we don't want to store newlines in linked list
			else{
				
				if(c ==	'\0'){}
				else
					printf("%c", currentNode->data);
				// printf("%c(%d)", currentNode->data, currentNode->x);
				
				//calculate spaces required between this node and next node
				int missingSpaces;
				missingSpaces = (currentNode->next->x - currentNode->x -1);

				for(int i = 0; i < missingSpaces; i++) {
					printf(" ");
				}
			}
		}
	}
}


void destroyNodes(struct node *n){
	if(n->next == NULL){
		free(n);
		return;
	}

	struct node* temp;

	temp = n->next;
	free(n);

	destroyNodes(temp);
}

void destroyLists(struct row* n){
	destroyNodes(n->row);
	if(n->next != NULL){
		destroyLists(n->next);
		free(n);
	}
	else
		free(n);
	 
}

int main(int argc, char **argv){

	
	#define BUFFER_SIZE (2048)

	FILE *file;
	char c;
	int row;
	int col;
	char filename[BUFFER_SIZE];

	//initialize first node of the linked lists
	struct node* firstNode;
	firstNode = malloc(sizeof(struct node));
	firstNode->next = NULL;
	firstNode->x = -1; 
	firstNode->data = '\0';

	//initialize first rowHead
	struct row* origin;
	origin = malloc(sizeof(struct row));
	origin->row = firstNode;
	origin->next = NULL;


	//take in each 3-pair argument, store those args as row, col, and filename
	while(scanf("%d %d %s", &row, &col, filename) == 3) {
		
		//x-coordinate of our node on our row
		int x;

		//copy origin to rowHead so as not to alter its true value
		struct row* rowHead;
		rowHead = origin;
		
		//each 3-pair argument has an offset argument of two dimensions
		//this for loop takes care of vertical offset (moving the rows down)
		for(int i = 0; i < row; i++) { 
				// printf("inserting vertical space");

				//THIS MAY BE A SOURCE OF PORBLEMS
				//placeChar usually ignores newline, but not when its x 
				//coord is 0
				placeChar('\n', rowHead, 0); 
				
				//if we need more rows, to store our offset, create them
				if(rowHead->next == NULL){ 

					//every row starts with this dummy
					struct node* newNode; 
					newNode = malloc(sizeof(struct node));
					newNode->data = '\0'; 
					newNode->next = NULL;
					newNode->x = -1;

					struct row* next;
					next = malloc(sizeof(struct row));
					next->row = newNode; 
					next->next = NULL;

					rowHead->next = next; 					
				}

				rowHead = rowHead->next;
			}

		//now that we have produced vertical offset, start reading the 
		//file to the lists

		file = fopen(filename, "r");

		//start the x co
		for(x = col; fscanf(file, "%c", &c) != EOF; x++){ 
			
			//if we read a newline, we have to start writing in the following
			//row
			if(c == '\n'){ 	

				//if that row DNE, make it
				if(rowHead->next == NULL){ 
					struct node* newNode; 
					newNode = malloc(sizeof(struct node));
					newNode->data = '\0'; 
					newNode->next = NULL;
					newNode->x = -1;

					struct row* next;
					next = malloc(sizeof(struct row));
					next->row = newNode; 
					next->next = NULL;
					
					rowHead->next = next; 	
				}
			
				//go to new row, reset the horizontal offset x (which will increment immediately)
				rowHead = rowHead->next;
				x = (col - 1); 
				continue;
				
			}

			//for non-last node, place it in the row
			else
				placeChar(c, rowHead, x);			
		}

		fclose(file);
		
		// printPic(origin);
		// destroyLists(origin);
	}

	printPic(origin);
	destroyLists(origin);

	return 0;
}





