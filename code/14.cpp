#include<cstdio>
#include<cstdlib>
#include<windows.h> 
#include<iostream>
#include<queue>
//想写一个计时器 
using namespace std;
int timecount=0; 
typedef struct {
	int keyId;
	int time;
}Key;

// 全局变量 
FILE *fp;
char fileName[] = "input.txt";


// 打开文件 
void fileOpen(char *method) {
	if((fp=fopen(fileName,method)) == NULL) {
        printf("file cannot be opened\n");
        return;
    }
    //else printf("file has been opened\n");
}

// 关闭文件 
void fileClose() {
	if(fclose(fp)!=0) printf("file cannot be closed\n");
    //else printf("file has been closed\n");
    return;
} 

// 显示图形界面
void ShowBox(int n, int *box, int time, int *key) {
	system("cls");
	cout<<"正在盒子里的钥匙"<<endl; 
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
	cout<<"在老师手上的钥匙"<<endl; 
	cout<<"                          "; 
	for (int i=1; i<=n; i++) {
		if (key[i] == 0) printf(" %d ", i);
	}
	printf("\n\n\n\n\n\n\n");
	while(time>0)
	{
		Sleep(1000);
		time--;
		cout<<"                    第："<<timecount++<<"秒"<<endl;
	}


} 

int main() {
	fileOpen("r");
	
	int n = 0, k = 0;
	fscanf(fp, "%d%d", &n, &k);
	
	int w = 0, s = 0, c = 0;
	Key *borrow = (Key*)malloc(sizeof(Key) * k), *ret = (Key*)malloc(sizeof(Key) * k);//借出和返回的储存
	for (int i=0; i<k; i++) {
		fscanf(fp, "%d%d%d", &w, &s, &c);
		// 借出的钥匙号和时间 
		borrow[i].keyId = w;
		borrow[i].time = s;
		// 归还的钥匙号和时间 
		ret[i].keyId = w;
		ret[i].time = s + c;
	}
	
	// 对借出和归还的钥匙，根据时间从小到大排序 
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

	// 将事件放进队列中 
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
	
	// 对key钥匙和box钥匙盒数组初始化。key数组：下标为编号，值为在box中的位置 
	int key[n+1], box[n+1]; 
	for (int i=1; i<=n; i++) {
		box[i] = i;
		key[i] = i;
	}
	// 根据队列进行事件安排
	int time = 0;
	while (!q.empty()) {
		Key cur = q.front();
		q.pop();
		
		ShowBox(n, box, cur.time - time, key);
		time = cur.time;
		
		// 如果该钥匙已经发在一个位置了，就取出 
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

	
	printf("\n所有老师已经下课\n");
	
	free(borrow);
	free(ret);
	fileClose();
	system("pause"); 
}
