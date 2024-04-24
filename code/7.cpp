#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

using namespace std;

#define TURE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASSIBLE -1
#define OVERFLOW -2
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

#define m 3 					//B-树的阶，暂设为3
#define n 17					//关键字个数，暂设为17
typedef int KeyType;
typedef int Status;

typedef struct BTNode
{
	int keynum;					//结点中关键字个数
	struct BTNode *parent;		//指向双亲结点
	KeyType key[m + 1];			//关键字向量，0号单位元未用
	struct BTNode *ptr[m + 1];	//子树指针向量
}BTNode, *BTree;				//B-树结点和B-树的类型

typedef struct
{
	BTNode *pt;					//指向找到的结点
	int i;						//1...m,在结点中的关键字序号
	int tag;					//1:查找成功；0：查找失败
}Result;						//B-树的查找结果类型

typedef struct QNode
{
	BTree data;
	struct QNode *next;
}QNode, *QueuePtr;

typedef struct
{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

Status InitQueue(LinkQueue *Q)
//创建一个队列 
{
	Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q->front)exit(OVERFLOW);
	Q->front->next = NULL;
	return OK;
}

Status EnQueue(LinkQueue *Q, BTree e)
//入队 
{
	QueuePtr p;
	p = (QueuePtr)malloc(sizeof(QNode));
	if (!p)
	{
		exit(OVERFLOW);
	}
	p->data = e;
	p->next = NULL;
	Q->rear->next = p;
	Q->rear = p;
	return OK;
}

Status DeQueue(LinkQueue *Q, BTree *e)
//出队 
{
	QueuePtr p;
	if (Q->front == Q->rear)
	{
		return ERROR;	
	}
	p = Q->front->next;
	*e = p->data;
	Q->front->next = p->next;
	if (Q->rear == p)
	{
		Q->rear = Q->front;
	}
	free(p);
	return OK;
}

Status QueueEmpty(LinkQueue Q)
//判断是否队空 
{
	if (Q.front == Q.rear)
		return OK;
	else 
		return ERROR;
}

int Search(BTree p, KeyType K)
//在p->key[1...keynum]中查找i，使得p->key[i]<=k<p->key[i+1]
{
	int i, j = 0; 
	for (i = 1; i <= p->keynum; i++)
	{
		if (p->key[i] <= K)
		{
			j = i;
		}
	}
	return j;
}
 
Result SearchBTree(BTree T, KeyType K)
//在m阶B-树T上查找关键字K，返回结果（pt，i，tag）.
//若查找成功，则特征值tag=1，指针pt所指结点中第i个关键字等于K；否则特征值tag=0，等于K的关键字应插入在指针pt所指结点中第i和第i+1个关键字之间
{
	BTree p = T, q = NULL;
	int found = FALSE;
	Result r;
	int i = 0;

	while (p && (!found))
	{
		i = Search(p, K);						//在p->key[1..keynum]中查找i使得：p->key[i]<=k<p->key[i+1]
		if ((i > 0) && (p->key[i] == K))
		{
			found = TURE;
		}
		else
		{
			q = p;
			p = p->ptr[i];
		}
	}
	r.i = i;
	if (found)
	//查找成功
	{ 
		r.pt = p; 
		r.tag = 1; 
	}			
	else 
	//查找不成功，返回K的插入位置信息
	{ 
		r.pt = q; 
		r.tag = 0; 
	}				
	return r;
}

Status NewRoot(BTree &T,KeyType x,BTree ap)
//生成含信息（T，x，ap）的新的根结点*T，原T和ap为子树指针
{
	BTree p;
	p = (BTree)malloc(sizeof(BTNode));
	p->ptr[0] = T;
	T = p;
	if (T->ptr[0]) 
	{
		T->ptr[0]->parent = T;
	}
	T->parent = NULL;
	T->keynum = 1;
	T->key[1] = x;
	T->ptr[1] = ap;
	if (T->ptr[1])
	{
		T->ptr[1]->parent = T;
	}
	return OK;
}

