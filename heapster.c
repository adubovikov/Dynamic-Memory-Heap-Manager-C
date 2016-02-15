#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Block {
	int block_size; // # of bytes in the data section
	struct Block *next_block; //in C, you have to use "struct Block as the type
	void *data; // a pointer to where the data starts
};

const int SIZE_OF_VOID = sizeof(void*);
const int SIZE_OF_OVERHEAD = sizeof(struct Block);
void * mallocFreePointer; //this is just for fun so that there is no memory leak
struct Block *free_head;

void my_initialize_heap(int size) {
	if(mallocFreePointer != NULL) //memory leak checking
		free(mallocFreePointer);
	free_head = malloc(size) ;
	free_head->data = (unsigned char*)free_head + SIZE_OF_OVERHEAD;
	mallocFreePointer = free_head; //memory leak checking
	free_head->block_size = size - SIZE_OF_OVERHEAD;
	free_head->next_block = NULL;
}

void* my_alloc(int size) {
	if (size % SIZE_OF_VOID != 0)
		size += SIZE_OF_VOID;
	size = size - (size % SIZE_OF_VOID);
	struct Block *tempItr = free_head;
	struct Block *lastTemp = tempItr;
	struct Block *nextTemp = tempItr;
	while (tempItr != NULL) {
		if (tempItr->block_size >= size) { 
			if (tempItr->block_size - size >= SIZE_OF_OVERHEAD + SIZE_OF_VOID) {
				//i need to split
				nextTemp = (struct Block *)(((unsigned char*)tempItr->data) + size);
				nextTemp->block_size = (tempItr->block_size - size - SIZE_OF_OVERHEAD);
				if (tempItr != free_head) { //not free_head
					lastTemp->next_block = nextTemp;
					nextTemp->next_block = tempItr->next_block;
				}
				else
					free_head = nextTemp;
				nextTemp->data = (unsigned char*)nextTemp + SIZE_OF_OVERHEAD;
				return tempItr->data;
			}
			else {
				lastTemp = tempItr->next_block;
				if (free_head == tempItr)
					free_head = lastTemp;
				return tempItr->data;
			}
		}
		lastTemp = tempItr;
		tempItr = tempItr->next_block;
	}
	return 0;
}

void my_free(void *data) {
	struct Block *temp_head = free_head;
	free_head = (struct Block *)((unsigned char*)data - SIZE_OF_OVERHEAD);
	free_head->next_block = temp_head;
}

int main(void) {
	//TEST MAIN PROGRAM
	my_initialize_heap(2000);
	void *test1 = my_alloc(sizeof(int));
	my_free(test1);
	void *test2 = my_alloc(sizeof(int));
	void *test3;
	my_free(test2);
	printf("1) %p = %p\n", test1, test2);
	test1 = my_alloc(sizeof(int));
	test2 = my_alloc(sizeof(int));
	my_free(test2);
	my_free(test1);
	printf("2) %p != %p\n", test1, test2);
	test1 = my_alloc(sizeof(int));
	test2 = my_alloc(sizeof(int));
	test3 = my_alloc(sizeof(int));
	printf("3) %p, %p and %p are three ints\n", test1, test2, test3);
	my_free(test2);
	my_free(test3);
	test2 = my_alloc(sizeof(double));
	printf("   this is a double %p\n", test2);
	test3 = my_alloc(sizeof(int));
	printf("   this is another int... %p\n", test3);
	my_free(test1);
	my_free(test2);
	my_free(test3);
	test1 = my_alloc(sizeof(char));
	test2 = my_alloc(sizeof(int));
	printf("4) char: %p\n   int: %p", test1, test2); 
	my_free(test2);
	test1 = my_alloc(sizeof(int[100]));
	test2 = my_alloc(sizeof(int));
	printf("5) %d = %d?\n", ((int)test2 - (int)test1), (100*sizeof(int) + SIZE_OF_OVERHEAD));
	printf("   %p this is the in pointer", test2);
	my_free(test1);
	my_free(test2);
	printf("\nTest Complete\nThis is the start of the Standard Deviation Program:\n");

	//STANDARD DEVIATION PROGRAM
	int n = 0;
	while (n < 1) {
		printf("Enter in a pos integer\n");
		scanf_s("%d", &n); 
	}
	int *intArray = (int*) my_alloc(sizeof(int) * n);
	int sum = 0;
	for (int i = 0; i < n; i++) {
		printf("please enter integer %d\n", (i + 1));
		scanf_s("%d", &intArray[i]);
		sum += intArray[i];
	}
	double dev = 0;
	for (int i = 0; i < n; i++) {
		dev += pow((double)intArray[i] - ((double)sum / (double)n), 2);
	}
	dev = sqrt(dev / n);
	printf("this is the standard deviation for what you entered: %f\n", dev);
	free(mallocFreePointer);
	while (0 == 0) {} // so that the stupid consol doesnt close...
	return 0;

}
