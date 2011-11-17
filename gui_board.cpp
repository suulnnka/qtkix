#include "gui_board.h"

extern "C" {
#include "othellov3.c"
}

int Gui_Board::gui_board_new_game(){
	color = QT_BOARD_BLACK;
	step = 0;
	computer = QT_BOARD_WHITE;
	for(int i = 0 ;i<100;i++ )
		qboard[i] = QT_BOARD_NONE;
	for(int i = 0 ; i <= 9 ; i++ ){
		qboard[D2toD1(i,0)] = QT_BOARD_DUMMY;
		qboard[D2toD1(i,9)] = QT_BOARD_DUMMY;
		qboard[D2toD1(0,i)] = QT_BOARD_DUMMY;
		qboard[D2toD1(9,i)] = QT_BOARD_DUMMY;
	}
	qboard[D2toD1(4,4)] = QT_BOARD_WHITE;
	qboard[D2toD1(5,5)] = QT_BOARD_WHITE;
	qboard[D2toD1(4,5)] = QT_BOARD_BLACK;
	qboard[D2toD1(5,4)] = QT_BOARD_BLACK;

	is_over = false;

	return 0;
}
int Gui_Board::gui_board_undo(){
	if(step<=0)
		return 0;

	while(history[--step] == QT_BOARD_PASS);

	gui_board_replay();

	computer = -color;
	is_over = false;
	return color;
}
int Gui_Board::gui_board_auto_undo(){
	if(step<=1)
		return 0;

	int t = computer;

	int c = gui_board_undo();
	while(gui_board_undo() == c);

	computer = t;

	return 0;
}
int Gui_Board::gui_board_replay(){
	int t = computer;
	int s = step;
	gui_board_new_game();
	computer = t;

	for(int i=0;i<s;i++)
		gui_board_flip(history[i]);

	return 0;
}
int Gui_Board::gui_board_search(int deep,Ai_Data *data){
	if( is_over )
		return 0;

	//non ai
	//computer = QT_BOARD_PASS;

	//version v0.01
	//if(color == computer)
	//	return AI(qboard,deep,data,color);

	//version v0.02
	//if(color == computer)
	//	return AI(history,step,deep,data);

	//version v0.03
	if(color == computer)
		return AI(deep,qboard,color,data);

	return 0;
}
int Gui_Board::gui_board_shape_x(int x,int y,int shape){
	for(int i = 0 ;i < (shape&3) ;i++){
		int tx = x;
		int ty = y;
		x = 9-ty;
		y = tx;
	}
	if(shape & 4)
		x = 9 - x ;
	return x;
}
int Gui_Board::gui_board_shape_y(int x,int y,int shape){
	for(int i = 0 ;i < (shape&3) ;i++){
		int tx = x;
		int ty = y;
		x = 9-ty;
		y = tx;
	}
	if(shape & 4)
		x = 9 - x ;
	return y;
}
int Gui_Board::gui_board_shape_pan(int x,int y,int shape){
	if(shape&4)
		x = 9 - x;
	for(int i = 0 ; i < (shape&3);i++ ){
		int tx = x;
		int ty = y;
		x = ty;
		y = 9-tx;
	}
	return x*10+y;
}
int Gui_Board::gui_board_flip(int move){
	if( is_over )
		return 0;

	if(move == QT_BOARD_PASS){
		history[step] = move ;
		color = -color;
		step++;
		return 0;
	}

	if( ! gui_board_can_move(move) )
		return 0;

	for(int dir= 0;dir<8;dir++){
		int cnt = gui_board_can_move(move,dir);
		for(int cur = move + gui_board_dir[dir] ;
			cnt;cnt--,cur+=gui_board_dir[dir]){
			qboard[cur] = color;
		}
	}

	qboard[move] = color;
	history[step]=move ;
	step++ ;
	color = -color ;

	return 0;
}
int Gui_Board::gui_board_can_move(int move,int dir){
	int cnt = 0;
	for(int cur = move + gui_board_dir[dir] ;
		qboard[cur] == color || qboard[cur] == -color;
		cur+=gui_board_dir[dir]){
		if(qboard[cur] == color)
			return cnt;
		cnt++;
	}
	return 0;
}
int Gui_Board::gui_board_can_move(int move){
	if(move>=100||move<0)
		return false;
	if(qboard[move]!=QT_BOARD_NONE)
		return false;
	for(int dir = 0;dir<8;dir++)
		if(gui_board_can_move(move,dir))
			return true;
	return false;
}
int Gui_Board::gui_board_can_move(){
	for(int i = 0 ; i<100 ; i++ )
		if(gui_board_can_move(i))
			return true;
	return false;
}
int Gui_Board::gui_board_double_hit(){
	if( is_over )
		return 0;
	if( ! gui_board_can_move() ){
		gui_board_flip(QT_BOARD_PASS);
		if( ! gui_board_can_move() ){
			is_over = true;
			return 0;
		}
	}

	if( computer == color )
		return true;

	return 0;
}
int Gui_Board::gui_board_get_color(int move){
	return qboard[move];
}
int Gui_Board::gui_board_cnt(int c){
	int cnt = 0;
	for(int i = 0 ; i <100 ; i++)
		if(qboard[i] == c)
			cnt ++ ;
	return cnt;
}
int Gui_Board::gui_board_computer_swap_color(){
	computer = -computer;
	return 0;
}