Status Insert(BTree &q, int i,KeyType x, BTree ap)
//将x和ap分别插入到q->key[i+1]和q->ptr[i+1]
{
	int j;
	for (j = q->keynum ; j > i; --j)
	{
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[i + 1] = x;
	q->ptr[i + 1] = ap;
	q->keynum++; 
	return OK;
}

Status split(BTree &q,int s, BTree &ap)
//将q->key[s+1..m]和q->ptr[s..m]移入新结点*ap
{
	int i, j;
	ap = (BTree)malloc(sizeof(BTNode));			//生成新的结点
	ap->ptr[0] = q->ptr[s];						//原来结点中间位置关键字相应指针指向的子树放到新生成结点的0棵子树中去
	
	for (i = s + 1, j = 1; i <= q->keynum; i++,j++)
	{
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
	}
	ap->keynum = q->keynum - s;
	ap->parent = q->parent;
	for (i = 0; i <= q->keynum - s;i++)
	{
		if (ap->ptr[i])
		{
			ap->ptr[i]->parent = ap;
		}
	}
	q->key[q->keynum] = 0;
	q->keynum = s - 1;
	return OK;
}

Status InsertBTree(BTree &T, KeyType K, BTree q, int i)
//在m阶B-树T上*q的key[i]与key[i+1]之间插入关键字K
//若结点中已无多余位置，则对结点进行分裂处理
{
	KeyType x = K;
	int finished = FALSE;
	BTree ap = NULL;
	int s; 

	while ( q && !finished )
	{
		Insert(q, i, x, ap);					//将x和ap分别插入到q->key[i+1]和q->ptr[i+1]
		
		if (q->keynum < m)                      //插入完成
			finished = TURE;		
		else
		//节点无多余位置，需要分裂                              		//分裂结点*q
		{
			//将q->key[s+1..m]和q->ptr[s..m]移入新结点*ap
			s = (m + 1) / 2;
			split(q, s, ap);
			x = q->key[s];
			q = q->parent;
			if (q)
			{
				i = Search(q, x);				//在双亲结点*q中查找x的插入位置
			}
		}
	}
	if (!finished)								//T是空树（参数q初值为NULL）或者根结点已分裂为结点*q和*ap
		NewRoot(T, x, ap);						//生成新的根结点*T，原T和ap为子树指针
	return OK;
}

static Status MergeBTree(BTree &T, BTree node);

static Status Merge_BTree(BTree &T, BTree left, BTree right, int mid)
//递归调用MergeBTree来合并结点
{
	int x = 0;
	BTree parent = left->parent;

	//合并到左兄弟结点
	left->key[++left->keynum] = parent->key[mid];

	memcpy(left->key + left->keynum + 1, right->key + 1, right->keynum*sizeof(int));
	memcpy(left->ptr + left->keynum, right->ptr, (right->keynum + 1)*sizeof(BTree));

	for (x = 0; x <= right->keynum;x++)
	{
		if (right->ptr[x] != NULL)
		{
			right->ptr[x]->parent = left;
		}
	}

	left->keynum += right->keynum;

	for (x = mid; x < parent->keynum ; x++)
	{
		parent->key[x] = parent->key[x + 1];
		parent->ptr[x] = parent->ptr[x + 1];
	}

	parent->key[x + 1] = 0;
	parent->ptr[x + 1] = NULL;
	parent->keynum--;
	free(right);

	if (parent->keynum <= (m / 2 - 1))
	{
		return MergeBTree(T, parent);
	}

	return OK;
}

static Status MergeBTree(BTree &T, BTree node)
//删除某关键字后结点中无数据，对其进行结点合并调整
{
	int i = 0, mid = 0;
	BTree parent = node->parent, right = NULL, left = NULL;

	//node是根结点, 不必进行合并处理 
	if (parent == NULL)
	{
		if (node->keynum == 0)
		{
			if (node->ptr[0] != NULL)
			{
				T = node->ptr[0];
				node->ptr[0]->parent = NULL;
			}
			else 
			{
				T = NULL;
			}
			free(node);
		}
		return OK;
	}

	//查找node为parent的第几个结点
	for (i = 0; i <= parent->keynum; i++)
	{
		//printf("%d,%d ", i, parent->ptr[0]->key[i]);
		if (parent->ptr[i] == node)break;
	}

	if (i > parent->keynum)						
	{
		return ERROR;
	}
	
	else if (i == parent->keynum)
	{
		mid = i;
		left = parent->ptr[mid - 1];

		//合并结点，当两个节点有一个不存在关键字时
		if ((node->keynum + left->keynum) < (m - 1))
		{
			return Merge_BTree(T, left, node, mid);
		}

		//借用结点left->key[keynum]
		for (int x = node->keynum; x>1; x--)
		{
			node->key[x] = node->key[x - 1];
			node->ptr[x] = node->ptr[x - 1];
		}
		node->ptr[1] = node->ptr[0];

		node->key[1] = parent->key[mid];

		node->keynum++;
		node->ptr[0] = left->ptr[left->keynum];
		if (left->ptr[left->keynum] != NULL)
		{
			left->ptr[left->keynum]->parent = node;
		}

		parent->key[mid] = left->key[left->keynum];
		left->key[left->keynum] = 0;
		left->ptr[left->keynum] = NULL;
		left->keynum--;
		return OK;
	}
	
	mid = i + 1;
	right = parent->ptr[mid];

	//合并结点
	if ((node->keynum + right->keynum) < (m - 1))
	{
		return Merge_BTree(T, node, right, mid);
	}

	//借用结点right->ptr[1]
	node->key[++node->keynum] = parent->key[mid];
	node->ptr[node->keynum] = right->ptr[0];
	if (right->ptr[0] != NULL)
	{
		right->ptr[0]->parent = node;
	}
	
	parent->key[mid] = right->key[1];
	right->ptr[0] = right->ptr[1];
	for (int x = 1; x < right->keynum; x++)
	{
		right->key[x] = right->key[x + 1];
		right->ptr[x] = right->ptr[x + 1];
	}
	right->key[right->keynum] = 0;
	right->ptr[right->keynum--] = NULL;
	return OK;
}

static Status DeleteBTree(BTree &T, BTree node, int i)
//在m阶B-树T上找到并删除关键字K
//若删除后结点中无数据，则沿双亲链进行结点合并调整
{
	BTree child = node->ptr[i - 1], orig = node;

	//使用node->ptr[i-1]中最大值替代被删除的关键字
	while (child != NULL)
	{
		node = child;
		child = node->ptr[child->keynum];
	}
	orig->key[i] = node->key[node->keynum];

	//处理最底层被删除的关键字
	node->key[node->keynum--] = 0;
	
	//若删除后结点中无数据，则沿双亲链进行结点合并调整
	if (node->keynum <= (m / 2 - 1))
	{
		MergeBTree(T, node);
	}
	return OK;
}

Status TraverseBTree(BTree T, LinkQueue Q, int newline, int sum)
//按关键字的顺序对DT的每个结点调用函数Visit()一次且至多一次(层次遍历)
{
	BTree p;
	int i;
	if (T)
	{
		printf("[ ");
		EnQueue(&Q, T->ptr[0]);
		for (i = 1; i <= T->keynum; i++)
		{
			printf("%d ", T->key[i]);
			EnQueue(&Q, T->ptr[i]);
		}
		sum += T->keynum + 1;
		printf("]");
		if (newline == 0)
		{
			printf("\n");
			newline = sum - 1;
			sum = 0;
		}
		else
			--newline;
		if (!QueueEmpty(Q))
		{
			DeQueue(&Q, &p);
			TraverseBTree(p, Q, newline, sum);
		}
	}
	return OK;
}

void print(BTree T)
{
	LinkQueue Q;
	if (!T)
	{
		printf("[ ]\n");
		return ;
	}
	InitQueue(&Q);
	TraverseBTree(T, Q, 0, 0);
	return;
}

void InputKey(BTree &T, KeyType K[])
{
	Result s;
	for (int i = 0; i < n; i++)
	{
		s = SearchBTree(T, K[i]); 
		if (!s.tag)
		{
			InsertBTree(T, K[i], s.pt, s.i);
			system("cls");
			printf("待插入关键字有%d个:\n", n - i -1);
			for (int j = i + 1; j < n; j++)
				printf("%d ", K[j]);
			printf("\n");
			printf("\n");
			printf("插入关键字%d:", K[i]);
			printf("\n");
			print(T);
			_getch();
		}		
	}
}

Status DeleteKey(BTree &T, KeyType K)
{
	int i = 0;
	BTree node = T;
	Result s;

	while (NULL != node)
	{
		s = SearchBTree(T, K);
		node = s.pt;
		i = s.i; 
		if (K == node->key[i])
			return DeleteBTree(T, node, i);
		else if(K < node->key[i])
			break;
	}
	return OK;
}

int main()
{
	BTree T = NULL;
	Result s;
	
	FILE *fp;
	fp = fopen("BTree.txt", "r");
	if(!fp)
	{
		cout << "打开文件失败!";
		exit(0);
	}

    KeyType key[n];
	for(int i = 0; !feof(fp); i++)
		fscanf(fp, "%d", &key[i]);

	fclose(fp);
	
	InputKey(T, key);						//生成B-树
	
	KeyType k[4] = { 12, 80, 5, 37 };		
	for (int i = 0; i < 4; i++)             //查找关键字
	{
		system("cls");
		printf("%d阶B-树为:", m);
		printf("\n");
		print(T);
		printf("\n");
		printf("查找关键字%d:", k[i]);
		getchar();
		s = SearchBTree(T, k[i]);
		if (s.tag)
		{
			printf("查找成功!\n");
		}
		else 
		{
			printf("查找失败!\n");
		}
		getchar();
	}
	
	for (int i = 0; i < n; i++)             //删除B-树
	{
		system("cls");
		printf("%d阶B-树为:", m);
		printf("\n");
		print(T);
		printf("\n");
		printf("删除关键字%d\n",key[i]);
		DeleteKey(T, key[i]);
		print(T);
		_getch();
	}
	return 0;
}
