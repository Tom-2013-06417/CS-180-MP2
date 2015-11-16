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

char * returnEquivalent(Dict * D, int index){

	dNode * curr = D->top;

	while (curr->index != index){
		curr = curr->next;
	}

	return curr->entry;
}

//DIEGO MADE NEW FUNCTIONS + CHANGES BETWEEN THIS COMMENT...
//NEW FUNCTION!!! prints an entire line of entries
int * initConsiderArray(int numOfEntries){
	int i;
	int * array = (int *) malloc(sizeof(int) * numOfEntries + 1);
	array[0] = numOfEntries;
	
	for(i=1; i<=numOfEntries; i++){
		array[i] = 1;
	}

	return array;
}


//NEW FUNCTION!!! prints an entire line of entries
void printEntryLine(int * array, int attrCount){
	int j;
	for (j=0; j<attrCount; j++){
		printf("%d\t", array[j]);
	}
}

//MODIFIED FUNCTION!!! slightly streamlined using the previous function
void printEquivalentTable(int ** array, int attrCount){
	int i, j;
	for (i=0; i<50; i++){
		printEntryLine(array[i], attrCount);
		printf("\n");
	}
}

//NEW FUNCTION!!! return a certain line of entries as an array
int * getEntryLine(int ** array, int attrCount, int lineNo){
	int * x = (int *) malloc(sizeof(int) * attrCount);
	int j;

	for(j=0; j<attrCount; j++){
		x[j] = array[lineNo][j];
	}

	printEntryLine(x, attrCount);

	return x;
}

//NEW FUNCTION!!! simply traverse through the attributes array
void traverseAttr(Attrib A){
	attribNode * curr = A.link;
	entryNode * currE;

	if (curr == NULL) printf("Empty Attribute List\n");
	else {
		while(curr != NULL){
			printf("%s\n", curr->attributeName);
			
			currE = curr->E->link->next;
			if(currE == NULL) printf("Attribute Does Not Hold Values\n");
			else{
				while(currE != NULL){
					printf("\t%s\n", currE->entry);
					currE = currE->next;
				}
			}

			printf("\n\n");
			curr = curr->next;	
		}
	}
}

//NEW FUNCTION!!! simply traverse through the (integer) attributes array
void traverseAttrArray(Dict * D, int ** attrArray, int attrCount){
	int i, j;

	for (i = 0; i < attrCount; i++){
		printf("ATTRIBUTE: %s\n", returnEquivalent(D, i));
		for(j = 1; j <= attrArray[i][0]; j++) printf("\tOPTION: (%d:%d) %s\n", j, attrArray[i][j], returnEquivalent(D, attrArray[i][j]));
		printf("\n");
	}
}


//NEW FUNCTION!!! return a 2D array of Attributes and the corresponding Attribute Options
//based off traverseAttr()
int ** getAttrArray(Attrib A){
	int ** array;
	attribNode * curr = A.link;
	entryNode * currE;
	int attrOptionsCtr;
	int i, index;


	//initialize the return array
	array = (int **) malloc(sizeof(int *) * A.count);

	//verify attribute list non-emptiness
	if (curr == NULL) printf("Empty Attribute List\n");
	else {
		//traverse through non-empty attribute list
		while(curr != NULL){
			printf("%s\n", curr->attributeName);
			
			//access the attribute option list
			currE = curr->E->link->next;
			attrOptionsCtr = 0;			
			
			//verify attribute option list non-emptiness
			if(currE == NULL) printf("Attribute Does Not Hold Values\n");
			else{

				//traverse through attribute option list to count number of options
				while(currE != NULL){
					printf("\t[%d] %s\n", currE->equivalentValue, currE->entry);
					currE = currE->next;
					attrOptionsCtr++;
				}

				//get current attribute's equivalent value and simplify shit
				index = curr->E->link->equivalentValue;
				
				//initialize inner array
				array[index] = (int *) malloc((sizeof(int) * attrOptionsCtr) + 1);

				//set inner array's first element to be the number of options for the attribute
				array[index][0] = attrOptionsCtr;

				//reset the currE pointer
				currE = curr->E->link->next;

				//copy the linked list's data unto the array; exiting the program if shit happens
				for(i = 1; i <= attrOptionsCtr; i++){
					if(currE == NULL){
						printf("Unbound shit and shit 1\n");
						exit(0);
					}

					array[index][i] = currE->equivalentValue;
					//printf("| %d |", array[index][i]);
					currE = currE -> next;
				}

				printf("\n%d\n", array[index][0]);

				if(currE != NULL){
					printf("Unbound shit and shit 2\n");
					exit(0);
				}
			}

			printf("\n\n");
			curr = curr->next;	
		}
	}

	return array;
}


