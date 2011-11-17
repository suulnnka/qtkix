//othello by vilinov
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ai_data.h"

#define NONE 0
#define BLACK 1
#define WHITE 2
#define DUMMY 3

#define f(x,y) (x)*9+y

void draw();
void MoveGenerator(int deep);//*
int getnum();
void turn(int deep,int num);//*
void back(int deep);//*
//int AI();
void help();
int check(int num);
int CheckWay(int num);
int SetLevel();

void EndGenerator(int deep);
int HappyEnding(int deep);
int BadEnding(int deep,int alpha,int beta);
int Ending(int deep,int alpha,int beta);

int search(int deep);
int AlphaBeta(int deep,int alpha,int beta);
int PVS(int deep,int alpha,int beta);

int MAX_DEPTH=10;
int END_DEPTH=14;

int colour=BLACK;
int computer=WHITE;
int black=2,white=2;

int node;

char board[91]={
	3,3,3,3,3,3,3,3,3,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,2,1,0,0,0,
	3,0,0,0,1,2,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,3,3,3,3,3,3,3,3,3};
int direction[8]={-10,-9,-8,-1,1,8,9,10};
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
	0,0,0,0,0,0,0,0,0};

int MoveList[20][30];
int TurnList[20][10];
//0-7 各方向上的吃子个数。8 num。9 总吃子个数
int space[20];//ENDing search

int flag[20];
int BestScore;
int BestMove;

int move[61]={0,//这个零是“捣乱”的。说笑，为了一点点的效率提升……
/*C4*/31,32,58,59,39,42,48,51,
/*C3*/30,33,57,60,
/*A1*/10,17,73,80,
/*A3*/12,15,75,78,28,35,55,62,
/*A4*/13,14,76,77,37,44,46,53,
/*B3*/21,24,66,69,29,34,56,61,
/*B4*/22,23,67,68,38,43,47,52,
/*A2*/11,16,74,79,19,26,64,71,
/*B2*/20,25,65,70};

int way;//check()&CheckWay()&turn()&back()
int now;//CheckWay()
int count;//CheckWay()
int temp;//MoveGenerator()&turn()&back()&AI()&G()

