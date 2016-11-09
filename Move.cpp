#include "Move.h"


Move::Move()
{
	row = 0;
	col = 0;
	score = 0;
}
Move::Move(int col, int row){
	this->row=row;
	this->col=col;
}

Move::Move(int score){
	this->score = score;
}


