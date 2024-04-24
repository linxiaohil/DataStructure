#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <random>
#include <iomanip>
#include <time.h>
using namespace std;

#define Person_Num 360

float MAXT = 0;
float MAXKM = 0;

struct Person
{
    string ID;//���֤��
    string name;//����
    long int Total_KM=0;//�����
    int times = 0;//��������
}person[Person_Num+1];

struct Record
{
    int Flight_ID;//�����
    string Flight_Date;//��������
    string ID;//���������֤��
    int KM;//���
}record[1000];

vector <string> split(string& str,const char space = ' ')
//�ַ����ָ��split
{
    istringstream sstr(str);
    vector <string> result;
    string tmp;
    while(getline(sstr,tmp,space))
    {
        result.push_back(tmp);
    }
    return result;

}

vector<string> generate_name();

void gennerate_ID()
//����ID������
{
    srand(time(0));
    vector<string> nm = generate_name();
    for(int i=0;i<360;++i)
    {
        string str = to_string(rand()%9+1);
        for(int j=0;j<16;++j)
        {
            switch(rand()%10)
            {
                case 0:
                    str += '0';
                    break;
                case 1:
                    str += '1';
                    break;
                case 2:
                    str += '2';
                    break;
                case 3:
                    str += '3';
                    break;
                case 4:
                    str += '4';
                    break;
                case 5:
                    str += '5';
                    break;
                case 6:
                    str += '6';
                    break;
                case 7:
                    str += '7';
                    break;
                case 8:
                    str += '8';
                    break;
                case 9:
                    str += '9';
                    break;
            }
        }
        switch(rand()%10)
        {
            case 0:
                str += 'X';
                break;
            case 1:
                str += '1';
                break;
            case 2:
                str += '2';
                break;
            case 3:
                str += '3';
                break;
            case 4:
                str += '4';
                break;
            case 5:
                str += '5';
                break;
            case 6:
                str += '6';
                break;
            case 7:
                str += '7';
                break;
            case 8:
                str += '8';
                break;
            case 9:
                str += '9';
                break;
        }
        person[i].ID = str;
        person[i].name = to_string(rand()%nm.size());
        //person[i].name = nm[rand()%nm.size()];
    }
    
}

vector<string> generate_name()
//����������� �����ı������ң�ֻ���������˺���
{
    fstream name;
    name.open("name.txt",ios::in);
    vector<string> nm;
    string str;
    while(getline(name,str))
    {
        vector<string> tmp;
        tmp = split(str,'��');
        vector<string>::iterator it;
        for(it=tmp.begin();it!=tmp.end();++it)
        {
            //cout<<*it<<endl;
            nm.push_back(*it);
        }
    }
    return nm;
}

void generate_record()
//�������1000�����м�¼
{
    srand(time(0));
    vector<string> nm = generate_name();
    vector <int> Flight_ID = {
                            12001,12002,12003,12004,12005,12006,12007,
                            13001,13002,13003,13004,13005,13006,13007,
                            14001,14002,14003,14004,14005,14006,14007,
                            15001,15002,15003,15004,15005,15006,15007
                            };
    int Flight_ID_size = Flight_ID.size();
    int size = Person_Num;
    string YEAR = "2021";
    for(int i=0;i<1000;++i)
    {
        string MONTH = to_string(rand()%12 +1);
        string DAY = to_string(rand()%27 +1);
        string date = YEAR + '-' + MONTH + '-' + DAY;
        int fid = Flight_ID[rand()%Flight_ID_size];
        int pid = rand()%size;
        person[pid].times++;
        record[i].ID = person[pid].ID;
        record[i].Flight_ID = fid;
        record[i].Flight_Date = date;
        record[i].KM = rand()%1000;//1000���ڵ������
        person[pid].Total_KM += record[i].KM;
    }
}

void Write_File()
{
    fstream infile;
    infile.open("data.txt",ios::out|ios::app);
    cout<<"����ģ������360���˵���Ϣ:"<<endl;
    gennerate_ID();
    cout<<"ID�����ɣ�"<<endl;
    generate_name();
    cout<<"���������ɣ�"<<endl;
    cout<<"����ģ������1000�����м�¼:"<<endl;
    generate_record();
    cout<<"��¼������"<<endl;
    cout<<"���潫��Ϣд���ļ���־:"<<endl;
    for(int i=0;i<1000;++i)
    {
        infile
            <<record[i].ID
            <<" "<<record[i].Flight_Date
            <<" "<<record[i].Flight_ID
            <<" "<<record[i].KM<<endl;
    }
    cout<<"��Ϣ��д��!"<<endl;
    infile.close();
}

struct HashNode
{
    string key;//�ؼ���
    Record* arr = new Record[1000];//����ַ
    int size=0;//����ַ��С
}HashList[Person_Num],HashList2[2000];

int search(int i)
{
    for(int j=0;j<Person_Num;++j)
    {
        if(record[i].ID==HashList[j].key)
        {
            return j;
        }
    }
    return -1;
}

int search2(int i)
{
    for(int j=0;j<Person_Num;++j)
    {
        if(record[i].ID==person[j].ID)
        {
            return j;
        }
    }
    return -1;
}

void Print_Hash1();

