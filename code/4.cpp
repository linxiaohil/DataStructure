#include<cstdio>
#include<cstring>
#include<cstdlib> 
#include<iostream>
#include<vector>
#include<stack>
#include<queue>

using namespace std;



#define CharCount 300

// λ���������ݸ�ʽ 
#define dataType unsigned int
#define dataLen 32

typedef struct {
	int count; //�ַ����ֵĴ���
	dataType code;
	char codeLen;
}Char;

typedef struct
{
	char ch;
	int weight;
	int parent;
	int lchild, rchild;
}HuffmanTree;

FILE* fp;
char fileName[100];

//���ļ�
void fileOpen(const char* method)
{
	fp = fopen(fileName, method);
	if (fp == NULL) {
		printf("���ļ�ʧ��\n");
		return;
	}
}

void fileClose()
{
	if (fclose(fp) != 0) printf("�ر��ļ�ʧ��\n");
	return;
}

void CreateHuffmanTree(HuffmanTree*& hTree, int n, int m)
{
	// ����n-1����������Сֵ
	for (int i = 1; i <= n - 1; i++)
	{
		int fMin = INT_MAX, sMin = INT_MAX - 1;
		int fMinP = 0, sMinP = 0; 
		int j = 1;
		//����
		while (j < m && hTree[j].weight) {
			//����ýڵ�û��˫��,��ȨֵС�ڵڶ�С��Ȩֵ
			if ((!hTree[j].parent) && (hTree[j].weight < sMin)) {
				if (hTree[j].weight < fMin) {
					sMin = fMin;
					sMinP = fMinP;
					fMin = hTree[j].weight;
					fMinP = j;
				}
				else {
					sMin = hTree[j].weight;
					sMinP = j;
				}
			}
			j++;
		}
		if (j < m) {
			hTree[fMinP].parent = j;
			hTree[sMinP].parent = j;

			hTree[j].lchild = fMinP;
			hTree[j].rchild = sMinP;
			hTree[j].weight = fMin + sMin;
		}
	}
}
//����x�������n����λ��ֵ
void SetBit(dataType& x, int n, int bo)
{
	if (bo)
		x = x | 1U << n;
	else x = x & ~(1U << n);
}

//��ȡ����λ
dataType GetBit(dataType x, int n, int m)
{
	return (x & ~(~0U << (m - n + 1)) << n) >> n;
}

//��01�ַ�����ʽ�Ķ�����ת��������
void getCode(stack<char> hCode, dataType& code, char& codeLen)
{
	codeLen = hCode.size();
	if (codeLen > dataLen)
	{
		return;
	}

	//�ַ��е�ջ���Ƕ����Ƶĸ�λ
	int i = codeLen - 1;
	code = 0;
	while (!hCode.empty()) {
		char bitCh = hCode.top();
		printf("%c", bitCh);
		SetBit(code, i, bitCh - '0');
		hCode.pop();
		i--;
	}
}
//������ת�����ַ���ʽ��01���Ķ�����
stack<char> setCode(dataType code, char codeLen)
{
	stack<char> hCode;
	int i = 0;
	while (i < codeLen) {
		hCode.push(GetBit(code, i, i) + '0');
		i++;
	}
	return hCode;
}

//����������
void HuffmanEnCode(HuffmanTree* hTree, int n, int m, Char* chs)
{
	vector<stack<char>> hCodes;
	stack<char> temp;
	for (int i = 1; i <= n; i++) {
		int son = i;
		int parent = hTree[son].parent;
		while (parent) {
			if (hTree[parent].lchild == son) temp.push('0');
			else temp.push('1');
			son = parent;
			parent = hTree[son].parent;
		}
		hCodes.push_back(temp);
		while (!temp.empty()) temp.pop();
	}

	printf("\n=============Huffman������=================\n");
	for (int i = 0; i < hCodes.size(); i++) {
		printf("�ַ���%c----Ȩ�أ�%d----���룺", hTree[i + 1].ch, hTree[i + 1].weight);
		getCode(hCodes[i], chs[hTree[i + 1].ch].code, chs[hTree[i + 1].ch].codeLen);
		printf("--->%d(*len:%d\n", chs[hTree[i + 1].ch].code, chs[hTree[i + 1].ch].codeLen);
	}
	printf("\n");
}

