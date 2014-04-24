#include <stdio.h>
#include <stdlib.h>
#include "AggLists.h"

int length(NodePtr top) {
    int n = 0;
    NodePtr curr = top;
    while(curr != NULL) {
        n++;
        curr = curr->next;
    }
    return n;
}//end length

NodePtr
makeNode(int tag, int ant) {
    NodePtr np = (NodePtr)malloc(sizeof(Node));
    if(np == NULL)
        return NULL;
    np -> tagNum = tag;
    np -> antNum = ant;
    np -> next = NULL;
    return np;
}//end makeNode

NodePtr
deleteNode(NodePtr curr, NodePtr previous)
{
    if(curr->next == NULL)
        return NULL;
    previous -> next = curr -> next;
    free(curr);
    return ;
}//end deleteNode

NodePtr
deleteHead(NodePtr top)
{
    if(top->next == NULL)
        return NULL;
    NodePtr old = top;
    top = top->next;
    free(old);
    return top;
}//end deleteHead

NodePtr
addToHead(NodePtr top, int tag, int ant)
{
    NodePtr np = makeNode(tag, ant);
    if(top == NULL) {
        top = np;
        top->next = NULL;
        return top;
    }
    //NodePtr newNode = makeNode(nodeToAdd, 0);
    np -> next = top ;
    top = np;

    return top;
}//end addToHead

NodePtr
addInBody(NodePtr top, NodePtr nodeToAdd, int position)
{
    return NULL;
}//end addInBody

void deleteList(NodePtr* listTop)
{
    while(*listTop != NULL) {
        *listTop = deleteHead(*listTop);
    }
}//end deleteList

void
printList(NodePtr np) {
    while(np != NULL) {
        printf("%d is the tag and %d is the ant\n", np-> tagNum, np -> antNum);
        np = np -> next;
    }
}//end printList

NodePtr
addToTail(NodePtr top, int tag, int ant) {

    NodePtr current,newNode;

    current = top;

    newNode = makeNode(tag, ant);
    newNode->next = NULL;

    if(current == NULL){
      return (current->next = newNode);
    }

    while(current-> next != NULL) {
      printf("Current is: tag %d and ant %d\n", current->tagNum, current->antNum);
        current = current -> next;
    }
    current->next = newNode;
    return current;
}
/**
* Send in the top NodePtr and the tag and ant combo that we want
* to search for.
* Returns NULL on not found or top when found.
*/
NodePtr search(NodePtr top, int tag, int ant) {
    while(top != NULL && top->tagNum != tag && top->antNum != ant)
        top = top->next;

    return top;

}//end searchForExisting

/*  Run the following line to compile this test framework:
* gcc -DAGG_LISTS_DEBUG AggLists.c -o Test
*/
#ifdef AGG_LISTS_DEBUG
int
main(void) {
    NodePtr top, last, np, temp;
    int enteredNum1 = 0, enteredNum2 = 0;

    top = NULL;

    printf("Enter a number, I'll add it to the list:");
    scanf("%d %d", &enteredNum1, &enteredNum2);

    while(enteredNum1 != 0 && enteredNum2 != 0) {
        np = makeNode(enteredNum1, enteredNum2);
        if(top == NULL) top = np;
        else last->next = np;
        last = np;
        printf("Again: ");
        scanf("%d %d", &enteredNum1, &enteredNum2);
    }
    printList(top);
//     printf("Now I'm going to delete the first node\n");
//     top = deleteHead(top);
//     printList(top);
//     printf("Now I'm going to add a node to the head: 99 99\n");
//     top = addToHead(top, 99, 99);
//     printList(top);
    printf("The current length of top before zeroing is %d\n", length(top));

   // printf("Searching for existing node in the list: 20 20 is being looked for. \n");
   // temp = search(top, 20, 20);

    top = addToTail(top, 75, 75);

    printf("Printing list after addToTail\n");
    printList(top);

//     if(temp->next != NULL)
//         printf("Found 20 20! NOT NULL returned!\n");
//     printList(top);
//     //deleteList(&top);
//     if(length(top) == 0)
//         printf("Zeroed!\n");
//     else
//         printf("Not Zeroed!\n");
// 
//     if(search(top, 50, 50) != NULL) {
//         top = addToTail(top, 50, 50);
//     }
//     printList(top);
    deleteList(&top);
}
#endif