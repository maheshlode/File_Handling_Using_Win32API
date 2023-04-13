#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <strsafe.h>
#include "ErrorMessage.h"
#include "CreateNodeToInsertData.h"
#define ONEKB 1024
#define ONEMB ONEKB*1000
#define FILESIZE ONEMB*10


struct Node
{
	char* cDataBuffer=NULL;
	struct Node* pNextNode = NULL;

};

typedef struct Node DATANODE;

DATANODE* dnHeadNode = NULL;

BOOL InsertDataToNode(char* pcDataBuffer)
{
	DATANODE* dnDataNode = NULL;

	dnDataNode = (DATANODE*)malloc(sizeof(DATANODE));
	if (dnDataNode == NULL)
	{
		printf("\nMemory allocation Failed\n");
		return FALSE;
	}
	dnDataNode->cDataBuffer = (char*)malloc((ONEMB * sizeof(char)));
	if (dnDataNode->cDataBuffer == NULL)
	{
		printf("\nMemory allocation Failed\n");
		return FALSE;
	}
	dnDataNode->cDataBuffer = pcDataBuffer;
	dnDataNode->pNextNode = NULL;

	if (dnHeadNode == NULL)
	{
		dnHeadNode = dnDataNode;
	}
	else
	{
		DATANODE* pdnTraverse = NULL;
		pdnTraverse = dnHeadNode;
		while (pdnTraverse->pNextNode!=NULL)
		{
			pdnTraverse = pdnTraverse->pNextNode;
		}
		pdnTraverse->pNextNode = dnDataNode;
	}
	//free(pcDataBuffer);
	return TRUE;
}

DATANODE* DisplayDataInLinkedList()
{
	
	DATANODE* pdnTraverse = NULL;
	pdnTraverse = dnHeadNode;
	printf("\nData in Linked List\n");
	while (pdnTraverse->pNextNode != NULL)
	{
		printf("%c->", pdnTraverse->cDataBuffer[1]);
		pdnTraverse = pdnTraverse->pNextNode;
	}printf("%c\n", pdnTraverse->cDataBuffer[1]);

	return dnHeadNode;
}

BOOL FreeLinkedList(DATANODE* pdnHeadOfLinkedList)
{
	DATANODE* pdnTraverse = NULL;
	pdnTraverse = pdnHeadOfLinkedList;
	while (pdnTraverse->pNextNode!=NULL)
	{
		free(pdnTraverse->cDataBuffer);
		pdnTraverse= pdnTraverse->pNextNode;
	}pdnTraverse = pdnTraverse->pNextNode;
	return TRUE;
}



DATANODE* SortDataInLinkedList(DATANODE* pHeadNode)
{
	int iSwappedCount = 0;
	DATANODE* pdnSwapNode1;
	//DATANODE* pdnSwapNode2=NULL;


	if (pHeadNode == NULL)
	{
		return NULL;
	}

	do
	{
		iSwappedCount = FALSE;
		pdnSwapNode1 = pHeadNode;

		while ((pdnSwapNode1->pNextNode) != NULL)
		{
			if (((int)(pdnSwapNode1->cDataBuffer[1])) > ((int)(pdnSwapNode1->pNextNode->cDataBuffer[1])))
			{
				if (SwapNodes(pdnSwapNode1, pdnSwapNode1->pNextNode)==FALSE)
				{
					return NULL;
				}
				iSwappedCount = TRUE;
			}
			
				pdnSwapNode1 = pdnSwapNode1->pNextNode;
			
			
		}
		//pdnSwapNode2 = pdnSwapNode1;
	} while (iSwappedCount!=FALSE);

	return pHeadNode;
}



BOOL SwapNodes(DATANODE* pdnSwapNode1, DATANODE* pdnSwapNode2)
{
	char* pcTempBuffer = NULL;
	pcTempBuffer = pdnSwapNode1->cDataBuffer;
	pdnSwapNode1->cDataBuffer = pdnSwapNode2->cDataBuffer;
	pdnSwapNode2->cDataBuffer = pcTempBuffer;

	return TRUE;
}