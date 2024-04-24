#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <time.h>
using namespace std;

#define MAXN 0x3f3f3f  //�����������ֵ
#define col 50000  

int arr[10][col];  //�����άָ������

void Random()
{
    srand(time(0));  //����ʱ������
    for(int k=0;k<col;++k)
    {
        arr[0][k] = rand()%MAXN+1;
    }
    //memset(arr[0],rand()%MAXN+1,50000);
    for(int i=1;i<10;++i)
    {
        for(int j=0;j<col;++j)
        {
            arr[i][j] = arr[0][j];
        }
    }
}

void swap(int &a,int &b)
{
    a = a+b;
    b = a-b;
    a = a-b;
}

void Bubble(int c)
{
    if(c<0 || c>10) 
    {
        return;
    }
    if(col<=0)
    {
        return;
    }
    int *a = arr[c];
    int len = col;
    bool flag;
    for(int i=0;i<len-1;++i)
    {
        flag = false;
        for(int j=0;j<len-1-i;++j)
        {
            if(a[j]>a[j+1])
            {
                swap(a[j],a[j+1]);
                flag = true;
            }
        }
        if(!flag) break;
    }
    /*
    cout<<"����������Ϊ��"<<endl;
    for(int i=0;i<col;++i)
    {
        cout<<arr[c][i]<<" ";
    }
    */
}

void Select(int c)
{
    if(c<0 || c>10) 
    {
        return;
    }
    if(col<=0)
    {
        return;
    }
    int index;
    for(int i=0;i<col-1;++i)
    {
        index = i;
        for(int j=i+1;j<col;++j)
        {
            if(arr[c][index]>arr[c][j])
            {
                index = j;
            }
        }
        if(index != i)//��������ᵼ�¿ռ䶪ʧ
        {
            swap(arr[c][index],arr[c][i]);
        }
    }
    /*
    cout<<"����������Ϊ��"<<endl;
    for(int i=0;i<col;++i)
    {
        cout<<arr[c][i]<<" ";
    }
    */
}

int merge(int r[],int s[],int left,int mid,int right)
//s����Ҫ�洢�����飬��r����ʱ����
{
    int i,j,k;
    i = left;
    j = mid+1;
    k = left;
    while(i<=mid && j<=right)
    //��r���������鰴��С�鲢
    {
        if(r[i]<=r[j])
        {
            s[k] = r[i];
            i++;
            k++;
        }
        else
        {
            s[k] = r[j];
            j++;
            k++;
        }
    }
    while(i<=mid)
    {
        s[k++] = r[i++];
    }
    while(j<=right)
    {
        s[k++] = r[j++];
    }
    return 0;
}

int merge_sort(int r[],int s[],int left,int right)
{
    int mid;
    int* t = new int[right+1];
    //��ʱ����
    if(left==right)
    {
        s[left] = r[right];
    }
    else
    {
        mid = (left+right)/2;
        merge_sort(r,t,left,mid);
        merge_sort(r,t,mid+1,right);
        merge(t,s,left,mid,right);
    }
    return 0;
}

void MMerge(int c)
{
    if(col<=0) 
    {
        return;
    }
    if(c<0 || c>10) 
    {
        return;
    }
    int len = col;
    int *a = arr[c];
    merge_sort(a,a,0,len-1);
}

void Quick_sort(int *arr,int low,int high);

void Quick(int c)
{
    if(col<=0) 
    {
        return;
    }
    if(c<0 || c>10) 
    {
        return;
    }
    int *a = arr[c];
    Quick_sort(a,0,col-1);
}

int partition(int *arr ,int l,int r)
// ���ʵ��arr[l, r]������ѡ��һ����׼����߱Ȼ�׼С���ұ߱Ȼ�׼��
// ���ػ�׼����λ��
{
    int pivot = arr[l];
    while(l<r)
    {
        while(l<r && arr[r]>=pivot) 
        {
            --r;
        }
        if(l<r)
        {
            arr[l] = arr[r];
        }
        while(l<r && arr[l]<=pivot)
        {
            ++l;
        }
        if(l<r)
        {
            arr[r] = arr[l];
        }
    }
    arr[l] = pivot;
    return l;
}

void Quick_sort(int *arr,int low,int high)
{
    if(low<high)
    {
        int mid = partition(arr,low,high);
        Quick_sort(arr,low,mid);
        Quick_sort(arr,mid+1,high);
    }
}

void adjustHeap(int *arr, int root, int n)
// ���ʵ�ֶѵĵ���
{
    int left = 2*root+1;
    if(left < n)
    {
        int flag = left;
        int right = left+1;
        if(right<n)
        {
            if(arr[right]>arr[flag])
            {
                flag = right;
            }
        }
        if(arr[root] < arr[flag])
        {
            swap(arr[root],arr[flag]);
            adjustHeap(arr,flag,n);
        }
    }
}

