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


typedef struct decisionTreeNodeInfo treeNodeInfo;
struct decisionTreeNodeInfo{
	int nodeAttrib;
	int terminalFlag;
};

typedef struct decisionTreeNode treeNode;
struct decisionTreeNode{
	int pathChoice;
	treeNodeInfo * info;
	struct decisionTreeNode * successor;
	struct decisionTreeNode * next;
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
//NEW FUNCTION!!! initializes an array of indicated size + 1, size of array noted at 0th index
int * initArray(int size){
	int i;
	int * array = (int *) malloc(sizeof(int) * size + 1);
	array[0] = size;
	
	for(i=1; i<=size; i++){
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
			// printf("%s\n", curr->attributeName);
			
			//access the attribute option list
			currE = curr->E->link->next;
			attrOptionsCtr = 0;			
			
			//verify attribute option list non-emptiness
			if(currE == NULL) printf("Attribute Does Not Hold Values\n");
			else{

				//traverse through attribute option list to count number of options
				while(currE != NULL){
					// printf("\t[%d] %s\n", currE->equivalentValue, currE->entry);
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

				// printf("\n%d\n", array[index][0]);

				if(currE != NULL){
					printf("Unbound shit and shit 2\n");
					exit(0);
				}
			}

			// printf("\n\n");
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
		// printf("%lf\n", entropy);
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

int * getDistribution(int * targetAttrArray, int attrCount, int ** entrySet, int entrySetSize, int * considerArray){
	//printf("%d\n", attrCount - 1);

	int i, j;
	int * x = (int *) malloc(sizeof(int) * attrCount);
	for(i = 0; i < attrCount; i++) x[i] = 0;

	x[0] = targetAttrArray[0];
	for(j = 1; j < entrySetSize; j++){
		//printf("%d\n", set[j][attrCount-1]);
		if(considerArray[j] == 1){
			for(i = 0; i <= targetAttrArray[0]; i++) {
				if(targetAttrArray[i] == entrySet[j][attrCount-1]){
			 		x[i]++;
			 		break;
			 	}
			}
		}
	}

	// printf("\n[");
	// 	for(i = 1; i <= x[0]; i++) printf(" %d ", x[i]);
	// printf("]\n");

	return x;
}

int * reconsiderArray(int ** entrySet, int entrySetSize, int * considerArray, int classifierAttr, int classifierSubAttr){
	int i;
	int * array = (int *) malloc(sizeof(int) * entrySetSize);

	for(i = 0; i < entrySetSize; i++) array[i] = considerArray[i];

	for(i = 1; i < entrySetSize; i++) if(entrySet[i][classifierAttr] != classifierSubAttr) array[i] = 0;
	return array;

}

// //calculate for information gain
/*
attrArray: array of attributes and respective attribute options
attrCount: number of attributes
entrySet: set of entries
entrySetSize: number of entries in the set
considerArray: array that indicates which entries are to be considered
classifierAttr: indicates the index of the attribute for which the classification shall be based upon
*/

double calculateGain(int ** attrArray, int attrCount, int ** entrySet, int entrySetSize, int * considerArray, int classifierAttr){
	int i , xy;

	int * initialDist = getDistribution(attrArray[attrCount - 1], attrCount, entrySet, entrySetSize, considerArray);
	int * subDist;
	double entropy;
	double gain = calculateEntropy(initialDist);

	printf("\nCalculating GAIN accdg to ATTRIBUTE %d\n   Entropy: %lf\n", classifierAttr,  gain);

	for (i = 1; i <= attrArray[classifierAttr][0]; i++){
		subDist = getDistribution(attrArray[attrCount - 1], attrCount, entrySet, entrySetSize, 
		                          reconsiderArray(entrySet, entrySetSize, considerArray, classifierAttr, attrArray[classifierAttr][i]));
		
		entropy = calculateEntropy(subDist);
		gain = gain - entropy;

	}

	// printf("\n\n   (Final Gain) %lf \n", gain);

	// printf("\nCalculating GAIN accdg to ATTRIBUTE %d\n   Initial Entropy: %lf\n", classifierAttr,  gain);

	// for (i = 1; i <= attrArray[classifierAttr][0]; i++){
	// 	subDist = getDistribution(attrArray[attrCount - 1], attrCount, entrySet, entrySetSize, 
	// 	                          reconsiderArray(entrySet, entrySetSize, considerArray, classifierAttr, attrArray[classifierAttr][i]));
		
	// 	printf("\n   DISTRIBUTION according to %d: \n", attrArray[classifierAttr][i]);	
	// 	for (xy = 1; xy <= subDist[0]; xy++){
	// 		printf("\t%d >> %d\n", subDist[xy], attrArray[attrCount - 1][xy]);
	// 	}
		
	// 	entropy = calculateEntropy(subDist);
	// 	printf("\n      (Previous Entropy) %lf - (New Entropy) %lf = ", gain, entropy);
		
	// 	gain = gain - entropy;
	// 	printf("(Current Gain) %lf \n", gain);

	// }

	// printf("\n\n   (Final Gain) %lf \n", gain);
	
	return gain;
}

int selectDeciderAttr(int ** attrArray, int attrCount, int ** entrySet, int entrySetSize, int * considerArray, int * availableAttr){
	int i;
	double gain = -10000, maxGain = -10000;
	int maxGainAttr = -1;

	printf("SELECTING DECIDER ATTRIBUTE\n");
	for(i = 0; i < attrCount - 1; i++){
		//printf("at %d: %d\n", i, availableAttr[i]);
		if(availableAttr[i] == 1){
			gain = calculateGain(attrArray, attrCount, entrySet, entrySetSize, considerArray, i);
			printf("   %d Gain: %lf\n", i, gain);

			if (gain > maxGain){
				maxGain = gain;
				maxGainAttr = i;	
				printf("   CURR MAX GAIN at %d\n", i);
			} 
		}
	}

	printf("\n\nMost INFO GAIN %lf at ATTRIBUTE %d\n", maxGain, maxGainAttr);

	return maxGainAttr;
}

treeNode * buildTree(int ** attrArray, int attrCount, int ** entrySet, int entrySetSize, int * considerArray, int * availableAttr){
	
	//initialize the node
	treeNodeInfo * info = (treeNodeInfo *) malloc(sizeof(treeNodeInfo));
	info->nodeAttrib = -1;
	info->terminalFlag = 0;

	treeNode * node = (treeNode *) malloc(sizeof(treeNode)); 
	node->pathChoice = -1;
	node->info = info;
	node->successor = NULL;
	node->next = NULL;


	int i, j;
	int * currentDist = getDistribution(attrArray[attrCount - 1], attrCount, entrySet, entrySetSize, considerArray);
	// int limit = attrArray[attrCount - 1][0];
	// int currentDist[4] = {3, 1 , 1, 1};
	int limit = currentDist[0];
	int flag = 0;
	int currentMax, currentMaxIndex;

	//CASE 1: Grouped into one option ang lahat ng remaining cases
	for(i=1; i<=limit; i++){
		if (flag != 0) flag = -1;
		if(currentDist[i] != 0){
			flag = 0;
			for(j=1; j<=limit; j++){
				if (i == j) break;
				if (currentDist[j] != 0){
					flag++;
					break;
				}
			}
		}
	}

	if (flag == 0){
		for (i = 1; i <= limit; i++) if(currentDist[i] != 0) break;
		printf("%d\n", i);
		printf("XERO!!!");
		info->nodeAttrib = i;
		info->terminalFlag = 1;
		printf("%d %d\n", info->nodeAttrib, node->info->nodeAttrib);
		return node;	
	} 



	//CASE 2: no more attribute choices
	flag = 0;
	for(i = 0; i < attrCount; i++){
		if (availableAttr[i] == 1){
			flag = 1;
			break;
		}
	}

	if (flag == 0){
		printf("NO MORE AVAILABLE ATTR %d\n", flag);
		
		currentMax = currentDist[1];
		currentMaxIndex = 0;
		for(i = 2; i <= limit; i++){
			if (currentDist[i] > currentMax){
				currentMax = currentDist[i];
				currentMaxIndex = i;
			} 
		}

		info->nodeAttrib = currentMaxIndex;
		info->terminalFlag = 1;
		printf("%d at %d (%d) (%d)\n", currentMax, currentMaxIndex, info->nodeAttrib, node->info->nodeAttrib);
		return node;

	}	



	//OTHERWISE SEGMENT
	int decider = selectDeciderAttr(attrArray, attrCount, entrySet, entrySetSize, considerArray, availableAttr);
	int * newAvailableAttr; 
	for(i=0; i<attrCount; i++) newAvailableAttr[i] = availableAttr[i];
	newAvailableAttr[decider] = 0;
	node->info->nodeAttrib = decider;	
	
	printf("LIMIT %d\n", limit);
	// int newEntryArray[4] = {3, 0, 0, 0};
	int * newEntryArray;

	treeNode * curr;
	curr = NULL;

	for (i = 1; i <= attrArray[decider][0]; i++){

		node->pathChoice = attrArray[decider][currentMaxIndex]; 
		printf("P: %d\n", node->pathChoice);
		newEntryArray = reconsiderArray(entrySet, entrySetSize, considerArray, decider, attrArray[decider][i]);
		
		flag = 0;
		for (j = 1; j <= limit; j++){
			if(newEntryArray[j] != 0){
				flag++;
				break;
			}
		}

		if(flag == 0){
			currentMax = currentDist[1];
			currentMaxIndex = 1;
			for (j = 2; j <= limit; j++){
				if(currentDist[j] > currentMax){
					currentMax = currentDist[j];
					currentMaxIndex = j;
				}
			}		
			printf("%d\n", currentMax);	

			info->nodeAttrib = attrArray[decider][currentMaxIndex];
			info->terminalFlag = 1;

			printf("%d, %d\n", node->info->nodeAttrib, node->info->terminalFlag);
			if (curr == NULL){
				curr = node;
				curr->next = NULL;
			}

			else{
				curr->next = node;
				curr = curr->next;
				curr->next = NULL;
			}
			return node;
		}

		else{
			node->successor = buildTree(attrArray, attrCount, entrySet, entrySetSize, newEntryArray, newAvailableAttr);

			if (curr == NULL){
				curr = node;
				curr->next = NULL;
			}

			else{
				curr->next = node;
				curr = curr->next;
				curr->next = NULL;
			}
		}


	}


}

// ...AND THIS COMMENT :)

void printStats(Attrib * A){
	attribNode * curr;
	entryNode * durr;
	
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
	int * availableAttr;
	int * dist;
	int i;
	
	//file read and initialization
	initialSet = readinput(&D, &A);
	printf("\n\n");
	// printf("\n\n%s\n\n", returnEquivalent(&D, 9));
	
	//start of ID3 shit supposedly
	// system("cls");
	attrArray = getAttrArray(A);
	// system("cls");
	traverseAttrArray(&D, attrArray, A.count);
	// system("cls");

	considerArray = initArray(50);
	availableAttr = (int *) malloc(sizeof(int) * A.count);
	for(i=0; i<A.count; i++) availableAttr[i] = 1; 
		printf("FLAG\n");
	
	// dist = getDistribution(attrArray[A.count - 1], A.count, initialSet, 51, considerArray);


	//PRINT DISTRIBUTION TEST
	// system("cls");
	// printf("DISTRIBUTION: \n");	
	// for (i = 1; i <= dist[0]; i++){
	// 	printf("\t%d >> %s\n", dist[i], returnEquivalent(&D, attrArray[A.count - 1][i]));
	// }

	//ENTROPY TEST
	// system("cls");
	// int zzz[5] = {4, 2, 0, 0, 0}; 
	// printf("%lf\n\n", calculateEntropy(zzz));
	// printf("%lf\n", calculateEntropy(dist));

	//RECONSIDER TEST
	// printf("\n\n%s\n\n", returnEquivalent(&D, 0));
	// int * newarray = reconsiderArray(initialSet, 51, considerArray, 0, 9);
	

	//GAIN TEST
	// system("cls");
	// calculateGain(attrArray, A.count, initialSet, 51, considerArray, 0);

	//DECIDER TEST
	// system("cls");
	// selectDeciderAttr(attrArray, A.count, initialSet, 51, considerArray, availableAttr);
	
	//BUILDER TEST
	int availableAttr2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	system("cls");
	buildTree(attrArray, A.count, initialSet, 51, considerArray, availableAttr);
	
	// getEntryLine(initialSet, A.count, 1);

	// freeVariables(&D, &A);
	
	return 0;

}