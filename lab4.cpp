#include "stdio.h"
#include"stdlib.h"
#include"time.h"
#include"lab4.h"
#define R 32    //物理内存块数
#define V 64   //虚拟内存块数
int block = 5;
int access[32]; //访问序列
int* memo;
int lost = 0;//缺页数
int index = 0;//指示当前下标
LNode* nodes;//改进型Clock置换算法用到的数据结构
int size = 3;//给进程分配的内存的大小
int p;//工作集的起始位置
int table[32];//物理内存，每一个元素代表一个页面
Link idle;
Link modified;

void generate()
{
	srand((unsigned)time(NULL)); //用时间做种，每次产生随机数不一样
	int p = rand() % 64;
	int m = 8, e = 8;
	int i, j;
	double t;
	t = rand() % 10 / 10.0;

	for (i = 0; i < 4; i++)
	{
		for (j = i * m; j < (i + 1) *m; j++)
		{
			access[j] = (p + rand() % e) % 64;
		}

		double r = (rand() % 10) / 10.0;

		if (r < t)
		{
			p = rand() % 64;
		}

		else
		{
			p = (p + 1) % 64;
		}
	}
}


//最佳置换算法
void initMemo()
{
	memo = (int*)malloc(block * sizeof(int));
	int i = 0;

	for (; i < block; i++)
	{
		memo[i] = -1;
	}

	return;
}

bool  isInMemo(int n)
{
	int i = 0;

	for (; i < block; i++)
	{
		if (access[n] == memo[i])
		{
			return true;
		}
	}

	return false;
}
void optimal(int n)
{
	int i = 0, j = 0;

	if (isInMemo(n))
	{
		printf("---------------页面已被调入---------------\n");
	}

	else
		if (index == block)
		{
			lost++;
			int max = 0, pos, tag;

			for (i = 0; i < block; i++)
			{
				tag = -1;

				for (j = n + 1; j < 32; j++)
				{
					if (access[j] == memo[i])
					{
						tag = j;
						break;
					}
				}

				if (tag == -1)
				{
					max = 32;
					pos = i;
					break;
				}

				else
				{
					if (max < tag)
					{
						max = tag;
						pos = i;
					}
				}
			}

			memo[pos] = access[n];
		}

		else
		{
			memo[index] = access[n];
			index++;
		}
}

void testOptimal()
{
	initMemo();
	int i = 0;
	printf("---------------最佳置换算法---------------\n");

	for (; i < 32; i++)
	{
		optimal(i);
		printf("%d %d %d\n", memo[0], memo[1], memo[2]);
	}

	printf("最佳置换算法缺页率： %2f %d\n\n\n", lost / 32.0, lost);
	lost = 0;
	free(memo);
	index = 0;
}

//先进先出置换算法
bool isInNodes(int n)
{
	int i;

	for (i = 0; i < block; i++)
	{
		if (nodes[i].data == access[n])
		{
			return true;
		}
	}

	return false;
}
void initQueue(pQueue q)
{
	q->rear = (pNode)malloc(sizeof(Node));

	if (q->rear == NULL)
	{
		printf("failed\n");
	}

	else
	{
		q->front = q->rear;
		q->rear->next = NULL;
		q->front->next = NULL;
		q->n = 0;
	}
}

void push(pQueue q, int num)
{
	pNode p = (pNode)malloc(sizeof(Node));

	if (p == NULL)
	{
		printf("failed");
	}

	else
	{
		p->next = NULL;
		p->num = num;

		if (q->front == q->rear)
		{
			q->front->next = p;
			q->rear = p;
		}

		else
		{
			q->rear->next = p;
			q->rear = p;
		}

		q->n++;
	}
}

void pop(pQueue q)
{
	pNode p;

	if (q->front != q->rear)
	{
		p = q->front->next;
		q->front->next = p->next;

		if (p == q->rear)
		{
			q->front = q->rear;
		}

		q->n--;
		free(p);
	}
}

void destroy(pQueue q)
{
	while (q->front != q->rear)
	{
		pop(q);
	}
}


bool findInQueue(pQueue q, int num)
{
	pNode p;

	if (q->front != q->rear)
	{
		p = q->front->next;

		while (p)
		{
			if (p->num == num)
			{
				return true;
			}

			else
			{
				p = p->next;
			}
		}
	}

	return false;
}