int* Heap_sort(int *arr, int len)
//  ����adjustHeap�������ʵ�ֶ�����
{
    for(int i=len/2;i>=0;--i)
    {
        adjustHeap(arr,i,len);
    }
    for(int j=len-1;j>0;--j)
    {
        swap(arr[0],arr[j]);
        adjustHeap(arr,0,j);
    }
    return arr;
}

void Heap(int c)
{
    if(col<=0) 
    {
        return;
    }
    if(c<0 || c>10) 
    {
        return;
    }
    int *a = arr[c];
    a = Heap_sort(a,col);
    /*
    cout<<"����������Ϊ��"<<endl;
    for(int i=0;i<col;++i)
    {
        cout<<arr[c][i]<<" ";
    }
    */
}

void Heal_sort(int *arr, int n)
{
    int step[3]={5,2,1};//ϣ������Ĳ���
    int s=0;
    while(s<3)
    {
        for(int i=step[s];i<n;++i)
        {
            int key = *(arr+i);
            int j = i-step[s];
            while(j>=0 && key<*(arr+j))
            {
                *(arr+j+step[s]) = *(arr+j);
                j-=step[s];
            }
            *(arr+j+step[s]) = key;
        }
        s++;
    }
}

void Heal(int c)
{
    if(col<=0) 
    {
        return;
    }
    if(c<0 || c>10) 
    {
        return;
    }
    int *a = arr[c];
    Heal_sort(a,col);
    /*
    cout<<"����������Ϊ��"<<endl;
    for(int i=0;i<col;++i)
    {
        cout<<arr[c][i]<<" ";
    }
    */
}

void Insert(int c)
{
    if(col<=0) 
    {
        return;
    }
    if(c<0 || c>10) 
    {
        return;
    }
    int *a = arr[c];
    int len = col;
    int temp;
    for(int i=1;i<len;++i)
    {
        int key = a[i];
        int j = i-1;
        while(j>=0 && a[j]>key)
        {
            a[j+1] = a[j];
            --j;
        }
        a[j+1] = key;
    }
    /*
    cout<<"����������Ϊ��"<<endl;
    for(int i=0;i<col;++i)
    {
        cout<<arr[c][i]<<" ";
    }
    */
}

int* jishu_sort(int *arr, int n)
{
    int max = *max_element(arr,arr+n);
    int *radix = new int[n*10];
    fill(radix,radix+n*10,0);
    int *tmparr = new int[n];
    for(int i=0;i<n;i++)
    {
        tmparr[i] = arr[i];
    }
    int jishu[10];
    fill(jishu,jishu+10,-1);
    int wei = 0;
    while(max>0)
    {
        max /= 10;
        wei ++;
    }  
    for(int i=1;i<=wei;i++)
    {
        int dev = 1;
        for(int j=1;j<i;j++)
        {
            dev *= 10;
        }
        for(int j=0;j<n;j++)
        {
            int index = arr[j]/(dev)%(10);
            //tmparr[j] = tmparr[j]/(10);
            jishu[index]++;
            radix[index*n + jishu[index]] = arr[j];
        }
        int ind = 0;
        for(int k=0;k<10;k++)
        {    
            if(jishu[k]>=0)
            {
                sort(radix + k*n,radix + k*n + jishu[k]+1);
                for(int j=0;j<jishu[k]+1;j++)
                {
                    arr[ind] = radix[k*n+j];
                    ind ++;
                }
                jishu[k] = -1;
            }
        }
        fill(radix,radix+n*10,0);
    }
    return arr;
}

void jishu(int c)
{
    if(col<=0) 
    {
        return;
    }
    if(c<0 || c>10) 
    {
        return;
    }
    int *a = arr[c];
    int len = col;
    a = jishu_sort(a,len);
    /*
    cout<<"����������Ϊ��"<<endl;
    for(int i=0;i<col;++i)
    {
        cout<<arr[c][i]<<" ";
    }
    */
}

