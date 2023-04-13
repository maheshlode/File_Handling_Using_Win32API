#pragma once
typedef struct Node DATANODE;
BOOL InsertDataToNode(char* pcDataBuffer);
DATANODE* DisplayDataInLinkedList();
BOOL FreeLinkedList(DATANODE*);
DATANODE* SortDataInLinkedList(DATANODE*);
BOOL SwapNodes(DATANODE*, DATANODE*);