#include "AIShell.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"

int MAX = 999999999;
int MIN = -999999999;
int MAXDEPTH = 100000;
int WINNER = 9999;
clock_t TIME1;
int MOVESMADE = 0;
int GLOBALDEPTH;

AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
	this->deadline=5;
	this->numRows=numRows;
	this->numCols=numCols;
	this->gravityOn=gravityOn;
	this->gameState=gameState;
	this->lastMove=lastMove;
	this->Winner = -2;

}

AIShell::~AIShell()
{
	
	//delete the gameState variable.
	for (int i =0; i<numCols; i++){
		delete [] gameState[i];
	}
	delete [] gameState;
}

//Eval function that sets weights up to k-3 pieces
void AIShell::evaluatePoints(int tempCounter, int& score)
{
	if(tempCounter == 2)
		score = 1;
		
	if(tempCounter == k - 3)
		score = 4;
	
	if(tempCounter == k - 2)
		score = 8;
	
	if(tempCounter == k - 1)
		score = 12;	
}

//Scans the vertical counts for both players
void AIShell::VerticalWins(int** state, int col, int row, int turn, int& count, int& score)
{	
	
	//Int tempCol = col; int tempRow = row;
	bool bottomBlocked = false; bool topBlocked = false;

	if(state[col][row] == turn){
		count++;
		if(row - 1 < 0)
			bottomBlocked = true;
		else if(state[col][row - 1] == -turn)
			bottomBlocked = true;

		while((++row < numRows) && state[col][row] == turn)
			++count;	
		
		if(row >= numRows)
			topBlocked = true;
		else if(state[col][row] == -turn)
			topBlocked = true;

		if(count >= k)
			{
				score = WINNER;
				return;
			}
		if(bottomBlocked && topBlocked)
		{
			score = 0;
			return;
		}
		evaluatePoints(count, score);
	}
}

//Main function for vertical win count
std::tuple<int, int> AIShell::vertWinCount(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;

	VerticalWins(state, col, row, 1, aiCount, aiScore);
	VerticalWins(state, col, row, -1, otherCount, otherScore);
	return std::make_tuple(aiScore, otherScore);
}

//Horizontal Scanning function
std::tuple<int, int> AIShell::countHorizontalWins(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; 
	bool leftBlocked = false; bool rightBlocked = false;
	bool otherLBlocked = false; bool otherRBlocked = false;
	if(state[col][row] == 1)
	{
		aiCount++;
		while(--tempCol >= 0 && state[tempCol][row] == 1)
			++aiCount; 
		
		//Checks for blocked pieces
		if(tempCol < 0)
			leftBlocked = true;
		else if(state[tempCol][row] == -1)
			leftBlocked = true;

		tempCol = col; 
		while(++tempCol < numCols && state[tempCol][row] == 1)
			++aiCount;
		

		if(tempCol == numCols)
			rightBlocked = true;
		else if(state[tempCol][row] == -1)
			rightBlocked = true;

		if(aiCount >= k)
			return std::make_tuple(WINNER, 0);
		
		evaluatePoints(aiCount, aiScore);
		if(leftBlocked && rightBlocked)
			aiScore = 0;
	}
		
	if(state[col][row] == -1)
	{
		otherCount++;
		while(--tempCol >= 0 && state[tempCol][row] == -1)
			++otherCount;
		
		//Checks for blocked pieces
		if(tempCol < 0)
			otherLBlocked = true;
		else if(state[tempCol][row] == 1)
			otherLBlocked = true;

		tempCol = col; 
		while(++tempCol < numCols && state[tempCol][row] == -1)
			++otherCount;
		

		if(tempCol == numCols)
			otherRBlocked = true;
		else if(state[tempCol][row] == 1)
			otherRBlocked = true;

		if(otherCount >= k)
			return std::make_tuple(0, WINNER);
		
		evaluatePoints(otherCount, otherScore);
		if(otherLBlocked && otherRBlocked)
			otherScore = 0;
	}
		return std::make_tuple(aiScore, otherScore);
}

