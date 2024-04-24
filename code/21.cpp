#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<windows.h>

using namespace std; 

typedef struct TNode {
	char key[100];
	char value[100];
	TNode *child;
	TNode *brother;
}*Tree;

// ȫ�ֱ��� 
FILE *fp;
char fileName[] = "input.txt";


// ���ļ� 
void fileOpen(char *method) {
	if((fp=fopen(fileName,method)) == NULL) {
        printf("file cannot be opened\n");
        return;
    }
    //else printf("file has been opened\n");
}

// �ر��ļ� 
void fileClose() {
	if(fclose(fp)!=0) printf("file cannot be closed\n");
    //else printf("file has been closed\n");
    return;
}

// �������
void TraverseTree(Tree tree) {
	if (tree) {
		printf("%s:%s\n", tree->key, tree->value);
		TraverseTree(tree->brother);
		TraverseTree(tree->child);
	}
}

// ����
void DestroyTree(Tree &tree) {
	if (tree) {
		DestroyTree(tree->brother);
		DestroyTree(tree->child);
		free(tree);
	}
}

// �ַ���������
void HandleStr(char *source, int start, int end, char *dist) {

	int i = start+1;
	int j = 0;
	while (i != end) {
		if (source[i] == '\\') i++;
		dist[j] = source[i];
		i++;
		j++;
	}
	dist[j] = '\0';
}



// �����������ݹ� 
void CreateTree(Tree &tree) {
	char str[1000]; 
	fscanf(fp, "%s", str);		// ��ȡ'{'����һ�е��ַ��� 
	int i = 0;
			
	TNode *preNode = tree, *node;
	bool isFirstNode = true;	// �ж��Ƿ�Ϊ���ڵ� 
	
	while (str[i] != '}') {
		node = (TNode*)malloc(sizeof(TNode));
		
		printf("ǰһ��======%s\n", str);
		if (str[i] == '"') {
			int j = i;
			while (str[j] != ':') j++;
			HandleStr(str, i, j-1, node->key);
			i = j;	// iλ��Ϊ':' 
		}
		i++;// ����':'��value���� 
		
		printf("��һ��=====%s\n", str);
		if (str[i] == '{') {
			strcpy(node->value, "object");
			CreateTree(node->child);	// ֵ��һ��������ݹ���������� 
		}
		else if (str[i] == '"') {
			node->child = NULL; 
			int j = i;
			// j<str�ַ���������Ϊ�˽����ַ�'}'ǰû��','����� 
			while (str[j] != ',' && str[j] != '}' && j < strlen(str)) j++;
			HandleStr(str, i, j-1, node->value);
			i = j;		// iλ��Ϊ','��'}' 
		}
		
		// Ϊ��һ�����ʱ��node���ָ�븳��treeָ�� 
		if (isFirstNode) {
			tree = node;
			isFirstNode = false;
		}
		// ���ǵ�һ����㣬���ϸ������ֵ�ָ��ָ��ǰ��� 
		else preNode->brother = node;
		preNode = node;
	
		fscanf(fp, "%s", str);
		i = 0;
	}
	node->brother = NULL;
}
 
// ��valueֵ
char* FindValue(Tree tree, char *str) {
	int i = 0;
	char key[1000];
	while (str[i]!='.' && i<strlen(str)) {
		key[i] = str[i];
		i++;
	}
	key[i] = '\0';
	
	TNode *node = tree;
	while (node) {
		if (strcmp(node->key, key) == 0) {
			if (str[i] == '.') {
				int j = 0;
				char nextStr[1000];
				i++;
				while (i<strlen(str)) {
					nextStr[j] = str[i];
					i++;
					j++;
				}
				nextStr[j] = '\0';
				return FindValue(node->child, nextStr);
			}
			else return node->value;
		}
		node = node->brother;
	}
	return NULL;
} 
// ��ʾ�� 
void ShowTree(Tree tree, int tab) {	
	TNode *node = tree;
	while (node) {
		for (int i=0; i<tab; i++) printf("|  ");
		printf("|--%s", node->key);
		if (node->child) {
			printf("\n");
			ShowTree(node->child, tab+1);
		}
		else printf(":%s\n", node->value);
		node = node->brother;
	}
} 

int main() {
	fileOpen("r");
	int n = 0, m = 0;
	fscanf(fp, "%d%d", &n, &m);
	
	Tree tree = NULL;
	char strLine[10000];
	fscanf(fp, "%s", strLine);// ��ȡ��ʼ��'{'
	CreateTree(tree);
	
	system("cls");	// ���� 
	cout<<"Json���Ľṹ����ͼ��ʾ"<<endl;
	printf("+\n");
	ShowTree(tree, 0);

	cout<<"\n\n\n\n\n���ؽ��:"<<endl;
	// ��ȡ�������� 
	for (int i=0; i<m; i++) {
		fscanf(fp, "%s", strLine);
		char *ret = FindValue(tree, strLine);
		char value[1000];
		if (!ret) sprintf(value, "NOTEXIST");
		else if (strcmp(ret, "object") == 0) sprintf(value, "OBJECT");
		else sprintf(value, "STRING %s", ret);
		printf("%s\n", value);
	}
	
	DestroyTree(tree);
	system("pause"); 
	fileClose();
}


