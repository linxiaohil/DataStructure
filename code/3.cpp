#pragma warning( disable : 4996)
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <time.h>
#include <graphics.h>
using namespace std;
//�������ݵ�����  
typedef struct ElemType {
	char name[30];
	char birth[30];
	char marrige[2];
	char addr[40];
	char isLive[2];
	char deadDay[30];
}ElemType;
//���庢���ֵܶ�����
typedef struct CSNode {
	ElemType data;
	struct CSNode* firstchild;
	struct CSNode* nextsibling;
}CSNode, *CSTree;

typedef struct node
{
	CSTree p;
	int sx;
	int sy;
	int ex;
	int ey;
}widget;

fstream dataFile;

void findFather(CSTree T,char *name, CSTree &fa)
{
	if (T) {
		CSTree son = T->firstchild;
		while (son) {
			if (!strcmp(son->data.name, name)) {
				fa = T;
				return;
			}
			son = son->nextsibling;
		}
		findFather(T->firstchild, name, fa);
		findFather(T->nextsibling, name, fa);
	}
}

//�������ļ����ļ���#��ʾ����
bool OpenFile(string& fileName)
{
	dataFile.open(fileName, ios::in|ios::out);
	if (!dataFile.is_open())
		return false;
	return true;
}
//���������ֵܶ�����
bool CreateCSTree(CSTree& T)
{
	char name[30], birth[30], marrige[2],addr[40],isLive[2],deadDay[30];
	dataFile >> name;
	if (!strcmp(name, "#")) {
		T = NULL;
	}
	else {
		T = (CSTree)malloc(sizeof(CSNode));
		if (!T) {
			cout << "�����ڴ�ʧ��" << endl;
			exit(1);
		}
		dataFile >> birth;
		dataFile >> marrige;
		dataFile >> addr;
		dataFile >> isLive;
		if (!strcmp(isLive, "n") || !strcmp(isLive, "N")) {
			dataFile >> deadDay;
			strcpy(T->data.deadDay, deadDay);
		}
		strcpy(T->data.name, name);
		strcpy(T->data.birth, birth);
		strcpy(T->data.marrige, marrige);
		strcpy(T->data.addr, addr);
		strcpy(T->data.isLive, isLive);
		CreateCSTree(T->firstchild);
		CreateCSTree(T->nextsibling);
	}
	return true;
}
//��ʾ����˵ĸ�����Ϣ
void ShowInformation(CSTree T)
{
	cout << T->data.name << " " << T->data.birth << " " << T->data.marrige << " " << T->data.addr << " " << T->data.isLive ;
	if (!strcmp(T->data.isLive, "n") || !strcmp(T->data.isLive, "N"))
		cout << " " << T->data.deadDay;
	cout << endl;
}
//��ȡ��n������Ϣ
bool GetNGenration(CSTree& T, int n)
{
	if (!T) {
		cout << "����Ϊ��" << endl;
		return false;
	}
	int depth = 1;
	queue<CSTree> q1, q2;
	q1.push(T);
	while (!q1.empty() || !q2.empty()) {
		CSTree temp;
		if (!q1.empty()) {
			while (!q1.empty()) {
				temp = q1.front();
				if (depth == n)				//����ò���Ҫ����Ĳ㣬��� 
					ShowInformation(temp);
				q1.pop();
				if (temp->firstchild) q2.push(temp->firstchild);   //������Ӳ�Ϊ�գ��ͷŵ���һ������ 
				if (temp->nextsibling) {							 //����ֵܲ�Ϊ�գ����ǰ��ֵܷŽ�������У����������ò� 
					q1.push(temp->nextsibling);
				}
			}
		}
		else {
			while (!q2.empty()) {
				temp = q2.front();
				if (depth == n)
					ShowInformation(temp);
				q2.pop();
				if (temp->firstchild) q1.push(temp->firstchild);
				if (temp->nextsibling) {
					q2.push(temp->nextsibling);
				}
			}
		}
		depth++;
	}
	return true;

}
//�������
bool InOrderTraverse(CSTree T)
{
	if (T) {
		ShowInformation(T);
		InOrderTraverse(T->firstchild);
		InOrderTraverse(T->nextsibling);
	}
	return true;
}
//��ȡ������ǵڼ���
int GetDepth(CSTree T, char *name)
{
	if (!T) {
		cout << "����Ϊ��" << endl;
		return 0;
	}

	int depth = 1;
	queue<CSTree> q1, q2;
	q1.push(T);
	while (!q1.empty() || !q2.empty()) {
		CSTree temp;
		if (!q1.empty()) {
			while (!q1.empty()) {
				temp = q1.front();
				if (!strcmp(temp->data.name, name))				//�����Ԫ����ҪѰ�ҵ�Ԫ�� 
					return depth;
				q1.pop();
				if (temp->firstchild) q2.push(temp->firstchild);   //������Ӳ�Ϊ�գ��ͷŵ���һ������ 
				if (temp->nextsibling) {							 //����ֵܲ�Ϊ�գ����ǰ��ֵܷŽ�������У����������ò� 
					q1.push(temp->nextsibling);
				}
			}
		}
		else {
			while (!q2.empty()) {
				temp = q2.front();
				if (!strcmp(temp->data.name, name))				//�����Ԫ����ҪѰ�ҵ�Ԫ�� 
					return depth;
				q2.pop();
				if (temp->firstchild) q1.push(temp->firstchild);
				if (temp->nextsibling) {
					q2.push(temp->nextsibling);
				}
			}
		}
		depth++;
	}
	cout << "û���ҵ�" <<name << endl;
}

