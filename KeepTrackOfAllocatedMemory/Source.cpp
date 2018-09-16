#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <string.h>

struct Node
{
	void* address;
	Node* next;
};

Node* createNode(void* memoryAddress)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->address = memoryAddress;
	node->next = NULL;
	return node;
}

void printNode(Node* node)
{
	if (node != NULL)
	{
		printf("Node @%p\n", node);
		printf("\t Memory Leak @%p\n", node->address);
	}
}

Node* insertBeginning(Node* list, Node* node)
{
	if (list == NULL)
	{
		list = node;
	}
	else
	{
		node->next = list;
		list = node;
	}
	return list;
}

void printList(Node* list)
{
	printf("Memory Leaks:\n");
	if (list != NULL)
	{
		Node* tmp = list;
		while (tmp)
		{
			printNode(tmp);
			tmp = tmp->next;
		}
	}
	else
	{
		printf("Empty List\n");
	}
}

void deleteList(Node* &list)
{
	if (list != NULL)
	{
		while (list != NULL)
		{
			Node* tmp = list;
			list = list->next;
			tmp->next = NULL;
			printf("Deleting Node @%p\n", tmp);
			printf("\t Deleting Memory @%p\n", tmp->address);
			free(tmp->address);
			tmp->address = NULL;
			free(tmp);
			tmp = NULL;
		}
		free(list);
		list = NULL;
	}
	else
	{
		printf("Empty List!\n");
	}
}

/* customMalloc function description
param   : list				-> the list where memory addresses are stored
param   : memoryBlockSize	-> the size of the memory to be allocatted
param   : dataTypeSize		-> the size of the data to be stored in the allocated memory(use sizeof operator)
return  :					-> a pointer to the beginning of the memory block allocated
example :					-> char* myChar = (char*)customMalloc(list, strlen("myChar") + 1, sizeof(char));
*/
void* customMalloc(Node* &list, int memoryBlockSize, int dataTypeSize)
{
	void* memoryBlock = (void*)malloc(memoryBlockSize * dataTypeSize);
	list = insertBeginning(list, createNode(memoryBlock));
	return memoryBlock;
}

/* customFree function description
param : list				-> the list where memory addresses are stored
param : memoryAddress		-> a pointer to an address that you want to free
*/
void customFree(Node* &list, void* memoryAddress)
{
	Node* tmp = list;
	Node* prev = NULL;
	if (tmp != NULL && tmp->address == memoryAddress)
	{
		list = tmp->next;
		printf("Deleting Node @%p\n", tmp);
		printf("\tDeleting Memory @%p\n", tmp->address);
		tmp->next = NULL;
		free(tmp->address);
		tmp->address = NULL;
		free(tmp);
		tmp = NULL;
		return;
	}
	while (tmp != NULL && tmp->address != memoryAddress)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		return;
	}
	prev->next = tmp->next;
	printf("Deleting Node @%p\n", tmp);
	printf("\tDeleting Memory @%p\n", tmp->address);
	free(tmp->address);
	tmp->address = NULL;
	free(tmp);
	tmp = NULL;
}

