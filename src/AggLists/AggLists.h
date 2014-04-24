/*! @file AggLists.h
 * @brief A Linked List implementation specifically for Tag Read Data
 * 
 * This library will be used to create linked lists of tag read data
 * and allow for aggregation of the tag reads, eliminiating extraneous
 * tag reads. This should save some processing down stream from the
 * TMR.
 * 
 * @bug No known bugs at this time
 * 
 */
#ifndef AGG_LISTS_SEEN
#define AGG_LISTS_SEEN

/*!@struct node
 * @brief Structure that will hold the data for each NODE on a LL
 * @var node::tagNum
 * The tag number being stored in the linked lists
 * @var node::antNum
 * The antenna number the tag has been identified on
 * @var node::next
 * A pointer to the next node in the list, this is a pointer of type node
 */
struct node{
	int tagNum,
	    antNum;
	struct node* next;
};

typedef struct node Node, *NodePtr;
/*!
 */
int length(NodePtr top);
NodePtr makeNode(int tag, int ant);
NodePtr deleteNode(NodePtr curr, NodePtr previous);
NodePtr deleteHead(NodePtr top);
NodePtr addToHead(NodePtr top, int tag, int ant);
NodePtr addInBody(NodePtr top, NodePtr nodeToAdd, int position);
void printList(NodePtr np);
void deleteList(NodePtr* listTop);
/**
 * @brief This is a function that will search through the linked list for tag and ant
 *
 * @param top The top of the linked list. 
 * @param tag The tag number we're attmpting to find
 * @param ant The antenna number we're attempting to find
 * @return NodePtr Null if tag and ant not found. Pointer to node if found.
 **/
NodePtr search(NodePtr top, int tag, int ant);
NodePtr addToTail(NodePtr top, int tag, int ant);

#endif