char HuffmanDeCode(HuffmanTree* hTree, int n, int m, stack<char>hCode)
{
	char ch, top;
	int pos = m - 1;
	while (!hCode.empty()) {
		top = hCode.top();
		hCode.pop();
		if (top == '0') pos = hTree[pos].lchild;
		else pos = hTree[pos].rchild;
	}
	if (hTree[pos].lchild || hTree[pos].rchild)	return 0;
	return hTree[pos].ch;
}

int main()
{
	Char chs[256];
	for (int i = 0; i < 256; i++) {
		chs[i].count = 0;
	}
	//��ȡ�ı��ļ�����ͳ�Ƴ��ִ���
	strcpy(fileName, "source.txt");
	fileOpen("r");
	char source[10000];
	int count = 0;
	int n = 0;
	while (!feof(fp)) {
		fscanf(fp, "%c", &source[count]);
		printf("%c", source[count]);
		if (chs[source[count]].count == 0) n++;
		chs[source[count]].count++;
		count++;
	}
	fileClose();

	//��ʼ��Huffman��
	int m = 2 * n;
	HuffmanTree* hTree = (HuffmanTree*)malloc(sizeof(HuffmanTree) * m);
	for (int i = 1; i < m; i++) {
		hTree[i].weight = hTree[i].parent = hTree[i].lchild = hTree[i].rchild = 0;
	}
	//��Huffmans�����Ա��д�1��nλ�ø�ֵ
	int j = 1;
	for (int i = 0; i < 256; i++) {
		if (chs[i].count) {
			hTree[j].parent = hTree[j].lchild = hTree[j].rchild = 0;
			hTree[j].weight = chs[i].count;
			hTree[j].ch = i;
			j++;
		}
	}

	CreateHuffmanTree(hTree, n, m);
	HuffmanEnCode(hTree, n, m, chs);

	//���ַ����ִ����Ͷ�Ӧ�Ķ����Ʊ��뱣�浽�ļ�
	strcpy(fileName, "Huffman.txt");
	fileOpen("w");
	stack<char> hCode;
	for (int i = 0; i < 256; i++) {
		if (chs[i].count) {
			fprintf(fp, "�ַ���%c---���ִ�����%d---�����Ʊ��룺", i, chs[i].count);
			stack<char> hCode = setCode(chs[i].code, chs[i].codeLen);
			while (!hCode.empty()) {
				fprintf(fp, "%c", hCode.top());
				hCode.pop();
			}
			fprintf(fp, "\n");
		}
	}
	fileClose();

	//���ı����б��벢������������ļ�
	strcpy(fileName, "code.dat");
	fileOpen("wb");
	queue<Char> q; //����
	for (int i = 0; i < count;) {
		int lenSum = 0;
		//ȡ����ַ������Ƕ����Ʊ��볤�ȼ�����С��dataType���ݶ����Ʊ��볤��
		while (1) {
			lenSum += chs[source[i]].codeLen;
			if (lenSum > dataLen) break;
			q.push(chs[source[i]]);
			i++;
		}
		//�������ַ������Ķ��������ݣ���dataType��λ��ʼ���
		dataType data = 0;
		int pos = dataLen - 1; //posָ��Ҫ�����ļ����ݵĸ�λ
		while (!q.empty()) {
			Char temp = q.front();
			q.pop();
			int j = temp.codeLen - 1, bo = 0;
			while (j >= 0) {
				bo = GetBit(temp.code, j, j); //��ȡ��λ�õĶ�������
				SetBit(data, pos, bo);
				j--;
				pos--;
			}

		}
		fwrite(&data, sizeof(dataType), 1, fp);
	}
	fileClose();

	//��ȡ�������ļ�������
	strcpy(fileName, "code.dat");
	fileOpen("rb");
	char recode[10000];
	int i = 0;
	dataType data;
	//ÿ�δ��ļ��ж�ȡһ��dataType�������͵�����
	while (fread(&data, sizeof(dataType), 1, fp)) {
		//�Ӹ�λ��ʼ
		int pos = dataLen - 1, j = 0;
		while (pos - j >= 0) {
			dataType temp = GetBit(data, pos - j, pos);
			char res = HuffmanDeCode(hTree, n, m, setCode(temp, j + 1));
			if (res) {
				recode[i] = res;
				i++;
				printf("%c", res);
				pos -= (j + 1);
				j = 0;
				continue;
			}
			j++;
		}
	}
	fileClose();

	strcpy(fileName, "recode.txt");
	fileOpen("w");
	for (int i = 0; i < count; i++) {
		fprintf(fp, "%c", recode[i]);
	}
	fileClose();
	free(hTree);
}
