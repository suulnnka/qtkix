#include "midsearch.h"
#include "bit.h"
#include "search.h"
#include "eval.h"
#include "flip.h"

int16 mid_pvs(uint8 deep,uint64 my,uint64 opp,int16 alpha,int16 beta){
	node ++;
	if(deep == 0)
		return eval(my,opp);
	uint64 move_list = get_move(my,opp);
	uint64 m;
	uint64 flipped;
	int16 best_score,score;
	if(move_list == 0ull){ //no move
		if(pass[deep]) //game over
			return bit_count(my)-bit_count(opp);
		pass[deep] = 1;
		score = -mid_pvs(deep,opp,my,-beta,-alpha);
		pass[deep] = 0;
		return score;
	}
	//first node
	m = move_list & (-move_list);
	flipped = (*flip[bit_to_x(m)])(my,opp) ;
	move_list = move_list ^ m;
	best_score = -mid_pvs(deep-1,opp^flipped,my|flipped|m,-beta,-alpha);
	//other nodes
	while(move_list != 0ull){
		if(best_score >= beta)
			break;
		if(best_score > alpha)
			alpha = best_score;

		m = move_list & (-move_list);
		flipped = (*flip[bit_to_x(m)])(my,opp) ;
		move_list = move_list ^m;

		score = -mid_alpha_beta(deep-1,opp^flipped,my|flipped|m,-alpha-1,-alpha);
		if(score > alpha && score < beta)
			best_score = -mid_pvs(deep-1,opp^flipped,my|flipped|m,-beta,-score);
		else if(score > best_score)
			best_score = score;
	}
	//search end
	return best_score;
}
int16 mid_alpha_beta(uint8 deep,uint64 my,uint64 opp,int16 alpha,int16 beta){
	node ++;
	if(deep == 0)
		return eval(my,opp);
	uint64 move_list = get_move(my,opp);
	uint64 m;
	uint64 flipped;
	int16 best_score = -10000,score;
	if(move_list == 0ull){ //no move
		if(pass[deep]) //game over
			return bit_count(my)-bit_count(opp);
		pass[deep] = 1;
		score = -mid_alpha_beta(deep,opp,my,-beta,-alpha);
		pass[deep] = 0;
		return score;
	}
	while(move_list != 0ull){

		m = move_list & (-move_list);
		flipped = (*flip[bit_to_x(m)])(my,opp);
		move_list = move_list ^m;

		score = -mid_alpha_beta(deep-1,opp^flipped,my|flipped|m,-beta,-alpha);

		if(score > best_score){
			best_score = score;
			if(score > alpha){
				if(score >= beta)
					break;
				alpha = score;
			}
		}
	}
	//search end
	return best_score;
}

uint8 mid_search(uint8 deep,uint64 my,uint64 opp,int8 *v){
	int16 best_score,score;
	uint8 best_move,move;
	uint64 move_list = get_move(my,opp);
	uint64 m;
	uint64 flipped;
	//first node
	m = move_list & (-move_list) ;
	best_move = bit_to_x(m);
	flipped = (*flip[bit_to_x(m)])(my,opp) ;
	move_list = move_list ^ m;
	best_score = -mid_pvs(deep-1,opp^flipped,my|flipped|m,-10000,10000);
	//left nodes
	while(move_list != 0ull){ //需要节点排序
		m = move_list & (-move_list);
		move = bit_to_x(m);
		flipped = (*flip[bit_to_x(m)])(my,opp) ;
		move_list = move_list ^ m ;
		score = -mid_alpha_beta(deep-1,opp^flipped,my|flipped|m,-best_score-1,-best_score);
		if(score > best_score){
			best_score = -mid_pvs(deep-1,opp^flipped,my|flipped|m,-10000,-score);
			best_move = move;
		}
	}
	//search end

	if(best_score>255)
		(*v) = 255;
	else if(best_score<-256)
		(*v) = -256;
	else
		(*v) = best_score;
	return best_move;
}
