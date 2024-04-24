#include<cstdio>
#include<cstdlib>
#include<windows.h> 
#include<iostream>
#include<queue>
//��дһ����ʱ�� 
using namespace std;
int timecount=0; 
typedef struct {
	int keyId;
	int time;
}Key;

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

// ��ʾͼ�ν���
void ShowBox(int n, int *box, int time, int *key) {
	system("cls");
	cout<<"���ں������Կ��"<<endl; 
	cout<<"                          "; 
	for (int i=1; i<=n; i++) {
		printf("-----");
	}
	printf("\n");
	cout<<"                          "; 
	for (int i=1; i<=n; i++) {
		if (box[i]) printf("| %2d ",box[i]);
		else printf("|    ");
	}
	printf("|\n");
	cout<<"                          "; 
	for (int i=1; i<=n; i++) {
		printf("-----");
	}
	printf("\n\n");
	cout<<"����ʦ���ϵ�Կ��"<<endl; 
	cout<<"                          "; 
	for (int i=1; i<=n; i++) {
		if (key[i] == 0) printf(" %d ", i);
	}
	printf("\n\n\n\n\n\n\n");
	while(time>0)
	{
		Sleep(1000);
		time--;
		cout<<"                    �ڣ�"<<timecount++<<"��"<<endl;
	}


} 

int main() {
	fileOpen("r");
	
	int n = 0, k = 0;
	fscanf(fp, "%d%d", &n, &k);
	
	int w = 0, s = 0, c = 0;
	Key *borrow = (Key*)malloc(sizeof(Key) * k), *ret = (Key*)malloc(sizeof(Key) * k);//����ͷ��صĴ���
	for (int i=0; i<k; i++) {
		fscanf(fp, "%d%d%d", &w, &s, &c);
		// �����Կ�׺ź�ʱ�� 
		borrow[i].keyId = w;
		borrow[i].time = s;
		// �黹��Կ�׺ź�ʱ�� 
		ret[i].keyId = w;
		ret[i].time = s + c;
	}
	
	// �Խ���͹黹��Կ�ף�����ʱ���С�������� 
	for (int i=0; i<k; i++) {
		for (int j=i; j<k; j++) {
			if (borrow[j].time < borrow[i].time || (borrow[j].time == borrow[i].time && borrow[j].keyId < borrow[i].keyId)) {
				Key temp = borrow[j];
				borrow[j] = borrow[i];
				borrow[i] = temp;
			}
			if (ret[j].time < ret[i].time || (ret[j].time == ret[i].time && ret[j].keyId < ret[i].keyId)) {
				Key temp = ret[j];
				ret[j] = ret[i];
				ret[i] = temp;
			}
		}
	}

	// ���¼��Ž������� 
	queue<Key> q;
	int i = 0, j = 0;
	while (i<k && j<k) {
		if (borrow[i].time < ret[j].time) {
			q.push(borrow[i]);
			i++; 
		}
		else if (borrow[i].time > ret[j].time) {
			q.push(ret[j]);
			j++;
		}
		else {
			q.push(ret[j]);
			j++;
		}
	}
	while (i<k) {
		q.push(borrow[i]);
		i++;
	}
	while (j<k) {
		q.push(ret[j]);
		j++;
	}
	
	// ��keyԿ�׺�boxԿ�׺������ʼ����key���飺�±�Ϊ��ţ�ֵΪ��box�е�λ�� 
	int key[n+1], box[n+1]; 
	for (int i=1; i<=n; i++) {
		box[i] = i;
		key[i] = i;
	}
	// ���ݶ��н����¼�����
	int time = 0;
	while (!q.empty()) {
		Key cur = q.front();
		q.pop();
		
		ShowBox(n, box, cur.time - time, key);
		time = cur.time;
		
		// �����Կ���Ѿ�����һ��λ���ˣ���ȡ�� 
		if (key[cur.keyId]) {
			box[key[cur.keyId]] = 0;
			key[cur.keyId] = 0;
		}
		else {
			for (int i=1; i<=n; i++) {
				if (box[i] == 0) {
					key[cur.keyId] = i;
					box[i] = cur.keyId;
					break;
				}
			}
		}
	}
	ShowBox(n, box, 1, key);

	
	printf("\n������ʦ�Ѿ��¿�\n");
	
	free(borrow);
	free(ret);
	fileClose();
	system("pause"); 
}
