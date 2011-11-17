//
#include <stdio.h>
#include "ai_data.h"

#define NONE 0
#define BLACK 1
#define WHITE 2
#define DUMMY 3

#define PLAYER 1//player
#define COMPUTER 0//computer

int MAX_DEEP  = 10 ;
#define MAX_MOVE 40
//其实是9层

#define ft(x,y) x*9+y
char a[91]={3,3,3,3,3,3,3,3,3,
    3,0,0,0,0,0,0,0,0,
    3,0,0,0,0,0,0,0,0,
    3,0,0,0,0,0,0,0,0,
    3,0,0,0,2,1,0,0,0,
    3,0,0,0,1,2,0,0,0,
    3,0,0,0,0,0,0,0,0,
    3,0,0,0,0,0,0,0,0,
    3,0,0,0,0,0,0,0,0,
    3,3,3,3,3,3,3,3,3,3
    };//一维数组的速度快

char control=BLACK;//black is first
char wh=COMPUTER,bl=COMPUTER;//which is white,player or computer?

int put[8]={-10,-9,-8,-1,1,8,9,10};//走路用的

/*
inline draw()//绘出棋盘
int PlayControl()//请求控制方出子 
int count(char colour)//数数

int FindLife()//查看是否还有棋可走
int check(int num)//查看num是否可走
int CheckWay(int num,int i)//检查第i条路是否可以走 
inline play(int num)//翻子

int AI()//…… 
*/
/////////////////////////////////////////////
int move[20][MAX_MOVE+1];
char board[20][91];
int BestMove,BestScore,deep;
int score;

int colour;//CheckWay1()&check1()
int way;//check1()&turn()
int num,deep_S;//MakeMove()&G()
int now;//turn()
int ValueBlack,ValueWhite;//G()

int node_num;
int value_cur;

int value[91]={
	0,0,0,0,0,0,0,0,0,
	0, 40,-8,4,3,3,4,-8, 40,
	0,-8,-10,2,1,1,2,-10,-8,
	0, 4, 2,3,1,1,3, 2, 4,
	0, 3, 1,1,2,2,1, 1, 3,
	0, 3, 1,1,2,2,1, 1, 3,
	0, 4, 2,3,1,1,3, 2, 4,
	0,-8,-10,2,1,1,2,-10,-8,
	0, 40,-8,4,3,3,4,-8, 40,
	0,0,0,0,0,0,0,0,0
};

int CheckWay1(int deep,int num,int way){
	for(num+=put[way]<<1;
	board[deep][num]&&board[deep][num]!=DUMMY;num+=put[way])
		if(colour==board[deep][num])
			return 1;
	return 0;
}
int check1(int deep,int num){
	colour=(deep+1)%2+1;
	for(way=0;way<8;way++)
		if(board[deep][num+put[way]]==3-colour&&CheckWay1(deep,num,way))
			return 1;
	return 0;
}
//没想到我的程序这么晦涩了
void MakeMove(int deep){
	deep_S=deep-1;
	move[deep][0]=0;
	int x,y;
	for(x=1;x<=8;x++)
	for(y=1;y<=8;y++){
		num = ft(x,y);
		if(!board[deep_S][num]&&check1(deep_S,num))
			move[deep][ ++move[deep][0] ]=num;
	}
	return;
}
int turn(int deep,int num){
	colour=deep%2+1;
	for(now=0;now<91;now++)
		board[deep][now]=board[deep-1][now];
	for(way=0;way<8;way++)
		if(board[deep][num+put[way]]==colour&&CheckWay1(deep,num,way))
			for(now=num+put[way];board[deep][now]!=colour;now+=put[way])
				board[deep][now]=colour;
	board[deep][num]=colour;
	return 1;
}
int G(int deep){
	ValueWhite=0;
	ValueBlack=0;
	for(num=10;num<82;num++)
		if(board[deep][num]==WHITE)
			ValueWhite+=value[num];
		else if(board[deep][num]==BLACK)
			ValueBlack+=value[num];
	if(deep%2)
		return ValueBlack-ValueWhite;
	return ValueWhite-ValueBlack;
	
}
int alpha_beta(int deep,int alpha,int beta){//暂不能用
	node_num ++ ;
	if(deep>MAX_DEEP)
		return G(deep-1);
	MakeMove(deep);
	if(move[deep][0]==0)
		return G(deep-1);
	for(;move[deep][0]>0;move[deep][0]--){
		turn(deep,move[deep][ move[deep][0] ]);
		score=-alpha_beta(deep+1,-beta,-alpha);
		if(score>=alpha){
			alpha=score;
			if(alpha>=beta)
				break;
		}
	}
	return alpha;
}
int search(){
	BestScore=-65535;
	MakeMove(1);
	for(;move[1][0]>0;move[1][0]--){
		turn(1,move[1][ move[1][0] ]);
		score=-alpha_beta(2,-65536,-BestScore);
		if(score>BestScore){
			BestScore=score;
			BestMove=move[1][ move[1][0] ];
		}
	}
	return BestMove;
}
//////////////////////////////////////////////
void draw()//绘出棋盘
{
    int x,y;
	//int t;
	printf(" 12345678\n");
	for(x=1;x<9;x++)
	{
		printf("%d",x);
		for(y=1;y<9;y++){
			#ifdef __WIN32__
			printf("%c",a[ft(x,y)]);
			#else
			if(!a[ft(x,y)])printf(" ");
			if(a[ft(x,y)]==WHITE)printf("W");
			if(a[ft(x,y)]==BLACK)printf("B");
			#endif
		}
		
		printf("\n");
	}
	return;
}

