# KeepTrackOfAllocatedMemory

## A simple project for keeping track of the allocated memory in C.
 - The solution uses a simply linked list to store the memory addresses(pointers).
 - It has four functions : CustomMalloc, CustomFree, PrintList and DeleteList.

### CustomMalloc:
 - The function usage is similar to standard malloc.
 - Stores the allocated memory addresses in a simply linked list.
 - Returns a pointer to the beginning of the allocated memory block.

### CustomFree:
 - Similar to standard free.
 - Frees the allocated memory and remove the node containing the allocated memory address from the linked list.

### PrintList:
 - This function displays the memory leaks.

### DeleteList:
 - This function is designed to free the entire allocated memory.
