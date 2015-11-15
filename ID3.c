#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//small entropy == large info gain

/*
WHAT WE NEED:
1) Function to Identify Information Gain given the remaining attributes and the target attributes
2) Linked List Splicing Function which splices the list into new linked lists based on an attribute and its choices
3) Function to Determine the Most Decisive Attribute
4) Recursively Calling ID3 Building Function
5) A Function that returns a line of responses :)

WHAT'S NICE TO HAVE
1) A Function that adds an element to a linked list given the linked list's head pointer and the element to be added
2) A Function that removes an element from a linked list given the linked list's head pointer and the element to be removed
*/

typedef struct contentbase{
	char attrName[512];
	int attrDist[6];
} DecTreeContent;

typedef struct nodebase{
	DecTreeContent properties;
	struct nodebase ** link;
} DecTreeNode;

//log base 2 function
double log2(double x){
	return (log(x) / log(2));
}

//calculate total number of unique attribute types
int calculateAttrTotal(int x[]){
	int i, total = 0;

	for(i = 0; i < 6; i++) total = total + x[i];
	return total;
}

//calculate for entropy
double calculateEntropy(int x[]){
	int i;
	double total = (double) (calculateAttrTotal(x));
	double entropy = 0;

	for(i = 0; i < 6; i++) if (x[i] != 0) entropy = entropy - ((x[i]/total) * log2(x[i]/total));
	return entropy;
}

//calculate for information gain
double calculateInfoGain(int x[]){
	// :))
}


//selects the most decidable/classifying node
DecTreeNode selectAttr(){
	// return the node with the least entropy
}


//recursively build the decision tree
void buildDecisionTree(){
	/*
	halt when:
		a node classifies all input -> leaf node
		no examples can be classified under the attribute -> leaf node for most common class of parent set
		no attributes remaining -> get majority as answer for leaf node


	ID3 (Examples, Target_Attribute, Attributes)
    Create a root node for the tree
    If all examples are positive, Return the single-node tree Root, with label = +.
    If all examples are negative, Return the single-node tree Root, with label = -.
    If number of predicting attributes is empty, then Return the single node tree Root,
    with label = most common value of the target attribute in the examples.
    Otherwise Begin
        A ← The Attribute that best classifies examples.
        Decision Tree attribute for Root = A.
        For each possible value, vi, of A,
            Add a new tree branch below Root, corresponding to the test A = vi.
            Let Examples(vi) be the subset of examples that have the value vi for A
            If Examples(vi) is empty
                Then below this new branch add a leaf node with label = most common target value in the examples
            Else below this new branch add the subtree ID3 (Examples(vi), Target_Attribute, Attributes – {A})
    End
    Return Root
	*/
}

int main(){
	double entropy;
	double infoGain;

	int attrDist[6] = {0, 0, 0, 0, 0, 0};

	attrDist[0] = 9;
	attrDist[1] = 5;
	printf("%lf", calculateEntropy(attrDist));

	//printf("%f", log2(3.000));
	
}