void NEW(){
	colour=BLACK;
	computer=WHITE;
	black=2,white=2;
	int x,y;
	for(x=0;x<91;x++)
		board[x]=3;
	for(x=1;x<9;x++)
	for(y=1;y<9;y++)
		board[f(x,y)]=0;
	board[f(4,4)]=2;
	board[f(5,5)]=2;
	board[f(4,5)]=1;
	board[f(5,4)]=1;
}
void draw(){
    int x,y;
	printf(" 12345678");
	for(x=1;x<9;x++){
		printf("\n%c",(char)x-1+'a');
		for(y=1;y<9;y++){
			if(!board[f(x,y)])printf(".");
			if(board[f(x,y)]==WHITE)printf("O");
			if(board[f(x,y)]==BLACK)printf("X");
		}
	}
	printf("\n");
	if(colour==computer){
		//printf("node:%dkn/s\n",node/(time2-time1));
		printf("BestScore:%d\n",BestScore);
	}
	return;
}
void MoveGenerator(int deep){
	MoveList[deep][0]=0;
	for(temp=60;temp>0;temp--)
		if(check(move[temp]))
			MoveList[deep][ ++MoveList[deep][0] ]=move[temp];
	return;
}
int check(int num){
	if(board[num]) return 0;
	for(way=7;way>=0;way--)
		if(CheckWay(num))
			return 1;
	return 0;
}
int CheckWay(int num){
	count=0;
	for(now=num+direction[way];
	board[now]&&board[now]!=DUMMY;now+=direction[way] ){
		if(board[now]==colour) 
			return count;
		count++;
	}
	return 0;
}
void turn(int deep,int num){     
	TurnList[deep][9]=0;
	for(way=7;way>=0;way--){
		temp=CheckWay(num);
		TurnList[deep][way]=temp;
		TurnList[deep][9]+=temp;
		for(now=num+direction[way];temp>0;temp--,now+=direction[way])
			board[now]=colour;
	}
	TurnList[deep][8]=num;
	board[num]=colour;
	if(colour==WHITE){
		white+=TurnList[deep][9]+1;
		black-=TurnList[deep][9];
	}else{
		white-=TurnList[deep][9];
		black+=TurnList[deep][9]+1;
	}
	return;
}
void back(int deep){
	int temp2=3-colour,num=TurnList[deep][8];
	for(way=7;way>=0;way--)
		for(temp=TurnList[deep][way],now=num+direction[way];temp>0;temp--,now+=direction[way])
			board[ now ]=temp2;
	board[ num ]=NONE;
	if(colour==WHITE){
		white-=TurnList[deep][9]+1;
		black+=TurnList[deep][9];
	}else{
		white+=TurnList[deep][9];
		black-=TurnList[deep][9]+1;
	}
	return;
}
/*
int getnum(){
	char s[30];
	int lenS;
	int num;
	for(;;){
		if(colour==computer)
			return AI();
		printf(">>");
		scanf("%29s",s);
		lenS=strlen(s);
		if(lenS==2)
			if(s[0]>='a'&&s[0]<='h'&&s[1]>='1'&&s[1]<='8'){
				num=f(s[0]-'a'+1,s[1]-'1'+1);
				if(check(num))
					return num;
			}
		if(lenS==1){
			if(s[0]=='q')
				exit(2);
			if(s[0]=='h')
				help();
			if(s[0]=='c')
				computer=3-computer;
			if(s[0]=='s')
				computer=1-computer;
			if(s[0]=='d')
				draw();
			if(s[0]=='n')
				return 0;
			if(s[0]=='l')
				SetLevel();
		}
		if(lenS==4&&!strcmp("exit",s))
			exit(2);
		if(lenS==4&&!strcmp("help",s))
			help();
		if(lenS==4&&!strcmp("draw",s))
			draw();
		if(lenS==3&&!strcmp("new",s))
			return 0;
		if(lenS==5&&!strcmp("level",s))
			SetLevel();
	}
}*/
int SetLevel(){
	char c;
	printf("当前中盘搜索深度%d层，终盘搜索深度%d层\n",MAX_DEPTH,END_DEPTH);
	while(1){
		printf("设置中盘搜索深度(1-12):");
		scanf("%d",&MAX_DEPTH);
		if (feof(stdin) || ferror(stdin)){
			printf("\n输入错误\n");
			exit(2);
		}
		while ( (c = getchar()) != '\n' && c != EOF ) ;
		if(MAX_DEPTH<1||MAX_DEPTH>12)
			continue;
		break;
	};
	while(1){
		printf("设置尾盘搜索深度(1-16):");
		scanf("%d",&END_DEPTH);
		if (feof(stdin) || ferror(stdin)){
			printf("\n输入错误\n");
			exit(2);
		}
		while ( (c = getchar()) != '\n' && c != EOF ) ;
		if(END_DEPTH<1||END_DEPTH>16)
			continue;
		break;
	};
	return 0;
}
void help(){
	printf("--------------HELP--------------\n");
	printf("\"h\",\"help\"   帮助\n");
	printf("\"a1\"..\"h8\"   棋子地点\n");
	printf("\"q\",\"exit\"   退出\n");
	printf("\"d\",\"draw\"   画出棋盘\n");
	printf("\"n\",\"new\"    重新开始\n");
	printf("\"l\",\"level\"  设置等级\n");
	printf("\"c\"          让步\n");
	printf("\"s\"          双人下棋（再按一次则返回人机）\n");
	printf("--------------END--------------\n");
	return;
}
int AI(int history[],int step,int deep,Ai_Data *data){
	NEW();

	int i;
	int x,y;
	for(i=0;i<step;i++){
		x = history[i]/10;
		y = history[i]%10;
		if(history[i] != 100)
			turn(0,f(x,y));
		colour = 3 - colour ;
	}
	node = 0;

	space[0]=64-white-black;
	memset(flag,0,sizeof(flag));
	int m;
	if(space[0]>deep)
		m = search(deep);
	else{
		space[0]=0;
		for(temp=60;temp>0;temp--)
			if(!board[ move[temp] ])
				space[ ++space[0] ]=move[temp];
		m = HappyEnding(space[0]);//o(∩_∩)o
	}

	data->node = node;
	data->value = BestScore;
	x = m / 9;
	y = m % 9;
	return x*10+y;
}
int G(){
	int val=0;
	for(temp=10;temp<81;temp++)
		if(board[temp]==WHITE)
			val+=value[temp];
		else if(board[temp]==BLACK)
			val-=value[temp];
	//node++;
	return val;
}
///eNDing
void EndGenerator(int deep){
	MoveList[deep][0]=0;
	for(temp=space[0];temp>0;temp--)
		if(check(space[temp]))
			MoveList[deep][ ++MoveList[deep][0] ]=space[temp];
	return;
}
int HappyEnding(int deep){
    int score;
	node++;
	EndGenerator(deep);
	//the first node
	turn(deep,MoveList[deep][ MoveList[deep][0] ]);
	BestScore=-Ending(deep-1,-65,65);
	BestMove=MoveList[deep][ MoveList[deep][0] ];
	back(deep);
	MoveList[deep][0]--;
	//first node end
	for(;MoveList[deep][0]>0;MoveList[deep][0]--){
		turn(deep,MoveList[deep][ MoveList[deep][0] ]);
		score=-BadEnding(deep-1,-BestScore-1,-BestScore);
		if(score>BestScore){
			BestScore=-Ending(deep-1,-65,-score);
			BestMove=MoveList[deep][ MoveList[deep][0] ];
		}
		back(deep);
	}
	return BestMove;
}
int BadEnding(int deep,int alpha,int beta){
	int current=-65,score;
	node++;
	if(!deep)
		if(colour==WHITE)
			return black-white;
		else
			return white-black;
	colour=3-colour;
	EndGenerator(deep);
	if(!MoveList[deep][0]){//no move
		if(flag[deep])//game over
			if(colour==WHITE){
				colour=3-colour;
				return white-black;
			}else{
				colour=3-colour;
				return black-white;
			}
		//让步……英语怎么说？
		flag[deep]=1;
		score=-BadEnding(deep,-beta,-alpha);
		flag[deep]=0;
		colour=3-colour;
		return score;
	}
	for(;MoveList[deep][0]>0;MoveList[deep][0]--){
		turn(deep,MoveList[deep][ MoveList[deep][0] ]);
		score=-BadEnding(deep-1,-beta,-alpha);
		back(deep);
		if(score>current){
			current=score;
			if(score>=beta)
				break;
		}
	}
	colour=3-colour;
	return current;
}
int Ending(int deep,int alpha,int beta){
	int best,score;
	node++;
	if(!deep)
		if(colour==WHITE)
			return black-white;
		else
			return white-black;
	colour=3-colour;
	EndGenerator(deep);
	if(!MoveList[deep][0]){//no move
		if(flag[deep])//game over
			if(colour==WHITE){
				colour=3-colour;
				return white-black;
			}else{
				colour=3-colour;
				return black-white;
			}
		//让步
		flag[deep]=1;
		score=-Ending(deep,-beta,-alpha);
		flag[deep]=0;
		colour=3-colour;
		return score;
	}
	//first node
	turn(deep,MoveList[deep][ MoveList[deep][0] ]);
	best=-Ending(deep-1,-beta,-alpha);
	back(deep);
	MoveList[deep][0]--;
	//node end
	for(;MoveList[deep][0]>0;MoveList[deep][0]--){
		if(best<beta){
			if(best>alpha)
				alpha=best;
			turn(deep,MoveList[deep][ MoveList[deep][0] ]);
			score=-BadEnding(deep-1,-alpha-1,-alpha);
			if(score>alpha&&score<beta)
				best=-Ending(deep-1,-beta,-score);
			else if(score>best)
				best=score;
			back(deep);
		}
	}
	colour=3-colour;
	return best;
}
///eNDing eND
///search
int search(int deep){
    int score;
	node++;
	MoveGenerator(deep);
	//the first node
	turn(deep,MoveList[deep][ MoveList[deep][0] ]);
	BestScore=-PVS(deep-1,-32767,32767);
	BestMove=MoveList[deep][ MoveList[deep][0] ];
	back(deep);
	MoveList[deep][0]--;
	//first node end
	for(;MoveList[deep][0]>0;MoveList[deep][0]--){
		turn(deep,MoveList[deep][ MoveList[deep][0] ]);
		score=-AlphaBeta(deep-1,-BestScore-1,-BestScore);
		if(score>BestScore){
			BestScore=-PVS(deep-1,-32767,-score);
			BestMove=MoveList[deep][ MoveList[deep][0] ];
		}
		back(deep);
	}
	return BestMove;
}
int AlphaBeta(int deep,int alpha,int beta){
	int current=-32767,score;
	node++;
	if(!deep)
		if(colour==WHITE)
			return -G();
		else
			return G();
	colour=3-colour;
	MoveGenerator(deep);
	if(!MoveList[deep][0]){//no move
		if(flag[deep])//game over
			if(colour==WHITE){
				colour=3-colour;
				return G();
			}else{
				colour=3-colour;
				return -G();
			}
		//让步……英语怎么说？
		flag[deep]=1;
		score=-AlphaBeta(deep,-beta,-alpha);
		flag[deep]=0;
		colour=3-colour;
		return score;
	}
	for(;MoveList[deep][0]>0;MoveList[deep][0]--){
		turn(deep,MoveList[deep][ MoveList[deep][0] ]);
		score=-AlphaBeta(deep-1,-beta,-alpha);
		back(deep);
		if(score>current){
			current=score;
			if(score>=beta)
				break;
		}
	}
	colour=3-colour;
	return current;
}
int PVS(int deep,int alpha,int beta){
	int best,score;
	node++;
	if(!deep)
		if(colour==WHITE)
			return -G();
		else
			return G();
	colour=3-colour;
	MoveGenerator(deep);
	if(!MoveList[deep][0]){//no move
		if(flag[deep])//game over
			if(colour==WHITE){
				colour=3-colour;
				return G();
			}else{
				colour=3-colour;
				return -G();
			}
		//让步
		flag[deep]=1;
		score=-PVS(deep,-beta,-alpha);
		flag[deep]=0;
		colour=3-colour;
		return score;
	}
	//first node
	turn(deep,MoveList[deep][ MoveList[deep][0] ]);
	best=-PVS(deep-1,-beta,-alpha);
	back(deep);
	MoveList[deep][0]--;
	//node end
	for(;MoveList[deep][0]>0;MoveList[deep][0]--){
		if(best<beta){
			if(best>alpha)
				alpha=best;
			turn(deep,MoveList[deep][ MoveList[deep][0] ]);
			score=-AlphaBeta(deep-1,-alpha-1,-alpha);
			if(score>alpha&&score<beta)
				best=-PVS(deep-1,-beta,-score);
			else if(score>best)
				best=score;
			back(deep);
		}
	}
	colour=3-colour;
	return best;
}
/*
///search eND
int main(){
	int num;
	draw();
	for(;;){
		MoveGenerator(0);
		if(!MoveList[0][0]){
			colour=3-colour;
			MoveGenerator(0);
			if(!MoveList[0][0]){
				if(white>black)printf("\nWhite Win!\n\n");
				if(white<black)printf("\nBlack Win!\n\n");
				if(white==black)printf("\nDraw!\n\n");
				NEW();
				draw();
				continue;
			}
		}
		node=0;
		time1=clock();
		num=getnum();
		time2=clock();
		if(!num){
			NEW();
			draw();
			continue;
		}
		turn(0,num);
		draw();
		colour=3-colour;
	}
	return 0;
}
*/