void fifo(pQueue q, int num)
{
	if (findInQueue(q, num))
	{
		printf("---------------已装入内存---------------\n");
	}

	else
	{
		if (q->n == size)
		{
			pop(q);
			push(q, num);
			lost++;
		}

		else
		{
			push(q, num);
		}
	}
}

void fifoTest()
{
	Queue q;
	pNode p;
	initQueue(&q);
	int i = 0;
	printf("---------------先进先出置换算法---------------\n");

	for (; i < 32; i++)
	{
		fifo(&q, access[i]);
		p = q.front->next;

		while (p)
		{
			printf("%d ", p->num);
			p = p->next;
		}

		printf("\n");
	}

	printf("先进先出算法缺页率：%f  %d\n\n\n", lost / 32.0, lost);
	destroy(&q);
}


//最近最久未使用算法
void LRU(int n)
{
	int i, j;

	if (isInMemo(n))
	{
		printf("---------------已经装入内存---------------\n");
	}

	else
		if (index == block)
		{
			int max = n, pos = -1, tag;

			for (i = 0; i < block; i++)
			{
				for (j = n - 1; j >= 0; j--)
				{
					if (access[j] == memo[i])
					{
						tag = j;
						break;
					}
				}

				if (tag < max)
				{
					max = tag;
					pos = i;

					if (max == 0)
					{
						break;
					}
				}
			}

			memo[pos] = access[n];
			lost++;
		}

		else
		{
			memo[index] = access[n];
			index++;
		}
}

void testLRU()
{
	int i;
	initMemo();
	printf("---------------最近最久未使用算法---------------\n");

	for (i = 0; i < 32; i++)
	{
		LRU(i);
		printf("%d %d %d\n", memo[0], memo[1], memo[2]);
	}

	printf("最近最久未使用缺页率： %2f %d \n\n\n", lost / 32.0, lost);
	lost = 0;
	index = 0;
	free(memo);
}

//改进型Clock置换算法
void updated_Clock(int n)
{
	if (isInNodes(n))
	{
		printf("---------------已经装入内存---------------\n");
	}

	else
		if (index == block)
		{
			lost++;
			int i = 0, tag = -1;

			while (true)
			{
				if ((i / block) % 2 == 0)
				{
					if (nodes[i % block].flag == 0 && nodes[i % block].modify == 0)
					{
						tag = i % block;
						break;
					}
				}

				if ((i / block) % 2 == 1)
				{
					if (nodes[i % block].flag == 0 && nodes[i % block].modify == 1)
					{
						tag = i % block;
						break;
					}

					else
					{
						nodes[i % block].flag = 0;
					}
				}

				i++;
			}

			nodes[tag].data = access[n];
			nodes[tag].flag = 1;

			if (rand() % 10 < 4)
			{
				nodes[tag].modify = 1;
			}

			else
			{
				nodes[tag].modify = 0;
			}
		}

		else
		{
			nodes[index].data = access[n];
			nodes[index].flag = 1;

			if (rand() % 10 < 4)
			{
				nodes[index].modify = 1;
			}

			else
			{
				nodes[index].modify = 0;
			}

			index++;
		}
}
void test_Clock()
{
	int i = 0, j = 0;
	printf("---------------改进型Clock置换算法---------------\n");
	nodes = (LNode*)malloc(block * sizeof(LNode));

	for (i = 0; i < block; i++)
	{
		nodes[i].data = -1;
		nodes[i].flag = -1;
		nodes[i].modify = -1;
	}

	for (i = 0; i < 32; i++)
	{
		updated_Clock(i);

		for (j = 0; j < block; j++)
		{
			printf("%d ", nodes[j].data);
		}

		printf("\n");
	}

	printf("改进型Clock置换算法缺页率： %2f %d \n\n\n", lost / 32.0, lost);
	lost = 0;
	index = 0;
}