//Bottom left to top right scanner looping function 
void AIShell::diagonalLRLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state, int turn)
{
	bool topDiagBocked = false; bool bottomDiagBlocked = false;

	if(state[col][row] == turn){
		while((++tempRow < numRows && ++tempCol < numCols) && state[tempCol][tempRow] == turn)
			++count; 
		
		//Checks to see if diagonal top right is blocked
		if(tempRow < numRows && tempCol < numCols)
		{
			if(state[tempCol][tempRow] == -turn && count != k)
				topDiagBocked = true;
		}
		else if(tempRow == numRows || tempCol == numCols && count != k)
			topDiagBocked = true;

		//Reset column and row to original position before checking the bottom left diagonal
		tempCol = col; tempRow = row;
		while((--tempRow >= 0 && --tempCol >= 0) && state[tempCol][tempRow] == turn)
			++count; 
		
		//Checks to see if diagonal bottom left is blocked
		if(tempCol >= 0 && tempRow >= 0)
		{
			if(state[tempCol][tempRow] == -turn && count != k)
				bottomDiagBlocked = true;
		}

		else if(tempRow < 0 || tempCol < 0 && count != k)
			bottomDiagBlocked = true;

		//If both diagonals are blocking, don't count this score
		if(topDiagBocked && bottomDiagBlocked)
			count = 0;
	}
}

//Main function to bottom left to top right diagonal count scanner
std::tuple<int, int> AIShell::countDiagonalWinsLR(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; int tempRow = row;

	if(state[col][row] == 1)
	{
		aiCount++;
		diagonalLRLoop(tempCol, tempRow, col, row, aiCount, state, 1);
		if(aiCount >= k)
			return std::make_tuple(WINNER, 0);;
		evaluatePoints(aiCount, aiScore);
	}
	else if(state[col][row] == -1)
	{
		++otherCount;
		diagonalLRLoop(tempCol, tempRow, col, row, otherCount, state, -1);
		if(otherCount >= k)
			return std::make_tuple(0, WINNER);
		evaluatePoints(otherCount,otherScore);
	}
	return std::make_tuple(aiScore, otherScore);
}

//Bottom right to top left scanner looping function 
void AIShell::diagonalRLLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state, int turn)
{
	bool leftBlocked = false; bool rightBlocked = false;	
	while((--tempCol >= 0 && ++tempRow < numRows) && state[tempCol][tempRow] == turn)
		++count;
	
	if(tempRow < numRows && tempCol >= 0)
	{
		if(state[tempCol][tempRow] == -turn && count != k)
			leftBlocked = true;
	}

	else if(tempRow == numRows || tempCol < 0 && count != k)
		leftBlocked = true;

	tempCol = col; tempRow = row;
	while((++tempCol < numCols && --tempRow >= 0) && state[tempCol][tempRow] == turn)
		++count; 
	

	if(tempRow >= 0 && tempCol < numCols && count != k)
	{
		if(state[tempCol][tempRow] == -turn)
			rightBlocked = true;
	}

	else if(tempRow < 0 || tempCol == numCols && count != k)
		rightBlocked = true;
	if(leftBlocked && rightBlocked)
		count = 0;
}

//Main function to bottom right to top left diagonal count scanner
std::tuple<int, int> AIShell::countDiagonalWinsRL(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; int tempRow = row;

	if(state[col][row] == 1)
	{
		aiCount++;
		diagonalRLLoop(tempCol, tempRow, col, row, aiCount, state, 1);
		if(aiCount >= k)
			return std::make_tuple(WINNER, 0);
		
		evaluatePoints(aiCount, aiScore);
	}
	else if(state[col][row] == -1)
	{
		otherCount++;
		diagonalRLLoop(tempCol, tempRow, col, row, otherCount, state, -1);
		if(otherCount >= k)
			return std::make_tuple(0, WINNER);
		
		evaluatePoints(otherCount, otherScore);
	}
	return std::make_tuple(aiScore, otherScore);
}

