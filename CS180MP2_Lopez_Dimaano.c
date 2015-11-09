#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


FILE * input;


typedef struct dictionaryNode dNode;
struct dictionaryNode{
	int index;
	char entry[500];
	struct dictionaryNode * next;
};

typedef struct dictionary Dict;
struct dictionary{
	dNode * top;
};

typedef struct attributeEntryNode entryNode;
struct attributeEntryNode{
	int equivalentValue;
	int count;
	char entry[200];
	struct attributeEntryNode * next;
};

typedef struct entry Entry;
struct entry{
	entryNode * link;
};

typedef struct attributeNode attribNode;
struct attributeNode{
	char attributeName[200];
	Entry * E;
	struct attributeNode * next;
};

typedef struct attribute Attrib;
struct attribute{
	attribNode * link;
	int count;
};



char * mygets(char * str, int num, FILE * stream ){																	//Sir Edge's code for fgets() without the \n thingy (from CS 11)...
	fgets(str, num, stream);
	
	if(str[strlen(str)-1] == 10){																					//If the (n-1)th character is a line feed (\n),
		str[strlen(str)-1] = 0;																						//turn it into a null character.
	}
			
	return str;
}


void initDict(Dict * D){
	D->top = NULL;
}

void initAttrib(Attrib * A){
	A->link = NULL;
}

void initEntries(Attrib * A){
	attribNode * curr;

	curr = A->link;

	while (1){
		curr->E = (Entry *)malloc(sizeof(Entry));
		curr->E->link = NULL;
		if (curr->next == NULL) break;
		curr = curr->next;
	}
}

int assignUniqueID(Dict * D, char * string){	
	dNode * curr, * alpha;
	
	if (D->top == NULL){
		alpha = (dNode *)malloc(sizeof(dNode));
		alpha->index = 0;
		strcpy(alpha->entry, string);
		alpha->next = NULL;
		D->top = alpha;
		return 0;
	}

	else {
		curr = D->top;		
		while (1){
			if (strcmp(curr->entry, string) == 0) return curr->index;	
			if (curr->next == NULL) break;		
			curr = curr->next;
		}
		alpha = (dNode *)malloc(sizeof(dNode));
		alpha->index = curr->index + 1;
		strcpy(alpha->entry, string);
		alpha->next = NULL;
		curr->next = alpha;
		return alpha->index;
	}
}

void assignAttribute(Attrib * A, char * string){
	attribNode * curr, * alpha;

	if (A->link == NULL){
		alpha = (attribNode *) malloc(sizeof(attribNode));
		strcpy(alpha->attributeName, string);
		alpha->next = NULL;
		A->link = alpha;
		A->count = 1;
	}

	else {
		curr = A->link;
		while (curr->next != NULL){
			curr = curr->next;
		}
		alpha = (attribNode *) malloc(sizeof(attribNode));
		strcpy(alpha->attributeName, string);
		alpha->next = NULL;
		curr->next = alpha;
		A->count++;
	}
}

void assignAttributeEntry(Entry * A, char * string, int id){
	entryNode * curr, * alpha;

	if (A->link == NULL){
		alpha = (entryNode *) malloc(sizeof(entryNode));
		strcpy(alpha->entry, string);
		alpha->equivalentValue = id;
		alpha->count = 1;
		alpha->next = NULL;
		A->link = alpha;
	}

	else {
		curr = A->link;
		while (1){
			if (strcmp(curr->entry, string) == 0){
				curr->count++;
				return;
			}	
			if (curr->next == NULL) break;		
			curr = curr->next;
		}

		alpha = (entryNode *) malloc(sizeof(entryNode));
		strcpy(alpha->entry, string);
		alpha->equivalentValue = id;
		alpha->count = 1;
		alpha->next = NULL;
		curr->next = alpha;
	}
}

void printDict(Dict * D){
	dNode * curr = D->top;

	while (1){
		printf("[%d] %s -> ", curr->index, curr->entry);
		if (curr->next == NULL) break;
		curr = curr->next;
	}
}

void printEquivalentTable(int ** array, int attribute){
	int i, j;
	for (i=0; i<50; i++){
		for (j=0; j<attribute; j++){
			printf("%d\t", array[i][j]);
		}
		printf("\n");
	}
}

void printStats(Attrib * A){
	attribNode * curr;
	entryNode  * durr;
	
	curr = A->link;

	do {
		printf("\nAttribute: %s\n", curr->attributeName);

		durr = curr->E->link;
		do {
			printf("%s:\t%d\n", durr->entry, durr->count);
			durr = durr->next;
		} while (durr != NULL);

		curr = curr->next;
	} while (curr != NULL);
}

/*void freeVariables(Dict * D, Attrib * A){
	entryNode * curr, * alpha;
	Entry * durr, * beta;

	attribNode * furr, * gamma;
	Attrib * gurr, * delta;

	gurr = A->link;

	while (1){
		furr = 
		alpha = furr;		
		if (curr == NULL) break;
		curr = curr->next;
		free(alpha);
	}

	curr = A->link;

}*/

void readinput(Dict * D, Attrib * A, int ** equivalentTable){	
	int i=0, j=0, ch;
	char buffer[512];
	char * medium;	

	attribNode * curr;

	input = fopen("input.csv", "r");

	mygets(buffer, 512, input);																						//Get first line: Labels
	medium = strtok(buffer, ",");

	while (medium != NULL){
		assignAttribute(A, medium);
		medium = strtok(NULL, ",");
	}

	initEntries(A);

	equivalentTable = (int **) malloc(sizeof(int *) * 50);
	
	while (!feof(input)){

		equivalentTable[i] = (int *) malloc(sizeof(int) * A->count);																								//Getting actual data now.

		curr = A->link;

		mygets(buffer, 512, input);

		if (strcmp(buffer, "") == 0){																					//If read line is \n (because mygets removes last string and transforms it to just blank) continue with loop.
			fflush(stdin);
			continue;
		}

		medium = strtok(buffer, ",");

		j=0;
		
		while (medium != NULL){
			equivalentTable[i][j] = assignUniqueID(D, medium);
			assignAttributeEntry(curr->E, medium, equivalentTable[i][j]);			
			medium = strtok(NULL, ",");	
			curr = curr->next;
			j++;
		}

		i++;
	}

	printStats(A);
	printf("\n");
	printEquivalentTable(equivalentTable, A->count);
	
	fclose(input);
}


int main(){

	system("cls");

	Dict D;
	initDict(&D);

	Attrib A;
	initAttrib(&A);

	int ** equivalentTable;
	
	readinput(&D, &A, equivalentTable);

	//freeVariables(&D, &A);
	
	return 0;

}