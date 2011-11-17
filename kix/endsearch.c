#include "endsearch.h"
#include "bit.h"
#include "flip.h"
#include "search.h"

int8 end_pvs(uint8 deep,uint64 my,uint64 opp,int8 alpha,int8 beta){
	node ++;
	if(deep == 0)
		return bit_count(my)-bit_count(opp);
	uint64 move_list = get_move(my,opp);
	uint64 m;
	uint64 flipped;
	int8 best_score,score;
	if(move_list == 0ull){ //no move
		if(pass[deep]) //game over
			return bit_count(my)-bit_count(opp);
		pass[deep] = 1;
		score = -end_pvs(deep,opp,my,-beta,-alpha);
		pass[deep] = 0;
		return score;
	}
	//first node
	m = move_list & (-move_list);
	flipped = (*flip[bit_to_x(m)])(my,opp) ;
	move_list = move_list ^ m;
	best_score = -end_pvs(deep-1,opp^flipped,my|flipped|m,-beta,-alpha);
	//other nodes
	while(move_list != 0ull){
		if(best_score >= beta)
			break;
		if(best_score > alpha)
			alpha = best_score;

		m = move_list & (-move_list);
		flipped = (*flip[bit_to_x(m)])(my,opp) ;
		move_list = move_list ^m;

		score = -end_alpha_beta(deep-1,opp^flipped,my|flipped|m,-alpha-1,-alpha);
		if(score > alpha && score < beta)
			best_score = -end_pvs(deep-1,opp^flipped,my|flipped|m,-beta,-score);
		else if(score > best_score)
			best_score = score;
	}
	//search end
	return best_score;
}
int8 end_alpha_beta(uint8 deep,uint64 my,uint64 opp,int8 alpha,int8 beta){
	node ++;
	if(deep == 0)
		return bit_count(my)-bit_count(opp);
	uint64 move_list = get_move(my,opp);
	uint64 m;
	uint64 flipped;
	int8 best_score = -65,score;
	if(move_list == 0ull){ //no move
		if(pass[deep]) //game over
			return bit_count(my)-bit_count(opp);
		pass[deep] = 1;
		score = -end_alpha_beta(deep,opp,my,-beta,-alpha);
		pass[deep] = 0;
		return score;
	}
	while(move_list != 0ull){

		m = move_list & (-move_list);
		flipped = (*flip[bit_to_x(m)])(my,opp) ;
		move_list = move_list ^m;

		score = -end_alpha_beta(deep-1,opp^flipped,my|flipped|m,-beta,-alpha);

		if(score > best_score){
			best_score = score;
			if(best_score > alpha){
				if(best_score >= beta)
					break;
				alpha = best_score;
			}
		}
	}
	//search end
	return best_score;
}

uint8 end_search(uint8 deep,uint64 my,uint64 opp,int8 *v){
	int8 best_score,score;
	uint8 best_move;
	uint64 move_list = get_move(my,opp);
	uint64 m;
	uint64 flipped;
	//first node
	m = move_list & (-move_list) ;
	best_move = bit_to_x(m);
	flipped = (*flip[bit_to_x(m)])(my,opp) ;
	move_list = move_list ^ m;
	best_score = -end_pvs(deep-1,opp^flipped,my|flipped|m,-65,65);
	//left nodes
	while(move_list != 0ull){ //需要节点排序
		m = move_list & (-move_list);
		flipped = (*flip[bit_to_x(m)])(my,opp) ;
		move_list = move_list ^ m ;
		score = -end_alpha_beta(deep-1,opp^flipped,my|flipped|m,-best_score-1,-best_score);
		if(score > best_score){
			best_score = -end_pvs(deep-1,opp^flipped,my|flipped|m,-65,-score);
			best_move = bit_to_x(m);
		}
	}
	//search end
	(*v) = best_score;
	return best_move;
}