//��������˺����ĸ��׶��ӵ���Ϣ
bool SearchInformation(CSTree T, char *name)
{
	if (T) {
		CSTree father;
		CSTree son;
		father = T->firstchild;
		son = T->firstchild;
		if (!strcmp(T->data.name, name)) {
			cout << "�����˵���Ϣ��"<< endl;
			ShowInformation(T);
			if(son)
				cout << "�����ӵ���Ϣ: " << endl;
			while (son) {
				ShowInformation(son);
				son = son->nextsibling;
			}
			return true;
		}
		while (father) {
			if (!strcmp(father->data.name, name)) {
				cout << "�����׵���Ϣ�� "<<endl;
				ShowInformation(T);
			}
			father = father->nextsibling;
		}
		if (SearchInformation(T->firstchild, name) || SearchInformation(T->nextsibling, name))
			return true;
	}
	return false;
}
//�����ղ�ѯ
bool SearchForBirth(CSTree T, char* birth)
{
	if (T) {
		if (!strcmp(T->data.birth, birth)) {
			ShowInformation(T);
			return true;
		}
		if (SearchForBirth(T->firstchild, birth) || SearchForBirth(T->nextsibling, birth))
			return true;
	}
	return false;
}
//ȷ�����˹�ϵ
void DeterRela(CSTree T, char* name1, char* name2)
{
	int depth1 = GetDepth(T, name1);
	int depth2 = GetDepth(T, name2);
	if (depth1 == depth2) {
		cout << name1 << "��" << name2 << "���ֵ�" << endl;
	}
	if (depth1 < depth2) {
		if (depth2 - depth1 >= 2)
			cout << name1 << "��" << name2 << "���游" << endl;
		else if (depth2 - depth1 == 1) {
			cout << name1 << "��" << name2 << "�ĸ���" << endl;
		}
	}
	if (depth1 > depth2) {
		if (depth1 - depth2 >= 2)
			cout << name2 << "��" << name1 << "���游" << endl;
		else if (depth1 - depth2 == 1) {
			cout << name2 << "��" << name1 << "�ĸ���" << endl;
		}
	}
}
//��Ӻ���
int AddChild(CSTree &T, char* name) {
	CSTree p_new;
	if (T) {
		if (!strcmp(T->data.name, name)) {
			p_new = (CSTree)malloc(sizeof(CSNode));
			if (p_new == NULL) {
				cout << "�ڴ�������" << endl;
				exit(1);
			}
			p_new->firstchild = NULL;
			p_new->nextsibling = NULL;
			cout << "������Ҫ��Ӻ��ӵ�����: ";
			cin >> p_new->data.name;
			cout << "������Ҫ��Ӻ��ӵ�����: ";
			cin >> p_new->data.birth;
			cout << "������Ҫ��Ӻ��ӵĻ���״��(y/n)��";
			cin >> p_new->data.marrige;
			cout << "������Ҫ��Ӻ��ӵĵ�ַ��";
			cin >> p_new->data.addr;
			cout << "������Ҫ��ӵĺ��ӵĽ���״��(y/n)��";
			cin >> p_new->data.isLive;
			if (!strcmp(p_new->data.isLive, "n")) {
				cout << "������Ҫ��ӵĺ��ӵ��������ڣ�" << endl;
				cin >> p_new->data.deadDay;
			}
			if (T->firstchild == NULL) {
				T->firstchild = p_new;
				return 1;
			}
			else {
				CSTree anotherSon = T->firstchild;
				while (anotherSon->nextsibling) {
					anotherSon = anotherSon->nextsibling;
				}
				anotherSon->nextsibling = p_new;
				return 1;
			}
		}
		if (AddChild(T->firstchild, name) || AddChild(T->nextsibling, name))
			return 1;
	}
	return 0;
}
void subPrint(CSTree A, widget* data, int* cnt)
{
	//��ջ���
	cleardevice();
	*cnt = 0;
	wchar_t name[20];

	//�����˳���ť
	RECT r_exit = { 535,405,600,440 };
	rectangle(535, 405, 600, 440);
	drawtext(_T("�˳�"), &r_exit, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//�������ذ�ť
	RECT r_return = { 430, 405, 495, 440 };
	rectangle(430, 405, 495, 440);
	drawtext(_T("����"), &r_return, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//�������׽��
	RECT r_pa = { 260,100,380,150 };
	rectangle(260, 100, 380, 150);
	//��Ϣת����utf-8תΪUnicode��charתΪwchar
	int nRet = MultiByteToWideChar(CP_ACP, 0, A->data.name, 20, name, 20);
	drawtext(name, &r_pa, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//���������ӽ��
	CSTree  p = A->firstchild;
	while (p)
	{
		data[*cnt].p = p;
		(*cnt)++;
		p = p->nextsibling;
	}
	int x = (640 - 60 * (*cnt)) / ((*cnt) + 1);
	int i, sx = x;
	for (i = 0; i < *cnt; i++)
	{
		data[i].sx = sx;
		data[i].sy = 270;
		data[i].ex = sx + 60;
		data[i].ey = 300;
		RECT r_son = { sx,270,sx + 60,300 };
		rectangle(sx, 270, sx + 60, 300);
		line(sx + 30, 270, 320, 150);
		int nRet = MultiByteToWideChar(CP_ACP, 0, data[i].p->data.name, 20, name, 20);
		drawtext(name, &r_son, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		sx += x + 60;
	}
}
void Print(CSTree T)
{
	// ���������������
	srand((unsigned)time(NULL));

	// ��ʼ��ͼ��ģʽ
	initgraph(640, 480);

	// ��������
	settextstyle(16, 8, _T("Courier"));

	// ������ɫ
	COLORREF color = RGB(252, 253, 248);
	setbkcolor(color);
	settextcolor(BLACK);
	setlinecolor(BLACK);

	// ����������
	MOUSEMSG m;

	CSTree tree = T;
	CSTree fa = NULL;
	widget data[20];
	int cnt = 0, i;
	subPrint(T, data, &cnt);
	while (1)
	{
		m = GetMouseMsg();
		fa = NULL;
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			//�˳�
			if (m.x <= 600 && m.x >= 535 && m.y >= 405 && m.y <= 440)
			{
				break;
			}
			//����
			if (m.x <= 495 && m.x >= 430 && m.y >= 405 && m.y <= 440)
			{
				findFather(T, tree->data.name, fa);
				if (fa != NULL)
				{
					subPrint(fa, data, &cnt);
					tree = fa;
				}
			}
	
			//ѡ���ӽ��
			for (i = 0; i < cnt; i++)
			{
				if (m.x >= data[i].sx && m.x <= data[i].ex && m.y >= data[i].sy && m.y <= data[i].ey)
				{
					tree = data[i].p;
					subPrint(tree, data, &cnt);
				}
			}
		}
	}

	// �ر�ͼ��ģʽ
	closegraph();

}
//�˵�
void Menue()
{
	cout << "\n==========================================���׹���ϵͳ===================================================\n";
	cout << "                                     1.��ʾn���˵�������Ϣ" << endl;
	cout << "                                     2.����������" << endl;
	cout << "                                     3.�����ղ���" << endl;
	cout << "                                     4.�������˹�ϵ" << endl;
	cout << "                                     5.��Ӻ���" << endl;
	cout << "                                     6.�����޸�" << endl;
	cout << "                                     7.�Ƴ�����" << endl;
	cout << "                                     8.�޸ĳ�Ա��Ϣ" << endl;
	cout << "                                     9.���Ƴ�Ա��Ϣ" << endl;
	cout << "\n========================================================================================================\n";
}
//�����޸�
void SaveChange(CSTree& T) 
{
	if (T) {
		dataFile << T->data.name << " " << T->data.birth << " " << T->data.marrige << " " << T->data.addr << " " << T->data.isLive;
		if (!strcmp("n", T->data.isLive) || !strcmp("N", T->data.isLive))
			dataFile << " " << T->data.deadDay;
		dataFile << endl;
		SaveChange(T->firstchild);
		SaveChange(T->nextsibling);
	}
	else {
		dataFile << "#" << endl;
	}
}
//ɾ����Ա
void Delete(CSTree& T)
{
	CSTree child, sibling;
	if (T) {
		child = T->firstchild;
		sibling = T->nextsibling;
		free(T);
		T = NULL;
		if (child)
			Delete(child);
		if (sibling)
			Delete(sibling);
	}
}
int SearchForDelete(CSTree& T, char *name)
{
	if (T) {
		if (!strcmp(T->data.name, name)) {
			Delete(T->firstchild);
			free(T);
			T = NULL;
			return 1;
		}
		if(SearchForDelete(T->firstchild, name) || SearchForDelete(T->nextsibling, name))
			return 1;
	}
}

//�޸ĳ�Ա��Ϣ
void Change(CSTree& T)
{
	int choice;
	while (1) {
		cout << "������Ҫ�޸���Ϣ�����" << endl;
		cout << "1.����" << endl;
		cout << "2.����" << endl;
		cout << "3.���" << endl;
		cout << "4.��ס��" << endl;
		cout << "5.����" << endl;
		cout << "6.��������" << endl;
		cout << "7.�����޸�" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:
			cout << "������Ҫ�޸ĳɵ�����: " << endl;
			cin >> T->data.name;
			break;
		case 2:
			cout << "������Ҫ�޸ĳɵ����գ�" << endl;
			cin >> T->data.birth;
			break;
		case 3:
			cout << "�������µĻ���״̬��" << endl;
			cin >> T->data.marrige;
		case 4:
			cout << "�������µ�סַ��" << endl;
			cin >> T->data.addr;
		case 5:
			cout << "�������µĽ���״̬��" << endl;
			cin >> T->data.isLive;
		case 6:
			cout << "�������µ��������ڣ�" << endl;
			cin >> T->data.deadDay;
		case 7:
			goto END;
			break;
		default:
			break;
		}
	}
END:return;
}
bool SearchForChange(CSTree& T, char* name)
{
	if (T) {
		if (!strcmp(T->data.name, name)) {
			Change(T);
			return true;
		}
		if (SearchForChange(T->firstchild, name) || SearchForChange(T->nextsibling, name))
			return true;
	}
	return false;
}
int main()
{
	CSTree T = NULL;
	string fileName = "data.txt";
	char str[100];
	int choice;
	if (!OpenFile(fileName)) {
		cout << "�ļ�������" << endl;
		exit(0);
	}
	CreateCSTree(T);
	dataFile.close();
	while (1) {
		Menue();
		cout << "���������ѡ��";
		scanf("%s", str);
		choice = atoi(str);
		switch (choice)
		{
		case 1:
			int n;
			cout << "������Ҫ���ҵڼ�����һ����������";
			cin >> n;
			GetNGenration(T, n);
			break;
		case 2:
			char name[30];
			cout << "������Ҫ���ҵ�������";
			cin >> name;
			if (!SearchInformation(T, name))
				cout << "û���ҵ���Ӧ��Ϣ" << endl;
			break;
		case 3:
			char birth[30];
			cout << "������Ҫ���ҵ����գ�";
			cin >> birth;
			if(!SearchForBirth(T, birth))
				cout << "û���ҵ���Ӧ��Ϣ" << endl;
			break;
		case 4:
			char name1[30], name2[30];
			cout << "������Ҫ��ѯ�����˵�������" << endl;
			cin >> name1 >> name2;
			DeterRela(T, name1, name2);
			break;
		case 5:
			cout << "������Ҫ��Ӻ��ӵĸ��׵�������" << endl;
			cin >> name;
			if (!AddChild(T, name))
				cout << "û���ҵ����׵���Ϣ��" << endl;
			break;
		case 6:
			if (!OpenFile(fileName)) {
				cout << "�ļ�������" << endl;
				exit(0);
			}
			SaveChange(T);
			dataFile.close();
			break;
		case 7:
			cout << "������Ҫ�Ƴ������ߵ�������" << endl;
			cin >> name;
			if (!SearchForDelete(T, name))
				cout << "δ�ҵ�Ҫ�Ƴ�������" << endl;
			break;
		case 8:
			cout << "������Ҫ�޸���Ϣ�ߵ�������" << endl;
			cin >> name;
			if (!SearchForChange(T, name))
				cout << "δ�ҵ�Ҫ�޸���Ϣ��" << endl;
			break;
		case 9:
			Print(T);
		default:
			break;
		}
		system("pause");
		system("cls");
	}
	return 0;
}
