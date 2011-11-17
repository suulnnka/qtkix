#ifndef GUI_BOARD_H
#define GUI_BOARD_H

#include "ai_data.h"

#define D2toD1(x,y) (x)*10+(y)

#define QT_BOARD_WHITE 1
#define QT_BOARD_BLACK -1
#define QT_BOARD_NONE 0
#define QT_BOARD_DUMMY 2

#define QT_BOARD_PASS 100

const int gui_board_dir[]={-11,-10,-9,-1,1,9,10,11};

class Gui_Board{
private:
	int color;
	int qboard[100];
	int history[150];
	int step;
	int computer;
	int is_over;
public:
	int gui_board_new_game();
	int gui_board_undo();
	int gui_board_auto_undo();
	int gui_board_replay();
	int gui_board_search(int deep,Ai_Data *data);
	int gui_board_shape_x(int x,int y,int shape);
	int gui_board_shape_y(int x,int y,int shape);
	int gui_board_shape_pan(int x,int y,int shape);
	int gui_board_flip(int move);
	int gui_board_double_hit();
	int gui_board_get_color(int move);
	int gui_board_can_move();
	int gui_board_can_move(int move);
	int gui_board_can_move(int move,int dir);
	int gui_board_cnt(int c);
	int gui_board_computer_swap_color();
};

#endif // GUI_BOARD_H