int count(char colour)//数数
{
	int x,y;
	int t,num=0;
	for(x=1;x<9;x++)
	{
		t=ft(x,0);
		for(y=1;y<9;y++)
			if(a[t+y]==colour)
				num++;
	}
	return num;
}

int CheckWay(int num,int i)//检查第i条路是否可以走 
{
	int now,count=0;//count是指中间有几个对方的子 
	for( now=num+put[i] ; a[now]!=DUMMY && a[now]!=NONE ; now+=put[i] )
	{
		if(a[now]==control)
			if(!count) return 0;//一个对方的子也没有 
			else return 1;
		count++;
	}
	return 0;
}

int play(int num)//翻子 
{
	int now,i;//now为当前的棋子序号 ，i为选择方向 
	for(i=0;i<8;i++)//8个方向 
	{
		if(!CheckWay(num,i))continue;//如果当前方向不可翻则退出 
		for( now=num+put[i] ; a[now]!=control ; now+=put[i] )
			a[now]=control;
	}
	a[num]=control;//别忘了当前子num需要翻过来 
	return 1;
}

int AI(int *bb,int deep ,Ai_Data *data,int c)
{
	MAX_DEEP = deep;
	int x,y;
	for(x = 1; x<=8; x++)
	for(y = 1; y<=8; y++){
		if(bb[x*10+y] == 1)
			a[ft(x,y)] = WHITE;
		if(bb[x*10+y] == -1)
			a[ft(x,y)] = BLACK;
		if(c == -1)
			a[ft(x,y)] = 3 - a[ft(x,y)];
		if(bb[x*10+y] == 0)
			a[ft(x,y)] = NONE;
	}
	int num;
	for(num=0;num<91;num++)
		board[0][num]=a[num];
	node_num = 0;
	BestScore = -65536;
	int m = search();
	data->node = node_num;
	data->value = BestScore;
	//printf("BestScore:%d\n",BestScore);
	x = m/9;
	y = m%9;
	return x*10+y;
}

/*int PlayControl()//请求控制方出子
{
	int x,y;
	if( ( control==BLACK && bl==PLAYER ) || ( control==WHITE && wh==PLAYER ) )
	{
        printf("please :");
        scanf("%d",&x);
		y=x%10;
		x=(x/10)%10;
		return ft(x,y);
	}
	else return AI();
}*/

int check(int num)//查看num是否可走
{
	if(a[num] != NONE) return 0;
	int i;
	for(i=0;i<8;i++)
		if(CheckWay(num,i)) return 1;
	return 0;
}

int FindLife()//查看是否还有棋可走，其实有两种方法可以实现的
{
	int x,y;
	for(x=1;x<9;x++)
	{
		for(y=1;y<9;y++)
			if(check(ft(x,y))) return 1;
	}
	return 0;
}
/*
int main()
{
    int step,num;
	int wsum,bsum;
	
	for(step=1;step<61;step++)
	{
		while(1)//这里是防出错
		{
			num=PlayControl();
			if(check(num))break;
		}
		
        play(num);
		draw();
			
		control=3-control;
		if(FindLife()) continue;//如果下一步有棋可走
			
		control=3-control;//连下的判断
		if(FindLife()) continue;
		//现在都没棋可走了
		break;
	}
	bsum=count(BLACK);
	wsum=count(WHITE);
	
	if(bsum<wsum)printf("\nwhite win\n");
	if(bsum>wsum)printf("\nblack win\n");
	if(bsum==wsum)printf("\ndraw\n");
	return 0;
}
//the end
*/
