#ifndef MINIMAXALPHABETA_H
#define MINIMAXALPHABETA_H

#pragma once
#include "Move.h"
#include <vector> 

// A new AIShell will be created for every move request.
class AIShell{

public:
	//these represent the values for each piece type.
	static const int AI_PIECE=1;
	static const int HUMAN_PIECE = -1;
	static const int NO_PIECE=0;

private:
	//Do not alter the values of numRows or numcols.
	//they are used for deallocating the gameState variable.
	int numRows; //the total number of rows in the game state.
	int numCols; //the total number of columns in the game state.
	int **gameState; //a pointer to a two-dimensional array representing the game state.
	bool gravityOn; //this will be true if gravity is turned on. It will be false if gravity is turned off.
	Move lastMove; //this is the move made last by your opponent. If your opponent has not made a move yet (you move first) then this move will hold the value (-1, -1) instead.
	int AImove;
	int Winner;

public:
	int deadline; //this is how many milliseconds the AI has to make move.
	int k;        // k is the number of pieces a player must get in a row/column/diagonal to win the game. IE in connect 4, this variable would be 4

	AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove);
	AIShell(const AIShell &toCopy);
	~AIShell();
	Move makeMove();
	Move getBestMove(int** gamestate, int depth, int turn, int alpha, int beta);
	std::vector<Move> availableMoves(int** gameState);
	int assignHeuristic(Move m);
	void callGetBestMove();
	bool checkRowColumnDiag(int x, int y, int stepX, int stepY);
	bool IsLinearMatch(int x, int y);
	int winner(int **gameState);
	Move createRandomMove(int **gameState);
	float distance (int x1,int x2, int y1, int y2);
	Move checkk(int **gameState, int player);
	int** clone(int** gamestate);
	int countTotalWins(int** gameState, int turn);
	int sizeOfAvailableMoves(int** gameState);
	int countVerticalWins(int** state, int col, int row);
	int countHorizontalWins(int** state, int col, int row);
	int countDiagonalWinsLR(int** state, int col, int row);
	int countDiagonalWinsRL(int** state, int col, int row);
	void evaluatePoints(int tempCounter, int& score);
	void diagonalLRLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state);
	void diagonalRLLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state);


};

#endif //AISHELL_H
