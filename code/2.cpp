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
		if((b!=col-1)&&(path[a][b+1]==0))          //判断是否能向右走 
    	{
    		path[a][b]=2;
    		stack[num].r=a;
	        stack[num].c=b;
        	num++;
    		find1(row,col,path,a,b+1,x2,y2);
    	}
    	else
    	{
	    	if((a!=row-1)&&(path[a+1][b]==0))        //不能向右时向下 
	    	{
	    		path[a][b]=2;
	    		stack[num].r=a;
	            stack[num].c=b;
        	    num++;
	    		find1(row,col,path,a+1,b,x2,y2);
	    	}
		    else
    		{
	    		if((a!=0) && (path[a-1][b]==0))     //不能向右向下时向上 
		    	{
		    		path[a][b]=2;
	    	    	stack[num].r=a;
	                stack[num].c=b;
        	        num++;
				    find1(row,col,path,a-1,b,x2,y2);
			    } 
				else
				{
				    if((b!=0) && (path[a][b-1]==0))     //不能向右向上向下时向左 
				    {
				    	path[a][b]=2;
	    	        	stack[num].r=a;
	                    stack[num].c=b;
        	            num++;
				    	find1(row,col,path,a,b-1,x2,y2);
					}
					else                      //无法移动时，标记此格，后退一步 
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
		printf("迷宫地图如下图所示：\n");
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{
				cout<<path[i][j]<<" ";
			}
			printf("\n");
		}
		printf("\n请输入入口和出口位置：\n");
		printf("例：1 1 7 7（入口为第1行第1列，出口为第7行第7列）\n");
		int x1,x2,y1,y2; 
		scanf("%d %d %d %d",&x1,&y1,&x2,&y2);
		x1--;
		x2--;
		y1--;
		y2--;
		if((path[x1][y1]==1) || (path[x2][y2]==1))
		{
			printf("输入有误，请重新输入！");
		}
		else
		{
			int flag;
			flag=find1(row,col,path,x1,y1,x2,y2);
			if(flag==1)
			{
				printf("迷宫线路存在：\n");
				path[x2][y2]=2;
				
				for(int i=0;i<row;i++)             //输出迷宫路线 
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
				printf("无线路。\n");
			}
		}
	}
	else
	{
		printf("文件打开失败"); 
	}
	return 0;
}