void main()
{
	Node* keepTrackOfAllocatedMemory = NULL;
	char* simpleChar = NULL;
	char** simpleCharArray = NULL;
	double** simpleMatrix = NULL;
	char*** simpleCharMatrix = NULL;
	int timesToRun = 1;
	while (timesToRun)
	{
		simpleChar = (char*)customMalloc(keepTrackOfAllocatedMemory, strlen("KeepTrackOfAllocatedMemory") + 1, sizeof(char));
		// simpleChar = 1 memory Addres
		strcpy_s(simpleChar, strlen("KeepTrackOfAllocatedMemory") + 1, (char*)"KeepTrackOfAllocatedMemory");
		printf("simpleChar = %s\n", simpleChar);
		simpleCharArray = (char**)customMalloc(keepTrackOfAllocatedMemory, 3, sizeof(char*));
		// simpleCharArray = 1 memory Addres
		for (int i = 0; i < 3; i++)
		{
			simpleCharArray[i] = (char*)customMalloc(keepTrackOfAllocatedMemory, strlen("Simple Char Array") + 1, sizeof(char));
		}
		// simpleCharArray[index] = 3 memory Addresses
		// total simpleCharArray = 4 memory Addresses
		for (int i = 0; i < 3; i++)
		{
			strcpy_s(simpleCharArray[i], strlen("Simple Char Array") + 1, (char*)"Simple Char Array");
		}
		for (int i = 0; i < 3; i++)
		{
			printf("simpleCharArray[%d] = %s\n", i, simpleCharArray[i]);
		}
		simpleMatrix = (double**)customMalloc(keepTrackOfAllocatedMemory, 3, sizeof(double*));
		// simpleMatrix = 1 memory Address
		for (int i = 0; i < 3; i++)
		{
			simpleMatrix[i] = (double*)customMalloc(keepTrackOfAllocatedMemory, 3, sizeof(double));
		}
		// simpleMatrix[index] = 3 memory Addresses
		// total simpleMatrix = 4 memory Addresses
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				simpleMatrix[i][j] = i * 2 + j * 3;
			}
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				printf("simpleMatrix[%d][%d] = %lf ", i, j, simpleMatrix[i][j]);
			}
			printf("\n");
		}
		simpleCharMatrix = (char***)customMalloc(keepTrackOfAllocatedMemory, 2, sizeof(char***));
		// simpleCharMatrix = 1 memory Address
		for (int i = 0; i < 2; i++)
		{
			simpleCharMatrix[i] = (char**)customMalloc(keepTrackOfAllocatedMemory, 3, sizeof(char**));
			// simpleCharMatrix[index] =  2 memory Addresses
			for (int j = 0; j < 3; j++)
			{
				simpleCharMatrix[i][j] = (char*)customMalloc(keepTrackOfAllocatedMemory, strlen("ChMtrx") + 1, sizeof(char*));
				// simpleCharMatrix[index][index] = 6 memory Addreses
			}
		}
		// total simpleCharMatrix = 9 memory Addresses
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				strcpy_s(simpleCharMatrix[i][j], strlen("ChMtrx") + 1, (char*)"ChMtrx");
			}
		}
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				printf("simpleCharMatrix[%d][%d] = %s ", i, j, simpleCharMatrix[i][j]);
			}
			printf("\n");
		}
		//total allocated memory = 1(simpleChar) + 4(simpleCharArray) + 4(simpleMatrix) + 9(simpleCharMatrix) = 18 memory Addresses

		//free memory using customFree
		printf("\nBegin free memory using customFree\n");
		customFree(keepTrackOfAllocatedMemory, simpleChar);
		//for (int i = 0; i < 3; i++)
		//{
		//	customFree(keepTrackOfAllocatedMemory, simpleCharArray[i]);
		//}
		customFree(keepTrackOfAllocatedMemory, simpleCharArray);
		//for (int i = 0; i < 3; i++)
		//{
		//	customFree(keepTrackOfAllocatedMemory, simpleMatrix[i]);
		//}
		customFree(keepTrackOfAllocatedMemory, simpleMatrix);
		//for (int i = 0; i < 2; i++)
		//{
		//	for (int j = 0; j < 3; j++)
		//	{
		//		customFree(keepTrackOfAllocatedMemory, simpleCharMatrix[i][j]);
		//	}
		//	customFree(keepTrackOfAllocatedMemory, simpleCharMatrix[i]);
		//}
		customFree(keepTrackOfAllocatedMemory, simpleCharMatrix);
		printf("End of free memory using customFree\n");
		printList(keepTrackOfAllocatedMemory);

		// free entire memory by using the list
		printf("\nBegin free memory using list\n");
		deleteList(keepTrackOfAllocatedMemory);
		printf("End of free memory using list\n");
		printList(keepTrackOfAllocatedMemory);

		simpleChar = (char*)customMalloc(keepTrackOfAllocatedMemory, strlen("Reuse") + 1, sizeof(char));
		// simpleChar = 1 memory Addres
		strcpy_s(simpleChar, strlen("Reuse") + 1, (char*)"Reuse");
		printf("\nsimpleChar = %s\n", simpleChar);
		simpleCharArray = (char**)customMalloc(keepTrackOfAllocatedMemory, 3, sizeof(char*));
		// simpleCharArray = 1 memory Addres
		for (int i = 0; i < 3; i++)
		{
			simpleCharArray[i] = (char*)customMalloc(keepTrackOfAllocatedMemory, strlen("SimpleChrArrayReuse") + 1, sizeof(char));
		}
		// simpleCharArray[index] = 3 memory Addresses
		// total simpleCharArray = 4 memory Addresses
		for (int i = 0; i < 3; i++)
		{
			strcpy_s(simpleCharArray[i], strlen("SimpleChrArrayReuse") + 1, (char*)"SimpleChrArrayReuse");
		}
		for (int i = 0; i < 3; i++)
		{
			printf("simpleCharArray[%d] = %s\n", i, simpleCharArray[i]);
		}
		simpleMatrix = (double**)customMalloc(keepTrackOfAllocatedMemory, 3, sizeof(double*));
		// simpleMatrix = 1 memory Address
		for (int i = 0; i < 3; i++)
		{
			simpleMatrix[i] = (double*)customMalloc(keepTrackOfAllocatedMemory, 3, sizeof(double));
		}
		// simpleMatrix[index] = 3 memory Addresses
		// total simpleMatrix = 4 memory Addresses
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				simpleMatrix[i][j] = (i * 4 + j * 2) + 5;
			}
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				printf("simpleMatrix[%d][%d] = %lf ", i, j, simpleMatrix[i][j]);
			}
			printf("\n");
		}
		simpleCharMatrix = (char***)customMalloc(keepTrackOfAllocatedMemory, 2, sizeof(char***));
		// simpleCharMatrix = 1 memory Address
		for (int i = 0; i < 2; i++)
		{
			simpleCharMatrix[i] = (char**)customMalloc(keepTrackOfAllocatedMemory, 3, sizeof(char**));
			// simpleCharMatrix[index] =  2 memory Addresses
			for (int j = 0; j < 3; j++)
			{
				simpleCharMatrix[i][j] = (char*)customMalloc(keepTrackOfAllocatedMemory, strlen("ChMtrxReuse") + 1, sizeof(char*));
				// simpleCharMatrix[index][index] = 6 memory Addreses
			}
		}
		// total simpleCharMatrix = 9 memory Addresses
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				strcpy_s(simpleCharMatrix[i][j], strlen("ChMtrxReuse") + 1, (char*)"ChMtrxReuse");
			}
		}
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				printf("simpleCharMatrix[%d][%d] = %s ", i, j, simpleCharMatrix[i][j]);
			}
			printf("\n");
		}
		// total allocated memory = 1(simpleChar) + 4(simpleCharArray) + 4(simpleMatrix) + 9(simpleCharMatrix) = 18 memory Addresses
		// free memory using the list of addresses
		printf("\nBegin free memory using list\n");
		deleteList(keepTrackOfAllocatedMemory);
		printf("End of free memory using list\n");
		printList(keepTrackOfAllocatedMemory);
		timesToRun--;
	}
	printf("Done!");
	_getch();
}