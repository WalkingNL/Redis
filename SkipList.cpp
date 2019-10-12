#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEVEL 16

typedef struct SkipList
{
	int data;
	int maxLevel;
	struct SkipList *next[MAX_LEVEL];
}*pSkipList, skipList;

void initNode(pSkipList);
void insertNode(pSkipList, int);
pSkipList findNode(pSkipList, int);
int randomLevel();
void traverseSkipList(pSkipList);

int levelCount = 1;

int main(void)
{
	pSkipList skipListFirst = (pSkipList)malloc(sizeof(skipList));
	initNode(skipListFirst);
	insertNode(skipListFirst, 12);
	insertNode(skipListFirst, 1);
	insertNode(skipListFirst, 3);

	traverseSkipList(skipListFirst);

	return 0;
}

void initNode(pSkipList pNode)
{
	pNode->data = -1;
	for (int i = 0; i < MAX_LEVEL - 1; i++)
	{
		pNode->next[i] = NULL;
	}
}

int randomLevel()
{
	srand((int)time(0));//as a seed in the current time
	int level = 1;

	for (int i = 1; i < MAX_LEVEL; i++) {
		if (rand() % 2 == 1) {
			level++;
		}
	}
	return level;
}

void insertNode(pSkipList FirstNode, int data)
{
	int level = randomLevel();
	pSkipList newNode = (pSkipList)malloc(sizeof(skipList));
	newNode->data = data;
	newNode->maxLevel = level;

	pSkipList *update = (pSkipList*)malloc(sizeof(skipList) * level);

	for (int i = 0; i < level; i++)
	{
		update[i] = FirstNode;
	}

	pSkipList p = FirstNode;

	for (int i = 0; i < level; i++)
	{
		while (p->next[i] != NULL && p->next[i]->data < data)
		{
			p = p->next[i];
		}
		update[i] = p;
	}

	for (int i = 0; i < level; i++)
	{
		newNode->next[i] = update[i]->next[i];
		update[i]->next[i] = newNode;
	}

	if (levelCount < level)
	{
		levelCount = level;
	}
}

pSkipList findNode(pSkipList pHead, int data)
{
	pSkipList p = pHead;
	for (int i = levelCount - 1; i >= 0; i--)
	{
		while (p->next[i] != NULL && p->next[i]->data < data)
		{
			p = p->next[i];
		}
	}

	if (p->next[0] != NULL && p->next[0]->data == data)
	{
		return p->next[0];
	}
	else
	{
		return NULL;
	}
}

void traverseSkipList(pSkipList pFirstNode)
{
	pSkipList p = pFirstNode->next[0];
	while (p != NULL)
	{
		printf("%d\t", p->data);
		p = p->next[0];
	}
	printf("\n");
}