//log base 2 function
double logx(double x, int base){
	return (double) (log(x) / log(base));
}

//calculate total number of unique attribute types
int calculateAttrTotal(int * x){
	int i, total = 0;

	for(i = 1; i <= x[0]; i++) total = total + x[i];
	return total;
}

//calculate for entropy
double calculateEntropy(int * x){
	int i;
	double total = (double) (calculateAttrTotal(x));
	double entropy = 0;

	for(i = 1; i <= x[0]; i++) {
		printf("%lf\n", entropy);
		if (x[i] != 0) entropy = entropy - ((x[i]/total) * logx(x[i]/total, x[0]));
	}
	return entropy;
}

//NEW FUNCTION!!! gets the distribution of the entries given a certain attribute and the set of entries to consider
/*
targetAttrArray: size of array [index 0] and array of possible target attribute options [index 1 -> size]
set: 2d array of entries for which the distribution will be identified
setSize: size of array of entries for which the distribution will be identified
attrCount: number of attributes in an entry

idea: pigeonhole shit
	use for loop to iterate through all entries

	if last item on entry == targetAttrArray[n] -> pigeon hole to targetAttrArray n
		check all targetAttrArray vals
		use another for loop to iterate for all targetAttrArray

*/

int * getDistribution(int * targetAttrArray, int attrCount, int ** set, int setSize, int * considerArray){
	//printf("%d\n", attrCount - 1);

	int i, j;
	int * x = (int *) malloc(sizeof(int) * attrCount);
	for(i = 0; i < attrCount; i++) x[i] = 0;

	x[0] = targetAttrArray[0];
	for(j = 1; j < setSize; j++){
		//printf("%d\n", set[j][attrCount-1]);
		if(considerArray[j] == 1){
			for(i = 0; i <= targetAttrArray[0]; i++) {
				if(targetAttrArray[i] == set[j][attrCount-1]){
			 		x[i]++;
			 		break;
			 	}
			}
		}
	}

	//printf("\n[");
	for(i = 1; i <= x[0]; i++) printf(" %d ", x[i]);
	//printf("]\n");

	return x;
}

// //calculate for information gain
// double calculateGain(int ** attrArray, int attrCount, int ** set, int setSize, int * availableAttr, int classifierAttr, int considerArray){
// 	int i;
// 	int * initialDist = getDistribution(attrArray[attrCount - 1], attrCount, set);
// }

//...AND THIS COMMENT :)

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

void shuffleArray(int ** array){
	
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

int ** readinput(Dict * D, Attrib * A){	
	int i=0, j=0;
	int ** equivalentTable;
	char buffer[512];
	char * medium;	

	attribNode * curr;

	equivalentTable = (int **) malloc(sizeof(int *) * 51);

	input = fopen("input.csv", "r");

	mygets(buffer, 512, input);																						//Get first line: Labels
	medium = strtok(buffer, ",");

	while (medium != NULL){		
		assignAttribute(A, medium);
		medium = strtok(NULL, ",");
	}

	rewind(input);

	while (medium != NULL){
		equivalentTable[i] = (int *) malloc(sizeof(int) * A->count);
		equivalentTable[i][j] = assignUniqueID(D, medium);
		medium = strtok(NULL, ",");
		j++;
	}

	initEntries(A);	
	
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
	printEquivalentTable(equivalentTable, A->count);
	
	fclose(input);

	return equivalentTable;
}


int main(){

	system("cls");

	Dict D;
	initDict(&D);

	Attrib A;
	initAttrib(&A);

	int ** initialSet;
	int ** attrArray;
	int * considerArray;
	int * dist;
	int i;
	
	//file read and initialization
	initialSet = readinput(&D, &A);
	printf("\n\n%s\n\n", returnEquivalent(&D, 9));
	
	//start of ID3 shit supposedly
	system("cls");
	attrArray = getAttrArray(A);
	system("cls");
	traverseAttrArray(&D, attrArray, A.count);
	system("cls");
	//printf("\n\n%s\n\n", returnEquivalent(&D, A.count - 1));

	considerArray = initConsiderArray(51);
	dist = getDistribution(attrArray[A.count - 1], A.count, initialSet, 51, considerArray);

	system("cls");
	printf("DISTRIBUTION: \n");	
	for (i = 1; i <= dist[0]; i++){
		printf("\t%d >> %s\n", dist[i], returnEquivalent(&D, attrArray[A.count - 1][i]));
	}

	// //get entropy
	// system("cls");
	// int zzz[3] = {2, 2, 0}; 
	// printf("%lf\n\n", calculateEntropy(zzz));
	// printf("%lf\n", calculateEntropy(dist));
	
	// //getEntryLine(initialSet, A.count, 1);

	// //freeVariables(&D, &A);
	
	return 0;

}