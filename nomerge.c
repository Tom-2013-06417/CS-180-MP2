#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>


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


typedef struct decisionTreeNode treeNode;
struct decisionTreeNode{
	int attrib;
	int terminalFlag;
	int numPaths;
	int * pathChoice;
	struct decisionTreeNode ** path;
};



Dict D;



//Unique Randomizer code segment retrieved from http://stackoverflow.com/questions/5064379/generating-unique-random-numbers-in-c

#define ERR_NO_NUM -1
#define ERR_NO_MEM -2

int myRandom (int size) {
	int i, n;
	static int numNums = 0;
	static int *numArr = NULL;

	// Initialize with a specific size.

	if (size >= 0) {
		if (numArr != NULL)
			free (numArr);
		if ((numArr = (int * )malloc (sizeof(int) * (size))) == NULL)
			return ERR_NO_MEM;
		for (i = 0; i  < size; i++)
			numArr[i] = i;
		numNums = size;
	}

	// Error if no numbers left in pool.

	if (numNums == 0)
	   return ERR_NO_NUM;

	// Get random number from pool and remove it (rnd in this
	// case returns a number between 0 and numNums-1 inclusive).

	n = rand() % numNums;
	i = numArr[n];
	numArr[n] = numArr[numNums-1];
	numNums--;
	if (numNums == 0) {
		free (numArr);
		numArr = 0;
	}

	return i;
}



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

char * getAttrName(int index){
	Dict * dict = &D;
	dNode * curr = dict->top;

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
		array[i] = 0;
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


//log base x function
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
	char c;

	for(i = 0; i < entrySetSize; i++) {
		array[i] = considerArray[i];
	}

	for(i = 1; i < entrySetSize; i++) {
		if(entrySet[i][classifierAttr] != classifierSubAttr) array[i] = 0;
		// printf("%d vs. %d\n", entrySet[i][classifierAttr], classifierSubAttr);
		// printf("%d     %d\n", array[i], considerArray[i]);
	}
	//scanf("%c",&c);
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

	//printf("\nCalculating gain at attribute %s\n   Entropy: %lf\n", returnEquivalent(&D, classifierAttr),  gain);

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

	// printf("Selecting the decider attribute\n");
	for(i = 0; i < attrCount - 1; i++){
		//printf("at %d: %d\n", i, availableAttr[i]);
		if(availableAttr[i] == 1){
			gain = calculateGain(attrArray, attrCount, entrySet, entrySetSize, considerArray, i);
			// printf("   %d Gain: %lf\n", i, gain);

			if (gain > maxGain){
				maxGain = gain;
				maxGainAttr = i;	
				// printf("   Current Max Gain at %d\n", i);
			} 
		}
	}

	// printf("\nMost gain %lf at attribute %s\n", maxGain, returnEquivalent(&D, maxGainAttr));

	return maxGainAttr;
}

