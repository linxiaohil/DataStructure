#include <iostream>
#include <fstream>
using namespace std;
 
typedef struct point
{
	int r,c;
}point;
 
point stack[256];
int num=0;
 
int find1(int row,int col,int path[][50],int a,int b,int x2,int y2)                              
{
	if((a==x2) && (b==y2)) 
    {
    	return 1;
    }
    else
	{
		if((b!=col-1)&&(path[a][b+1]==0))          //�ж��Ƿ��������� 
    	{
    		path[a][b]=2;
    		stack[num].r=a;
	        stack[num].c=b;
        	num++;
    		find1(row,col,path,a,b+1,x2,y2);
    	}
    	else
    	{
	    	if((a!=row-1)&&(path[a+1][b]==0))        //��������ʱ���� 
	    	{
	    		path[a][b]=2;
	    		stack[num].r=a;
	            stack[num].c=b;
        	    num++;
	    		find1(row,col,path,a+1,b,x2,y2);
	    	}
		    else
    		{
	    		if((a!=0) && (path[a-1][b]==0))     //������������ʱ���� 
		    	{
		    		path[a][b]=2;
	    	    	stack[num].r=a;
	                stack[num].c=b;
        	        num++;
				    find1(row,col,path,a-1,b,x2,y2);
			    } 
				else
				{
				    if((b!=0) && (path[a][b-1]==0))     //����������������ʱ���� 
				    {
				    	path[a][b]=2;
	    	        	stack[num].r=a;
	                    stack[num].c=b;
        	            num++;
				    	find1(row,col,path,a,b-1,x2,y2);
					}
					else                      //�޷��ƶ�ʱ����Ǵ˸񣬺���һ�� 
				    {
				    	path[a][b]=9;
				    	num--;
				    	if(num==0)
				    	{
				    		return 0;
						}
				    	int x,y;
				    	x=stack[num].r;
				    	y=stack[num].c;
				    	find1(row,col,path,x,y,x2,y2);
					}
				}
		    }
	    }
	} 	
}
 
int main() 
{
	ifstream infile;
	infile.open("test.txt");
	if(infile)
	{
		int row,col;
		infile>>row;
		infile>>col;
		int path[row][50];
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{
				infile>>path[i][j];
			}
		}
		infile.close();
		printf("�Թ���ͼ����ͼ��ʾ��\n");
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{
				cout<<path[i][j]<<" ";
			}
			printf("\n");
		}
		printf("\n��������ںͳ���λ�ã�\n");
		printf("����1 1 7 7�����Ϊ��1�е�1�У�����Ϊ��7�е�7�У�\n");
		int x1,x2,y1,y2; 
		scanf("%d %d %d %d",&x1,&y1,&x2,&y2);
		x1--;
		x2--;
		y1--;
		y2--;
		if((path[x1][y1]==1) || (path[x2][y2]==1))
		{
			printf("�����������������룡");
		}
		else
		{
			int flag;
			flag=find1(row,col,path,x1,y1,x2,y2);
			if(flag==1)
			{
				printf("�Թ���·���ڣ�\n");
				path[x2][y2]=2;
				
				for(int i=0;i<row;i++)             //����Թ�·�� 
				{
					for(int j=0;j<col;j++)
					{
						if((path[i][j]==1))
						{
							printf("# ");	
						}
						else
						{
							if(path[i][j]==9||(path[i][j]==0))
							{
								printf("  ");
							}
							else
							{
								printf("o ");
							}
						}
					}
					printf("\n");
				}
			}
			else
			{
				printf("����·��\n");
			}
		}
	}
	else
	{
		printf("�ļ���ʧ��"); 
	}
	return 0;
}
