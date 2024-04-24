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

#define m 3 					//B-���Ľף�����Ϊ3
#define n 17					//�ؼ��ָ���������Ϊ17
typedef int KeyType;
typedef int Status;

typedef struct BTNode
{
	int keynum;					//����йؼ��ָ���
	struct BTNode *parent;		//ָ��˫�׽��
	KeyType key[m + 1];			//�ؼ���������0�ŵ�λԪδ��
	struct BTNode *ptr[m + 1];	//����ָ������
}BTNode, *BTree;				//B-������B-��������

typedef struct
{
	BTNode *pt;					//ָ���ҵ��Ľ��
	int i;						//1...m,�ڽ���еĹؼ������
	int tag;					//1:���ҳɹ���0������ʧ��
}Result;						//B-���Ĳ��ҽ������

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
//����һ������ 
{
	Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q->front)exit(OVERFLOW);
	Q->front->next = NULL;
	return OK;
}

Status EnQueue(LinkQueue *Q, BTree e)
//��� 
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
//���� 
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
//�ж��Ƿ�ӿ� 
{
	if (Q.front == Q.rear)
		return OK;
	else 
		return ERROR;
}

int Search(BTree p, KeyType K)
//��p->key[1...keynum]�в���i��ʹ��p->key[i]<=k<p->key[i+1]
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
//��m��B-��T�ϲ��ҹؼ���K�����ؽ����pt��i��tag��.
//�����ҳɹ���������ֵtag=1��ָ��pt��ָ����е�i���ؼ��ֵ���K����������ֵtag=0������K�Ĺؼ���Ӧ������ָ��pt��ָ����е�i�͵�i+1���ؼ���֮��
{
	BTree p = T, q = NULL;
	int found = FALSE;
	Result r;
	int i = 0;

	while (p && (!found))
	{
		i = Search(p, K);						//��p->key[1..keynum]�в���iʹ�ã�p->key[i]<=k<p->key[i+1]
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
	//���ҳɹ�
	{ 
		r.pt = p; 
		r.tag = 1; 
	}			
	else 
	//���Ҳ��ɹ�������K�Ĳ���λ����Ϣ
	{ 
		r.pt = q; 
		r.tag = 0; 
	}				
	return r;
}

Status NewRoot(BTree &T,KeyType x,BTree ap)
//���ɺ���Ϣ��T��x��ap�����µĸ����*T��ԭT��apΪ����ָ��
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
//��x��ap�ֱ���뵽q->key[i+1]��q->ptr[i+1]
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
//��q->key[s+1..m]��q->ptr[s..m]�����½��*ap
{
	int i, j;
	ap = (BTree)malloc(sizeof(BTNode));			//�����µĽ��
	ap->ptr[0] = q->ptr[s];						//ԭ������м�λ�ùؼ�����Ӧָ��ָ��������ŵ������ɽ���0��������ȥ
	
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
//��m��B-��T��*q��key[i]��key[i+1]֮�����ؼ���K
//����������޶���λ�ã���Խ����з��Ѵ���
{
	KeyType x = K;
	int finished = FALSE;
	BTree ap = NULL;
	int s; 

	while ( q && !finished )
	{
		Insert(q, i, x, ap);					//��x��ap�ֱ���뵽q->key[i+1]��q->ptr[i+1]
		
		if (q->keynum < m)                      //�������
			finished = TURE;		
		else
		//�ڵ��޶���λ�ã���Ҫ����                              		//���ѽ��*q
		{
			//��q->key[s+1..m]��q->ptr[s..m]�����½��*ap
			s = (m + 1) / 2;
			split(q, s, ap);
			x = q->key[s];
			q = q->parent;
			if (q)
			{
				i = Search(q, x);				//��˫�׽��*q�в���x�Ĳ���λ��
			}
		}
	}
	if (!finished)								//T�ǿ���������q��ֵΪNULL�����߸�����ѷ���Ϊ���*q��*ap
		NewRoot(T, x, ap);						//�����µĸ����*T��ԭT��apΪ����ָ��
	return OK;
}

static Status MergeBTree(BTree &T, BTree node);

static Status Merge_BTree(BTree &T, BTree left, BTree right, int mid)
//�ݹ����MergeBTree���ϲ����
{
	int x = 0;
	BTree parent = left->parent;

	//�ϲ������ֵܽ��
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
//ɾ��ĳ�ؼ��ֺ����������ݣ�������н��ϲ�����
{
	int i = 0, mid = 0;
	BTree parent = node->parent, right = NULL, left = NULL;

	//node�Ǹ����, ���ؽ��кϲ����� 
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

	//����nodeΪparent�ĵڼ������
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

		//�ϲ���㣬�������ڵ���һ�������ڹؼ���ʱ
		if ((node->keynum + left->keynum) < (m - 1))
		{
			return Merge_BTree(T, left, node, mid);
		}

		//���ý��left->key[keynum]
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

	//�ϲ����
	if ((node->keynum + right->keynum) < (m - 1))
	{
		return Merge_BTree(T, node, right, mid);
	}

	//���ý��right->ptr[1]
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
//��m��B-��T���ҵ���ɾ���ؼ���K
//��ɾ�������������ݣ�����˫�������н��ϲ�����
{
	BTree child = node->ptr[i - 1], orig = node;

	//ʹ��node->ptr[i-1]�����ֵ�����ɾ���Ĺؼ���
	while (child != NULL)
	{
		node = child;
		child = node->ptr[child->keynum];
	}
	orig->key[i] = node->key[node->keynum];

	//������ײ㱻ɾ���Ĺؼ���
	node->key[node->keynum--] = 0;
	
	//��ɾ�������������ݣ�����˫�������н��ϲ�����
	if (node->keynum <= (m / 2 - 1))
	{
		MergeBTree(T, node);
	}
	return OK;
}

Status TraverseBTree(BTree T, LinkQueue Q, int newline, int sum)
//���ؼ��ֵ�˳���DT��ÿ�������ú���Visit()һ��������һ��(��α���)
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
			printf("������ؼ�����%d��:\n", n - i -1);
			for (int j = i + 1; j < n; j++)
				printf("%d ", K[j]);
			printf("\n");
			printf("\n");
			printf("����ؼ���%d:", K[i]);
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
		cout << "���ļ�ʧ��!";
		exit(0);
	}

    KeyType key[n];
	for(int i = 0; !feof(fp); i++)
		fscanf(fp, "%d", &key[i]);

	fclose(fp);
	
	InputKey(T, key);						//����B-��
	
	KeyType k[4] = { 12, 80, 5, 37 };		
	for (int i = 0; i < 4; i++)             //���ҹؼ���
	{
		system("cls");
		printf("%d��B-��Ϊ:", m);
		printf("\n");
		print(T);
		printf("\n");
		printf("���ҹؼ���%d:", k[i]);
		getchar();
		s = SearchBTree(T, k[i]);
		if (s.tag)
		{
			printf("���ҳɹ�!\n");
		}
		else 
		{
			printf("����ʧ��!\n");
		}
		getchar();
	}
	
	for (int i = 0; i < n; i++)             //ɾ��B-��
	{
		system("cls");
		printf("%d��B-��Ϊ:", m);
		printf("\n");
		print(T);
		printf("\n");
		printf("ɾ���ؼ���%d\n",key[i]);
		DeleteKey(T, key[i]);
		print(T);
		_getch();
	}
	return 0;
}