//Checks counter for a winning score
bool checkForWin(int score)
{
	if(score >= WINNER)
		return true;
	return false;
}

//Main function call for hueristic. Scans all directions
int AIShell::countTotalWins(int** state, int turn) 
{
	int aiScore = 0; int otherScore = 0;
	int aiHScore = 0; int otherHScore = 0;
	for(int col = 0; col < numCols; col++)
		for(int row = 0; row < numRows; row++)
		{
			//vertical Win count
			int vertScore = std::get<0>(vertWinCount(state, col, row));
			int otherScore = std::get<1>(vertWinCount(state,col,row));
			if(checkForWin(otherScore))
				return -WINNER;
			
			if(checkForWin(vertScore))
				return WINNER;
			
			aiScore += (vertScore);
			
			//horizontal wins count
			int horizScore = std::get<0>(countHorizontalWins(state, col, row));
			otherScore = std::get<1>(countHorizontalWins(state, col, row));

			if(checkForWin(otherScore))
				return -WINNER;
			
			if(checkForWin(horizScore))
				return WINNER;
			
			aiScore += horizScore;
			
			//Diagonal wins bottom left to top right
			int diagLRScore = std::get<0>(countDiagonalWinsLR(state, col, row));
			otherScore = std::get<1>(countDiagonalWinsLR(state, col, row));
			if(checkForWin(otherScore))
				return -WINNER;
			
			if(checkForWin(diagLRScore))
				return WINNER;
			
			aiScore += diagLRScore;
			
			//Diagonal wins bottom right to top left
			int diagRLScore = std::get<0>(countDiagonalWinsRL(state, col, row));
			otherScore = std::get<1>(countDiagonalWinsRL(state, col, row));
			if(checkForWin(otherScore))
				return -WINNER;
			
			if(checkForWin(diagRLScore))
				return WINNER;
			
			aiScore += diagRLScore;
		}	 	
			return aiScore;
}

/*
Scans the board and puts available moves into a vector
Scans middle of board, first becaue initial moves 
are in middle of board
*/
std::vector<Move> AIShell::availableMoves(int** state){
	int count = 0;
	std::vector<Move> moveVector;
	 if(numCols <=6)
	 {
	for(int col = 0; col < numCols; col++)
		for(int row = 0; row < numRows; row++)
		{
			if(state[col][row] == NO_PIECE)
			{
				Move m = Move(col, row);
				moveVector.push_back(m);
			}
		}
	}
	else
	{
		//Push the middle of the board into available moves vector first
		for(int col = numCols/2 - 1; col < numCols/2 + numCols/3; col++)
			for(int row = 0; row < numRows; row++)
			{
				if(state[col][row] == NO_PIECE)
				{
					Move m = Move(col, row);
					moveVector.push_back(m);
				}
			}
		//Push the left of the gameboard into available moves vector
		for(int col = 0; col < numCols/2 - 1; col++)
			for(int row = 0; row < numRows; row++)
			{
				if(state[col][row] == NO_PIECE)
				{
					Move m = Move(col, row);
					moveVector.push_back(m);
				}
			}
		//Push the right side of gameboard into vector
		for(int col = numCols/2 + numCols/3; col < numCols; col++)
			for(int row = 0; row < numRows; row++)
			{
				if(state[col][row] == NO_PIECE)
				{
					Move m = Move(col, row);
					moveVector.push_back(m);
				}
			}
	}
	return moveVector;
}	

//Makes move on gameboard
Move AIShell::makeMove(){
 	Move m;
 	// if(AVILABLEMOVES == -1)
 	// 	initializeGlobalMoves(AVILABLEMOVES);
 	++MOVESMADE;
 	m = SearchForMove(gameState);
	return m;
}