void Hash1()
{
    for(int i=0;i<Person_Num;++i)
    {
        HashList[i].key = person[i].ID;
    }
    for(int i=0;i<1000;++i)
    {
        int j = search(i);//���ҵ�i��record��Ӧ��ID��person�����е�λ��
        if(j == -1)
        {
            cout<<"���޴�ID��"<<endl;
            return;
        }
        HashList[j].arr[HashList[j].size++] = record[i];
    }
    cout<<"�����ӡ��ϣ��:"<<endl;
    Print_Hash1();

}

void Print_Hash1()
{
    cout<<setw(10)<<"ID"<<setw(24)<<"�����"<<setw(18)<<"��������"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    for(int i=0;i<Person_Num;++i)
    {
        if(HashList[i].size==0)
        {
            continue;
        }
        else
        {
            for(int j=0;j<HashList[i].size;++j)
            {
                cout<<setw(10)<<HashList[i].key<<setw(15)<<HashList[i].arr[j].Flight_ID
                <<setw(20)<<HashList[i].arr[j].Flight_Date<<endl;
            }
            cout<<"------------------------------------------ͳ�ƴ���:"<<HashList[i].size<<endl;
        }
    }
}

void Hash2()
{
    cout<<setw(10)<<"ID"<<setw(24)<<"�����"<<setw(18)<<"��������"<<setw(15)<<"��ͻ����"<<endl;
    for(int i=0;i<2000;++i)
    {
        HashList2[i].key = "";
    }
    for(int i=0;i<1000;++i)
    {
        int count=0;
        int tmp = search2(i);//���ҵ�i��record��Ӧ��ID��person�����е�λ��
        if(tmp == -1)
        {
            cout<<"���޴�ID"<<endl;
            return;
        }
        while(HashList2[tmp].key != "")
        {
            ++count;
            tmp += count*count;
            tmp %= 2000;
            if(HashList2[tmp].key != "")
            {
                tmp -= 2*count*count;
                tmp %= 2000;
            }
        }
        HashList2[tmp].key = record[i].ID;
        cout<<setw(10)<<record[i].ID<<setw(15)<<record[i].Flight_ID
                <<setw(20)<<record[i].Flight_Date<<setw(10)<<count<<endl;
    }
}

float MAX_T()
{
    int max = 0;
    for(int i=0;i<Person_Num;++i)
    {
        if(person[i].times>max)
        {
            max = person[i].times;
        }
    }
    return float(max);
}

float MAX_KM()
{
    int max = 0;
    for(int i=0;i<Person_Num;++i)
    {
        if(person[i].Total_KM>max)
        {
            max = person[i].Total_KM;
        }
    }
    return float(max);
}

bool cmp(const Person &a,const Person &b)
{
    float aa = float(a.times)/MAXT;
    float ab = float(a.Total_KM)/MAXKM;
    float ba = float(b.times)/MAXT;
    float bb = float(b.Total_KM)/MAXKM;
    //cout<<(aa+ab)*0.5 <<" "<< (ba+bb)*0.5<<endl;
    return (aa+ab)*0.5 > (ba+bb)*0.5;
    //return a.Total_KM*a.times>b.times*b.Total_KM;
}

int main()
{
    Write_File();
    system("pause");
    system("cls");
    while(1)
    {
        cout<<"�������ṩ���¹���:"<<endl;
        cout<<"1.��������ַ�������ͻ�Ĺ�ϣ����"<<endl;
        cout<<"2.����ƽ��̽�ⷨ�����ͻ�Ĺ�ϣ����"<<endl;
        cout<<"3.�˻�Ƶ��ͳ�Ƽ�VIP�ͻ�ɸѡ"<<endl;
        cout<<"-------------------------------------"<<endl;
        cout<<"��������Ҫʹ�õĹ���:";
        int c;
        cin>>c;
        Person sort_person[Person_Num];
        for(int i=0;i<Person_Num;++i)
        {
            sort_person[i] = person[i];
        }
        MAXT = MAX_T();
        MAXKM = MAX_KM();
        switch(c)
        {
            case 1:
                Hash1();
                system("pause");
                system("cls");
                break;
            case 2:
                Hash2();
                system("pause");
                system("cls");
                break;
            case 3: 
                //memcpy(sort_person,&person,sizeof person);
                sort(sort_person,sort_person+Person_Num,cmp); 
                cout<<MAXT<<" "<<MAXKM<<endl;
                cout<<setw(10)<<"ID"<<setw(18)<<"����"<<setw(15)<<"��������"<<setw(11)<<"�����"<<setw(11)<<"��Ҫϵ��"<<endl;
                for(int i=0;i<Person_Num;++i)
                {
                    float weight;
                    weight = (float(sort_person[i].times)/MAXT) + float(sort_person[i].Total_KM)/MAXKM;
                    weight *= 0.5;
                    cout<<setw(10)<<sort_person[i].ID<<setw(10)<<sort_person[i].name
                        <<setw(10)<<sort_person[i].times<<setw(15)<<sort_person[i].Total_KM
                        <<setw(10)<<setiosflags(ios::fixed)<<setprecision(4)<<weight;
                    if(i<=10)
                        cout<<"  ��"<<endl;
                    else
                        cout<<endl;
                }
                system("pause");
                system("cls");
                break;
            default:
                exit(0);
        }
    }

    return 0;
}