int main()
{
    double cost_time[10] = {0};
    while(1)
    {   
        cout<<"�ó���֧�ֵ����򷽷�(��һ������Ϊ���ź��������ڶ�������Ϊ��������):"<<endl;
        cout<<"1.ð������"<<endl;
        cout<<"2.ѡ������"<<endl;
        cout<<"3.�鲢����"<<endl;
        cout<<"4.��������"<<endl;
        cout<<"5.��������"<<endl;
        cout<<"6.ϣ������"<<endl;
        cout<<"7.������"<<endl;
        cout<<"8.��������"<<endl;
        cout<<"9.������������ƽ����ʱ��δ��ʱΪ0��"<<endl;
        cout<<"--------------------"<<endl;
        cout<<"����������Ҫʹ�õ����򷽷�:"<<endl;
        int c;cin>>c;
        time_t begin,end;
        double result;
        double total;
        switch(c)
        {
            case 1:
                total = 0;
                Random();
                sort(arr[0],arr[0]+col,less<int>());
                sort(arr[1],arr[1]+col,greater<int>());
                for(int i=0;i<10;++i)
                {
                    begin = clock();
                    Bubble(i);
                    end = clock();
                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
                    total += result;
                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
                }
                cost_time[c] = total/10;
                system("pause");
                system("cls");
                break;
            case 2:
                total = 0;
                Random();
                sort(arr[0],arr[0]+col,less<int>());
                sort(arr[1],arr[1]+col,greater<int>());
                for(int i=0;i<10;++i)
                {
                    begin = clock();
                    Select(i);
                    end = clock();
                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
                    total += result;
                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
                }
                cost_time[c] = total/10;
                system("pause");
                system("cls");
                break;
            case 3:
                total = 0;
                Random();
                sort(arr[0],arr[0]+col,less<int>());
                sort(arr[1],arr[1]+col,greater<int>());
//                ֻ���������������ء��������˾ͱ�ջ������ 
//                for(int i=0;i<5;++i)
//                {
//                    begin = clock();
//                    MMerge(i);
//                    end = clock();
//                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
//                    total += result;
//                    cost_time[c] = total/10;
//                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
//                }
//                system("pause");
                for(int i=5;i<10;++i)
                {
                    begin = clock();
                    MMerge(i);
                    end = clock();
                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
                    total += result;
                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
                }
                cost_time[c] = total/5;
                system("pause");
                system("cls");
                break;
            case 4:
                total = 0;
                Random();
                sort(arr[0],arr[0]+col,less<int>());
                sort(arr[1],arr[1]+col,greater<int>());
                for(int i=0;i<10;++i)
                {
                    begin = clock();
                    //Quick(i);//�ᱬջ
                    sort(arr[i],arr[i]+col,less<int>());
                    end = clock();
                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
                    total += result;
                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
                }
                cost_time[c] = total/10;
                system("pause");
                system("cls");
                break;
            case 7:
                total = 0;
                Random();
                sort(arr[0],arr[0]+col,less<int>());
                sort(arr[1],arr[1]+col,greater<int>());
                for(int i=0;i<10;++i)
                {
                    begin = clock();
                    Heap(i);
                    end = clock();
                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
                    total += result;
                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
                }
                cost_time[c] = total/10;
                system("pause");
                system("cls");
                break;
            case 6:
                total = 0;
                Random();
                sort(arr[0],arr[0]+col,less<int>());
                sort(arr[1],arr[1]+col,greater<int>());
                for(int i=0;i<10;++i)
                {
                    begin = clock();
                    Heal(i);
                    end = clock();
                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
                    total += result;
                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
                }
                cost_time[c] = total/10;
                system("pause");
                system("cls");
                break;
            case 5:
                total = 0;
                Random();
                sort(arr[0],arr[0]+col,less<int>());
                sort(arr[1],arr[1]+col,greater<int>());
                for(int i=0;i<10;++i)
                {
                    begin = clock();
                    Insert(i);
                    end = clock();
                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
                    total += result;
                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
                }
                cost_time[c] = total/10;
                system("pause");
                system("cls");
                break;
            case 8: 
                total = 0;
                Random();
                sort(arr[0],arr[0]+col,less<int>());
                sort(arr[1],arr[1]+col,greater<int>());
                for(int i=0;i<10;++i)
                {
                    begin = clock();
                    jishu(i);
                    end = clock();
                    result = double(end -begin)*1000/CLOCKS_PER_SEC;
                    total += result;
                    cout<<"��"<<i+1<<"�������Ĵ�������ʱ��Ϊ:"<<result<<endl;
                }
                cost_time[c] = total/10;
                system("pause");
                system("cls");
                break;
            case 9:
                cout<<"1.ð�����������ʱ��Ϊ:"<<cost_time[1]<<"ms"<<endl;
                cout<<"2.ѡ�����������ʱ��Ϊ:"<<cost_time[2]<<"ms"<<endl;
                cout<<"3.�鲢���������ʱ��Ϊ:"<<cost_time[3]<<"ms"<<endl;
                cout<<"4.�������������ʱ��Ϊ:"<<cost_time[4]<<"ms"<<endl;
                cout<<"5.�������������ʱ��Ϊ:"<<cost_time[5]<<"ms"<<endl;
                cout<<"6.ϣ�����������ʱ��Ϊ:"<<cost_time[6]<<"ms"<<endl;
                cout<<"7.�����������ʱ��Ϊ:"<<cost_time[7]<<"ms"<<endl;
                cout<<"8.�������������ʱ��Ϊ:"<<cost_time[8]<<"ms"<<endl;
                system("pause");
                system("cls");
                break;
            default:
                exit(0);
        }
    }
    return 0;
}