//页面缓冲置换算法
LNode* isinLinks(int n)
{
	LNode*p, *q;
	p = idle.next;
	q = NULL;

	while (p)
	{
		if (p->data == access[n])
		{
			if (q != NULL)
			{
				q->next = p->next;
				p->next = NULL;
				idle.num--;
				break;
			}

			else
			{
				idle.next = NULL;
			}
		}

		q = p;
		p = p->next;
	}

	if (p == NULL)
	{
		p = modified.next;

		while (p != NULL)
		{
			if (p->data == access[n])
			{
				if (p == modified.next)
				{
					modified.next = p->next;
				}

				else
				{
					q->next = p->next;
					p->next = NULL;
					modified.num--;
				}

				if (modified.num == 0)
				{
					modified.next = NULL;
				}

				break;
			}

			q = p;
			p = p->next;
		}
	}

	return p;
}

void addToLink(int data, int type)
{
	LNode* p;
	LNode* q;
	q = (LNode*)malloc(sizeof(LNode));
	q->data = data;
	q->flag = 1;

	if (type == 1)
	{
		q->modify = 1;
		p = modified.next;
	}

	else
	{
		q->modify = 0;
		p = idle.next;
	}

	q->next = NULL;

	if (p == NULL)
	{
		if (type == 0)
		{
			idle.next = q;
		}

		else
		{
			modified.next = q;
		}
	}

	else
	{
		while (p)
		{
			if (p->next == NULL)
			{
				p->next = q;
				break;
			}

			else
			{
				p = p->next;
			}
		}
	}

	if (type == 0)
	{
		idle.num += 1;

		if (idle.num == 10)
		{
			emptyIdle();
		}
	}

	else
	{
		modified.num += 1;

		if (modified.num == 10)
		{
			emptyModi();
		}
	}
}
void emptyIdle()
{
	LNode* p;
	p = idle.next;

	while (p)
	{
		idle.next = p->next;
		free(p);
		p = idle.next;
	}

	idle.num = 0;
}
void emptyModi()
{
	LNode* p;
	p = modified.next;

	while (p)
	{
		modified.next = p->next;
		free(p);
		p = modified.next;
	}

	modified.num = 0;
}

void PBA(int n)
{
	if (isInNodes(n))
	{
		printf("---------------已装入内存---------------\n");
	}

	else
		if (index == size)
		{
			LNode *p;

			if ((p = isinLinks(n)) != NULL)
			{
				nodes = (LNode*)realloc(nodes, (size + 1) * sizeof(LNode));
				nodes[size].data = p->data;
				nodes[size].flag = p->flag;
				nodes[size].modify = p->modify;
				nodes[size].next = p->next;
				free(p);
				size++;
				index++;
			}

			else
			{
				lost++;//缺页

				if (nodes[n % 3].modify == 1)
				{
					addToLink(nodes[n % 3].data, 1);
				}

				else
				{
					addToLink(nodes[n % 3].data, 0);
				}

				nodes[n % 3].data = access[n];
				nodes[n % 3].flag = 1;
				nodes[n % 3].next = NULL;

				if (rand() % 10 < 4)
				{
					nodes[n % 3].modify = 0;
				}

				else
				{
					nodes[n % 3].modify = 1;
				}
			}
		}

		else
		{
			nodes[index].data = access[n];
			nodes[index].flag = 1;
			nodes[index].next = NULL;

			if (rand() % 10 < 4)
			{
				nodes[index].modify = 1;
			}

			else
			{
				nodes[index].modify = 0;
			}

			index++;
		}
}

void test_PBA()
{
	int i = 0, j = 0;
	printf("---------------页面缓冲置换算法---------------\n");
	idle.num = 0;
	idle.next = NULL;
	modified.num = 0;
	modified.next = NULL;
	nodes = (LNode*)malloc(size * sizeof(LNode));

	for (i = 0; i < size; i++)
	{
		nodes[i].data = -1;
		nodes[i].flag = 0;
		nodes[i].modify = 0;
		nodes[i].next = NULL;
	}

	for (i = 0; i < 32; i++)
	{
		PBA(i);

		for (j = 0; j < size; j++)
		{
			printf("%d ", nodes[j].data);
		}

		printf("\n");
	}

	printf("页面缓冲置换算法（PBA）缺页率：%f   %d\n\n\n", lost / 32.0, lost);
}

int main(int argc, char* argv[])
{
	generate();
	testOptimal();
	fifoTest();
	testLRU();
	test_Clock();
	test_PBA();
	int i = 0;

	for (; i < 32; i++)
	{
		printf("%d, ", access[i]);
	}

	getchar();
	getchar();
	return 0;
}