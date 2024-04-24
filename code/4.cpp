#include<cstdio>
#include<cstring>
#include<cstdlib> 
#include<iostream>
#include<vector>
#include<stack>
#include<queue>

using namespace std;



#define CharCount 300

// 位操作的数据格式 
#define dataType unsigned int
#define dataLen 32

typedef struct {
	int count; //字符出现的次数
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

//打开文件
void fileOpen(const char* method)
{
	fp = fopen(fileName, method);
	if (fp == NULL) {
		printf("打开文件失败\n");
		return;
	}
}

void fileClose()
{
	if (fclose(fp) != 0) printf("关闭文件失败\n");
	return;
}

void CreateHuffmanTree(HuffmanTree*& hTree, int n, int m)
{
	// 经过n-1次找两个最小值
	for (int i = 1; i <= n - 1; i++)
	{
		int fMin = INT_MAX, sMin = INT_MAX - 1;
		int fMinP = 0, sMinP = 0; 
		int j = 1;
		//遍历
		while (j < m && hTree[j].weight) {
			//如果该节点没有双亲,且权值小于第二小的权值
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
//设置x这个数第n比特位的值
void SetBit(dataType& x, int n, int bo)
{
	if (bo)
		x = x | 1U << n;
	else x = x & ~(1U << n);
}

//获取比特位
dataType GetBit(dataType x, int n, int m)
{
	return (x & ~(~0U << (m - n + 1)) << n) >> n;
}

//将01字符串形式的二进制转化成整数
void getCode(stack<char> hCode, dataType& code, char& codeLen)
{
	codeLen = hCode.size();
	if (codeLen > dataLen)
	{
		return;
	}

	//字符中的栈顶是二进制的高位
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
//将整数转化成字符形式的01串的二进制
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

//哈夫曼编码
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

	printf("\n=============Huffman编码结果=================\n");
	for (int i = 0; i < hCodes.size(); i++) {
		printf("字符：%c----权重：%d----编码：", hTree[i + 1].ch, hTree[i + 1].weight);
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
	//读取文本文件，并统计出现次数
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

	//初始化Huffman树
	int m = 2 * n;
	HuffmanTree* hTree = (HuffmanTree*)malloc(sizeof(HuffmanTree) * m);
	for (int i = 1; i < m; i++) {
		hTree[i].weight = hTree[i].parent = hTree[i].lchild = hTree[i].rchild = 0;
	}
	//将Huffmans树线性表中从1到n位置赋值
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

	//将字符出现次数和对应的二进制编码保存到文件
	strcpy(fileName, "Huffman.txt");
	fileOpen("w");
	stack<char> hCode;
	for (int i = 0; i < 256; i++) {
		if (chs[i].count) {
			fprintf(fp, "字符：%c---出现次数：%d---二进制编码：", i, chs[i].count);
			stack<char> hCode = setCode(chs[i].code, chs[i].codeLen);
			while (!hCode.empty()) {
				fprintf(fp, "%c", hCode.top());
				hCode.pop();
			}
			fprintf(fp, "\n");
		}
	}
	fileClose();

	//对文本进行编码并输出到二进制文件
	strcpy(fileName, "code.dat");
	fileOpen("wb");
	queue<Char> q; //队列
	for (int i = 0; i < count;) {
		int lenSum = 0;
		//取多个字符，他们二进制编码长度加起来小于dataType数据二进制编码长度
		while (1) {
			lenSum += chs[source[i]].codeLen;
			if (lenSum > dataLen) break;
			q.push(chs[source[i]]);
			i++;
		}
		//将这多个字符编码后的二进制数据，从dataType高位开始存放
		dataType data = 0;
		int pos = dataLen - 1; //pos指向要存入文件数据的高位
		while (!q.empty()) {
			Char temp = q.front();
			q.pop();
			int j = temp.codeLen - 1, bo = 0;
			while (j >= 0) {
				bo = GetBit(temp.code, j, j); //获取该位置的二进制数
				SetBit(data, pos, bo);
				j--;
				pos--;
			}

		}
		fwrite(&data, sizeof(dataType), 1, fp);
	}
	fileClose();

	//读取二进制文件并解码
	strcpy(fileName, "code.dat");
	fileOpen("rb");
	char recode[10000];
	int i = 0;
	dataType data;
	//每次从文件中读取一个dataType数据类型的数据
	while (fread(&data, sizeof(dataType), 1, fp)) {
		//从高位开始
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