treeNode * buildTree(int ** attrArray, int attrCount, int ** entrySet, int entrySetSize, int * considerArray, int * availableAttr){
	//initialize the node
	treeNode * node = (treeNode *) malloc(sizeof(treeNode)); 
	node->attrib = 0;
	node->terminalFlag = 0;
	node->numPaths = 0;
	node->pathChoice = NULL;
	node->path = NULL;

	int i, j;
	int flag = 0;
	int currentMax, currentMaxIndex;
	int * currentDist = getDistribution(attrArray[attrCount - 1], attrCount, entrySet, entrySetSize, considerArray);
	int numTarget = currentDist[0];
	
	// printf("Distribution  : [");
	// for(i=1; i<=attrArray[attrCount - 1][0]; i++){
	// 	printf(" %d ", currentDist[i]);
	// }
	// printf("]\n");

	//CASE 1: Grouped into one option ang lahat ng remaining cases
	for(i=1; i<=numTarget; i++){
		if (flag != 0) flag = -1;
		if(currentDist[i] != 0){
			flag = 0;
			for(j=1; j<=numTarget; j++){
				if (i == j) break;
				if (currentDist[j] != 0){
					flag++;
					break;
				}
			}
		}
	}

	if (flag == 0){
		for (i = 1; i <= numTarget; i++) if(currentDist[i] != 0) break;
		node->attrib = attrArray[attrCount - 1][i];
		node->terminalFlag = 1;
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
		currentMax = currentDist[1];
		currentMaxIndex = 1;
		for(i = 2; i <= numTarget; i++){
			if (currentDist[i] > currentMax){
				currentMax = currentDist[i];
				currentMaxIndex = i;
			} 
		}

		node->attrib = currentMaxIndex;
		node->terminalFlag = 1;
		return node;
	}	



	//OTHERWISE SEGMENT
	int decider = selectDeciderAttr(attrArray, attrCount, entrySet, entrySetSize, considerArray, availableAttr);
	node->attrib = decider;	
	
	int * newAvailableAttr = (int *) malloc(sizeof(int)*attrCount); 
	for(i=0; i<attrCount; i++) newAvailableAttr[i] = availableAttr[i];
	newAvailableAttr[decider] = 0;

	int * newEntryArray = (int *)malloc(sizeof(int) * (entrySetSize+1));
	
	treeNode * curr = NULL;

	treeNode * temp = (treeNode *)malloc(sizeof(treeNode));
	temp->attrib = 0;
	temp->terminalFlag = 0;
	temp->numPaths = 0;
	temp->pathChoice = NULL;
	temp->path = NULL;
	
	int numAttr = attrArray[decider][0];
	int d;
	node->path = (treeNode **)malloc(sizeof(treeNode*)* (numAttr+1));
	for(d = 0; d<= numAttr; d++) node->path[d] = NULL;

	node->pathChoice = (int *)malloc(sizeof(int) * (numAttr+1));
	node->pathChoice[0] = numAttr;
	
	for (d = 1; d <= numAttr; d++){

		node->pathChoice[d] = attrArray[decider][d]; 
		
		// printf("Ramification  : \"%s\" under \"%s\" (%d out of %d)\n", returnEquivalent(&D, node->pathChoice[d]), returnEquivalent(&D, decider), d, numAttr);
		newEntryArray = reconsiderArray(entrySet, entrySetSize, considerArray, decider, attrArray[decider][d]);

		// printf("Entries:\n[\n");
		// for(j=1; j <51; j++){
		// 	printf(" %d ", newEntryArray[j]);
		// 	if(j%10 == 0) printf("\n");
		// }
		// printf("]\n");

		flag = 0;
		for (j = 1; j < entrySetSize; j++){
			if(newEntryArray[j] != 0){
				flag++;
				break;
			}
		}

		if(flag == 0){
			currentMax = currentDist[1]; 
			currentMaxIndex = 1;
			for (j = 2; j <= numTarget; j++){
				//printf("%d\n", currentDist[j]);

				if(currentDist[j] > currentMax){
					currentMax = currentDist[j];
					currentMaxIndex = j;
				}
			}		

			//printf("%d %d / %d\n", currentDist[currentMaxIndex], currentMaxIndex, attrArray[attrCount - 1][currentMaxIndex]);
			temp->attrib = attrArray[attrCount - 1][currentMaxIndex];
			temp->terminalFlag = 1;
			node->path[d] = temp;
			// printf("%s\n", returnEquivalent(&D, temp->attrib));
			// printf("\n\n\n\n");
			// printf("ERROR!!!\n");
			// printf("Classification: Attribute \"%s\" --- \"%s\" ---> Goes To \"%s\"\n", returnEquivalent(&D, node->attrib), returnEquivalent(&D, node->pathChoice[d]), returnEquivalent(&D, temp->attrib));

		}

		else{
			node->terminalFlag = 0;
			node->path[d] = buildTree(attrArray, attrCount, entrySet, entrySetSize, newEntryArray, newAvailableAttr);
			// printf("Classification: Attribute \"%s\" --- \"%s\" ---> Goes To \"%s\"\n", returnEquivalent(&D, node->attrib), returnEquivalent(&D, node->pathChoice[d]), returnEquivalent(&D, node->path[d]->attrib));
			
		}

		//printf("\n");
	}

	return node;
}

int assess(treeNode * root, int * testCase, int attrCount){
	int i, x, y;
	treeNode * curr = root;

	printf("TEST CASE : [");
	for(i=0; i<attrCount; i++){
		printf(" %s ", getAttrName(testCase[i]));
	}
	printf("]\n");

	while(curr->terminalFlag != 1){
		x = testCase[curr->attrib];
		for(i=1; i<curr->pathChoice[0]; i++) if(curr->pathChoice[i] == x) break;
		curr = curr->path[i];
	}

	printf("CONCLUSION: %s\n", getAttrName(testCase[attrCount - 1]));
	printf("HYPOTHESIS: %s\n", getAttrName(curr->attrib));
	//printf("\n");

	return (strcmp(getAttrName(curr->attrib), getAttrName(testCase[attrCount - 1])) == 0 ? 1:0);
}

