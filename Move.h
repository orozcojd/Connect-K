#ifndef MOVE_H
#define MOVE_H

#pragma once
class Move
{
public:
	int row; //the row to move to. 
	int col; //the col to move to.
	int score;
	Move();
	Move(int col, int row);
	Move(int score);
};

#endif //MOVE_H