/*
Places 4 initial moves in middle of board if available 
calls timer function and is main for iterative deepening function
*/
Move AIShell::SearchForMove(int** state)
{
	if(MOVESMADE <= 2)
	{
		//Pieces to place to speed up the initial game. Up to four pieces in the middle to start
		if(state[numCols/2][numRows/2] == NO_PIECE)
			return Move(numCols/2, numRows/2);

		else if(state[numCols/2 - 1][numRows/2 - 1] == NO_PIECE)
			return Move(numCols/2 - 1,numRows/2 - 1);

		else if(state[numCols/2][numRows/2-1] == NO_PIECE)
			return Move(numCols/2,numRows/2 - 1);
		else
			return Move(numCols/2-1, numRows/2);
	}
	TIME1 = clock();
	for(int depth = 1; depth < MAXDEPTH; depth++)
	{
		GLOBALDEPTH = depth;
		Move m = iterativeDeepening(depth, state, 0, MIN, MAX);
		if(m.score != 0)
			return m;
	}
}

//Sets time limit to stop recursive call. Max 4.7 second cutoff
bool triggered()
{
	if((float)(clock() - TIME1)/CLOCKS_PER_SEC > 4.7) //seconds
		return true;
	return false;
}

//Iterative Deepening call done inside
Move AIShell::iterativeDeepening(int depth, int** state, int turn, int alpha, int beta)
{
	int count = 0;
	Move m = miniMax(state, depth, turn, alpha, beta);
	if(m.score >= WINNER || m.score == -1 || triggered())
		return m;
	if(depth > 0) 
	{
		Move move = iterativeDeepening(depth - 1, state, turn, alpha, beta);
		if(move.score != 0)
			return move;
	}
	m.score = 0;
	return m;
}


/*
Minimax function with alpha beta pruning
Blocking done inside this function 
*/
Move AIShell::miniMax(int** state, int depth, int turn, int alpha, int beta){
	std::vector<Move> moveVector = availableMoves(state);
	std::vector<Move> toDelete;
	Move bestMove;

	if(depth == 0 || moveVector.size() == 0)
	{
		Move m(countTotalWins(state, turn));
		return m;
	}
	else if(turn % 2 == 0 )
	{
		//Player one - maximizing player
		int count = 0;
		int bestVal = MIN;
		for(int move = 0; move < moveVector.size(); move++)
		{		
			state[moveVector[move].col][moveVector[move].row] = AI_PIECE;
			toDelete.push_back(Move(moveVector[move].col, moveVector[move].row));
			Move m = miniMax(state, depth - 1, ++turn, bestVal, beta);

			moveVector[move].score = m.score;
			for(int i = 0; i < toDelete.size(); i++)
				state[toDelete[i].col][toDelete[i].row] = 0;

			//If player one receives a losing move, return it to use as blocking move
			if(m.score == -WINNER)
			{
				m.score = WINNER;
				return m;
			}
			if(bestVal < moveVector[move].score)
			{
				bestMove = moveVector[move];
				bestVal = moveVector[move].score;
			}
			if(alpha < bestVal)
				alpha = bestVal;
			if(beta <= bestVal)
				break;
			if(triggered())
				return bestMove;
		}
		return bestMove;
	}
	else
	{
		//Player two - minimizing player
		int bestVal = MAX;
		int count = 0;
		std::vector<Move> moveVector = availableMoves(state);
		for(int move = 0; move < moveVector.size(); move++){
			state[moveVector[move].col][moveVector[move].row] = HUMAN_PIECE;
			toDelete.push_back(Move(moveVector[move].col, moveVector[move].row));
			moveVector[move].score = miniMax(state, depth - 1, ++turn, alpha, bestVal).score;

			//If player two receives a winning score, player one uses this move to block
			if(moveVector[move].score == -WINNER)
			{
				bestMove = moveVector[move];
				return bestMove;
			}
			for(int i = 0; i < toDelete.size(); i++)
				state[toDelete[i].col][toDelete[i].row] = 0;
			if(bestVal > moveVector[move].score)
			{
				bestMove = moveVector[move];
				bestVal = moveVector[move].score;
			}
			if(beta > bestVal)
				beta = bestVal;

			if(bestVal <= alpha)
				break;

			if(triggered())
				return bestMove;
			
		}
		return bestMove;
	}
}