void printDecisionTree(treeNode * root, int space){
	int i, j;
	treeNode * curr = root;


	printf("%s\n", getAttrName(root->attrib));
	
	for(i = 1; i <= root->pathChoice[0]; i++){
		for(j = 0; j < space + 1; j++) printf("|--");
		printf("> [%s] ", getAttrName(root->pathChoice[i])); 

		curr = root->path[i];
		if (curr->terminalFlag != 1) printDecisionTree(curr, space+1);
		else printf("%s\n", getAttrName(curr->attrib));
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

void printConsiderArray(int * considerArray, int size){
	int i;

	for (i=0; i<size; i++){
		printf("%d ", considerArray[i]);
	}
}

void selectTrainingSamples(int ** inputArray, int * trainingSamplesConsiderArray){
	int i, j;

	i = myRandom (51);

	for (j=1; j<=30; j++){
		if (i == 0)	i = myRandom (-1);
		trainingSamplesConsiderArray[i] = 1;
		i = myRandom (-1);
	}
}

void selectSmallerTrainingSamples(int ** inputArray, int * trainingSamplesConsiderArray){
	int i, j, trainCounter=0, testCounter=0;
	
	i = myRandom (51);

	for (j=1; j<=50; j++){
		if (i == 0)	i = myRandom (-1);
		
		if (trainingSamplesConsiderArray[i] == 1 && trainCounter < 15){
			trainingSamplesConsiderArray[i] = 2;
			trainCounter++;
		}

		else if (trainingSamplesConsiderArray[i] == 0 && testCounter < 10){
			trainingSamplesConsiderArray[i] = 3;
			testCounter++;
		}

		i = myRandom (-1);
	}

	for (j=1; j<=50; j++){
		if (trainingSamplesConsiderArray[j] == 1) trainingSamplesConsiderArray[j] = 0;
	}

	for (j=1; j<=50; j++){
		if (trainingSamplesConsiderArray[j] == 2) trainingSamplesConsiderArray[j] = 1;
	}

}

int calculateAccuracy(treeNode * root, int ** entries, int * considerArray, int count, int testCasesCount, int experimentNumber){
	int i, num=0;
	float accuracy;

	if (experimentNumber == 1){

		for(i=1; i<testCasesCount+1; i++) {
			if (considerArray[i] == 0){
				printf("ENTRY #  %d\n", i);
				if (assess(root, entries[i], count)){
					num++;
				}
				printf("\n");
			}
		}

		accuracy = (float)num/float(20)*100;
		printf("Accuracy: %.2f %%\n", accuracy);
		return accuracy;

	}

	else if (experimentNumber == 2){

		for(i=1; i<testCasesCount+1; i++) {
			if (considerArray[i] == 3){
				printf("ENTRY # %d\n", i);
				if (assess(root, entries[i], count)){
					num++;
				}
				printf("\n");
			}
		}

		accuracy = (float)num/float(10)*100;
		printf("Accuracy: %.2f %%\n", accuracy);
		return accuracy;

	}

}

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

	//printStats(A);
	//printEquivalentTable(equivalentTable, A->count);
	
	fclose(input);

	return equivalentTable;
}



int main(){

	system("cls");

	initDict(&D);

	Attrib A;
	initAttrib(&A);

	int ** initialSet;
	int ** attrArray;

	int * considerArray;
	int * considerArrayForExperiment2;
	int * availableAttr;
	int * dist;
	int i, m;

	float averageAccuracy;

	srand(time(NULL));	
	
	initialSet = readinput(&D, &A);																	//file read and initialization
	attrArray = getAttrArray(A);

	traverseAttrArray(&D, attrArray, A.count);
	availableAttr = (int *) malloc(sizeof(int) * A.count);


	printf("\n\nEXPERIMENT 1: \n\n");
	
	////////////////////////////START EXPERIMENT NUMBER 1: MORE SAMPLES//////////////////////////////

	for (m=0; m<5; m++){

		printf("K-fold cross validation: K=%d\n\n", m+1);

		considerArray = initArray(50);
		
		selectTrainingSamples(initialSet, considerArray);											//Function to return randomly sampled training and test samples!
		
		if (m == 0) {
			considerArrayForExperiment2 = initArray(50);
			for (i=1; i<=50; i++) considerArrayForExperiment2[i] = considerArray[i];
		}

		for (i=0; i<A.count; i++) availableAttr[i] = 1; 

		treeNode * root = (treeNode *) malloc(sizeof(treeNode));
		root = buildTree(attrArray, A.count, initialSet, 51, considerArray, availableAttr);

		printDecisionTree(root, 0);
		printf("\n");	

		averageAccuracy += calculateAccuracy(root, initialSet, considerArray, A.count, 50, 1);
		printf("\n\n");

	}

	averageAccuracy /= 5;

	printf("\nAverage accuracy of Decision Tree for Experiment 1: %.2f %%\n", averageAccuracy);

	//////////////////////////////////////END OF EXPERIMENT 1////////////////////////////////////////


	printf("\n\nEXPERIMENT 2: \n\n");


	////////////////////////////START EXPERIMENT NUMBER 2: LESS SAMPLES//////////////////////////////	

	selectSmallerTrainingSamples(initialSet, considerArrayForExperiment2);

	for(i=0; i<A.count; i++) availableAttr[i] = 1; 

	treeNode * root = (treeNode *) malloc(sizeof(treeNode));
	root = buildTree(attrArray, A.count, initialSet, 51, considerArrayForExperiment2, availableAttr);

	printDecisionTree(root, 0);
	printf("\n");	

	averageAccuracy += calculateAccuracy(root, initialSet, considerArrayForExperiment2, A.count, 50, 2);
	printf("\n\n");

	//////////////////////////////////////END OF EXPERIMENT 2////////////////////////////////////////


	printf("\n\nEXPERIMENT 3: \n\n");


	///////////////////////////START EXPERIMENT NUMBER 3: NOISY SAMPLES//////////////////////////////



	//TRAVERSAL TEST
	// system("cls");
	// printf("%d\n", A.count);
	// int test[9] = {28, 29, 23, 24, 30, 31, 21,26, 38};
	// // med justenough public average manageable few notreally neardcs maginhawa
	// printf("\n");

	// printf("\n\nNOISEEEE\n");
	// printf("%d", assess(root, test, A.count));
	
	return 0;

}