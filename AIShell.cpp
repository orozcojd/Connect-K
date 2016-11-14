#include "AIShell.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <tuple>

int MAX = 999999999;
int MIN = -999999999;

AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
	this->deadline=0;
	this->numRows=numRows;
	this->numCols=numCols;
	this->gravityOn=gravityOn;
	this->gameState=gameState;
	this->lastMove=lastMove;
	this->Winner = -2;
}

AIShell::AIShell(const AIShell &toCopy)
{
	this->numRows = toCopy.numRows;
	this->numCols = toCopy.numCols;
	this->gravityOn = toCopy.gravityOn;
	this->lastMove = toCopy.lastMove;
	this->AImove = toCopy.AImove;
	this->Winner = toCopy.Winner;

	this->gameState = new int*;
	for(int col = 0; col < toCopy.numCols; col++)
		for(int row = 0; row < toCopy.numRows; row++)
		{
			gameState[col][row] = toCopy.gameState[col][row];
		}
}

AIShell::~AIShell()
{
	
	//delete the gameState variable.
	for (int i =0; i<numCols; i++){
		delete [] gameState[i];
	}
	delete [] gameState;
}

int** AIShell::clone(int** state)
{		
	int** cloned = NULL ;
	cloned = new int*[numCols];
	for(int i = 0; i < numCols; i++)
		cloned[i] = new int[numRows];

	for(int col = 0; col < numCols; col++)
		for(int row = 0; row < numRows; row++)
		{
			cloned[col][row] = state[col][row];
		}
	return cloned;
}

void AIShell::evaluatePoints(int tempCounter, int& score)
{
	if(tempCounter == 2)
		{
			score = 1;
		}
		if(tempCounter == k - 3)
		{
			score = 4;
		}

		if(tempCounter == k - 2)
		{
			score = 8;
		}

		if(tempCounter == k - 1)
		{
			score = 12;
		}
}
void AIShell::VerticalWins(int** state, int col, int row, int turn, int& count, int& score)
{	
	
	// int tempCol = col; int tempRow = row;
	bool bottomBlocked = false; bool topBlocked = false;

	if(state[col][row] == turn){
		count++;

		if(row - 1 < 0)
			bottomBlocked = true;
		else if(state[col][row - 1] == -turn)
			bottomBlocked = true;

		while((row + 1 < numRows) && state[col][row + 1] == turn)
		{ 
			++row; ++count;	
		}

		if(row >= numRows)
			topBlocked = true;
		else if(state[col][row] == -turn)
			topBlocked = true;

		if(count == k)
			{
				score = 9999;
				return;
			}
		if(bottomBlocked && topBlocked){
			score = 0;
			return;
		}
		evaluatePoints(count, score);
	}
}

int AIShell::vertWinCount(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;

	VerticalWins(state, col, row, 1, aiCount, aiScore);
	VerticalWins(state, col, row, -1, otherCount, otherScore);
	return aiScore - otherScore;

}

int AIShell::countHorizontalWins(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; 
	bool leftBlocked = false; bool rightBlocked = false;
	bool otherLBlocked = false; bool otherRBlocked = false;
	if(state[col][row] == 1)
	{
		aiCount++;
		while(tempCol - 1 >= 0 && state[tempCol - 1][row] == 1)
		{
			++aiCount; --tempCol;
		}
		//Checks for blocked pieces
		if(tempCol < 0)
			leftBlocked = true;
		else if(state[tempCol][row] == -1)
			leftBlocked = true;

		tempCol = col; 
		while(tempCol + 1 < numCols && state[tempCol + 1][row] == 1)
		{
			++aiCount; ++tempCol;
		}

		if(tempCol = numCols)
			rightBlocked = true;
		else if(state[tempCol][row] == -1)
			rightBlocked = true;

		if(aiCount == k)
		{
			return 9999;
		}
		evaluatePoints(aiCount, aiScore);
		if(leftBlocked && rightBlocked)
			aiScore = 0;
	}
		
	if(state[col][row] == -1)
	{
		otherCount++;
		while(tempCol - 1 >= 0 && state[tempCol - 1][row] == -1)
		{
			++otherCount; --tempCol;
		}
		//Checks for blocked pieces
		if(tempCol < 0)
			otherLBlocked = true;
		else if(state[tempCol][row] == 1)
			otherLBlocked = true;

		tempCol = col; 
		while(tempCol + 1 < numCols && state[tempCol + 1][row] == -1)
		{
			++otherCount; ++tempCol;
		}

		if(tempCol = numCols)
			otherRBlocked = true;
		else if(state[tempCol][row] == 1)
			otherRBlocked = true;

		if(otherCount == k)
		{
			return 9999;
		}
		evaluatePoints(otherCount, otherScore);
		if(otherLBlocked && otherRBlocked)
			otherScore = 0;
	}
		return aiScore - otherScore;

}



void AIShell::diagonalLRLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state, int turn)
{
	bool topDiagBocked = false; bool bottomDiagBlocked = false;

	if(state[col][row] == turn){
		while((tempRow + 1 < numRows && tempCol + 1 < numCols) && state[tempCol + 1][tempRow + 1] == turn)
		{
			++count; ++tempRow; ++tempCol;
		}
		//Checks to see if diagonal top right is blocked
		if(tempRow < numRows && tempCol < numCols)
			if(state[tempCol][tempRow] == -turn)
				topDiagBocked = true;
		else if(tempRow == numRows || tempCol == numCols)
			topDiagBocked = true;

		//Reset column and row to original position before checking the bottom left diagonal
		tempCol = col; tempRow = row;
		while((tempRow - 1 >= 0 && tempCol - 1 >= 0) && state[tempCol - 1][tempRow - 1] == turn)
		{
			++count; --tempRow; --tempCol;
		}
		//Checks to see if diagonal bottom left is blocked
		if(tempCol >= 0 && tempRow >= 0)
			if(state[tempCol][tempRow] == -turn)
				bottomDiagBlocked = true;

		else if(tempRow < 0 || tempCol < 0)
			bottomDiagBlocked = true;

		//If both diagonals are blocking, don't count this score
		if(topDiagBocked && bottomDiagBlocked)
			count = 0;
		
	}
}

int AIShell::countDiagonalWinsLR(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; int tempRow = row;

	if(state[col][row] == 1)
	{
		aiCount++;
		diagonalLRLoop(tempCol, tempRow, col, row, aiCount, state, 1);

		if(aiCount == k)
			return 9999;
		
		evaluatePoints(aiCount, aiScore);
	}
	else if(state[col][row] == -1)
	{
		++otherCount;
		diagonalLRLoop(tempCol, tempRow, col, row, otherCount, state, -1);

		if(otherCount == k)
			return 9999;
		evaluatePoints(otherCount,otherScore);
	}

	return aiScore - otherScore;
}


void AIShell::diagonalRLLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state, int turn)
{
	bool leftBlocked = false; bool rightBlocked = false;	
	while((tempCol - 1 >= 0 && tempRow + 1 < numRows) && state[tempCol - 1][tempRow + 1] == turn)
	{
		++count; --tempCol; ++tempRow;
	}

	if(tempRow < numRows && tempCol >= 0)
		if(state[tempCol][tempRow] == -turn)
			leftBlocked = true;
	else if(tempRow == numRows || tempCol < 0)
		leftBlocked = true;

	tempCol = col; tempRow = row;
	while((tempCol + 1 < numCols && tempRow - 1 >= 0) && state[tempCol + 1][tempRow - 1] == turn)
	{
		++count; ++tempCol; --tempRow;
	}

	if(tempRow >= 0 && tempCol < numCols)
		if(state[tempCol][tempRow] == -turn)
			rightBlocked = true;
	else if(tempRow < 0 || tempCol == numCols)
		rightBlocked = true;

	if(leftBlocked && rightBlocked)
		count = 0;
}

int AIShell::countDiagonalWinsRL(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; int tempRow = row;

	if(state[col][row] == 1)
	{
		aiCount++;
		
		diagonalRLLoop(tempCol, tempRow, col, row, aiCount, state, 1);
		if(aiCount == k)
		{
			return 9999;
		}
		evaluatePoints(aiCount, aiScore);
	}
	else if(state[col][row] == 1)
	{
		otherCount++;
		
		diagonalRLLoop(tempCol, tempRow, col, row, otherCount, state, 1);
		if(otherCount == k)
		{
			return 9999;
		}
		evaluatePoints(otherCount, otherScore);
	}

	return aiScore - otherScore;
}

int checkForWin(int score)
{
	if(score >= 9999)
		return 9999999;
}

int AIShell::countTotalWins(int** state, int turn) 
{
	std::cout<<"CALLING COUNT TOTAL WINS!!!!!!!!!!!"<<std::endl;
	int aiScore = 0; int otherScore = 0;
	int aiHScore = 0; int otherHScore = 0;
	for(int col = 0; col < numCols; col++)
		for(int row = 0; row < numRows; row++)
		{

			//Vertical Win count
			int vertScore = vertWinCount(state, col, row);
			checkForWin(vertScore);
			aiScore += vertScore;
			
			//Horizontal wins count
			int horizScore = countHorizontalWins(state, col, row);
			checkForWin(horizScore);
			aiScore += horizScore;
			
			// Diagonal wins left to right
			int diagLRScore = countDiagonalWinsLR(state, col, row);
			checkForWin(diagLRScore);
			aiScore += diagLRScore;
			
			//Diagonal wins right to left
			int diagRLScore = countDiagonalWinsRL(state, col, row);
			checkForWin(diagRLScore);
			aiScore += diagRLScore;
			
			//countDiagonalWinsLR(state, col row);
		}	 	
		// if(turn % 2 == 0)
			return aiScore;
		// return otherScore + otherHScore;
}

std::vector<Move> AIShell::availableMoves(int** state){
	int count = 0;
	std::vector<Move> moveVector;
	for(int col = 0; col < numCols; col++)
		for(int row = 0; row < numRows; row++)
		{
			if(state[col][row] == NO_PIECE)
			{
				Move m = Move(col, row);
				moveVector.push_back(m);
			}
		}
	return moveVector;
}	

Move AIShell::makeMove(){
 	Move m;
 	std::cout<<"MAKING MOVE" << std::endl;
 	m = getBestMove(gameState, 1, 0, MIN, MAX);

 	return m;
}

// Move AIShell::iterativeSearch(int** state, int depth, int turn)
// {
// 	for(int i = 1000; i > 0; i--)
// 	{

// 	}
// }

Move AIShell::getBestMove(int** state, int depth, int turn, int alpha, int beta){
	std::vector<Move> moveVector = availableMoves(state);
	std::vector<Move> toDeleteMax;
	std::vector<Move> toDeleteMin;
	Move bestMove;
	if(moveVector.size() == numRows * numCols || moveVector.size() == numRows * numCols - 1)
	{
		if(state[numCols/2][numRows/2] == NO_PIECE)
			return Move(numCols/2, numRows/2);
		else
			return Move(numCols/2 - 1, numRows/2 - 1);
	}
	else if(depth == 0 || moveVector.size() == 0)
	{
		Move m(countTotalWins(state, turn));
		std::cout<< "--------------------SCORE GIVEN OF: "<<m.score<<" ---------------------------"<<std::endl;
		return m;
	}
	else if(turn % 2 == 0 )
	{
		std::cout<<"turn: "<< turn << std::endl;
		int count = 0;
		int bestVal = MIN;
		//Move m;
		for(int move = 0; move < moveVector.size(); move++)
		{
			std::cout<<"PRINT THE MOVES OUT FROM THE MOVE LIST"<<std::endl;
			std::cout<<moveVector[move].col << " " <<moveVector[move].row << " "<< moveVector[move].score << std::endl;
			int** nextGameState = clone(state);
			nextGameState[moveVector[move].col][moveVector[move].row] = AI_PIECE;

			std::cout<<"--------------------------AI----------------------------------"<<std::endl;
			// for(int col = 0; col < numCols; col++){
	 	// 			for(int row = 0; row < numRows; row++)
	 	// 			{
	 	// 				std::cout<<nextGameState[col][row] <<" " ;
	 	// 			}
	 	// 			std::cout<<std::endl;
	 	// 		}
			//toDeleteMax.push_back(Move(moveVector[move].col, moveVector[move].row));
			moveVector[move].score = getBestMove(nextGameState, depth - 1, ++turn, bestVal, beta).score;
			//m.col = moveVector[move].col; m.row = moveVector[move].row;

			// for(int i = 0; i < toDeleteMax.size(); i++)
			// 	state[toDeleteMax[i].col][toDeleteMax[i].row] = 0;
			// std::cout<<bestVal<<"<-----------------------------"<<std::endl;
			if(bestVal < moveVector[move].score)
			{
				bestMove = moveVector[move];
				bestVal = moveVector[move].score;
			}
			if(alpha < bestVal)
				alpha = bestVal;

			if(beta <= bestVal)
				break;
			for (int i =0; i<numCols; i++){
				delete [] nextGameState[i];
					}
				delete [] nextGameState;
		}
		return bestMove;
	}
	else
	{
		int bestVal = MAX;
		//Move m;
		int count = 0;
		std::vector<Move> moveVector = availableMoves(state);
		for(int move = 0; move < moveVector.size(); move++){
			int** nextGameState = clone(state);
			nextGameState[moveVector[move].col][moveVector[move].row] = HUMAN_PIECE;
			// std::cout<<"----------------------------HUMAN---------------------------------"<<std::endl;
			// for(int col = 0; col < numCols; col++){
	 	// 			for(int row = 0; row < numRows; row++)
	 	// 			{
	 	// 				std::cout<<nextGameState[col][row] <<" " ;
	 	// 			}
	 	// 			std::cout<<std::endl;
	 	// 		}
			// std::cout<<"THE TURN IS --------------  HUMAN "<<turn<<std::endl;
			//toDeleteMin.push_back(Move(moveVector[move].col, moveVector[move].row));
			
			moveVector[move].score = getBestMove(nextGameState, depth-1, ++turn, alpha, bestVal).score;
			//m.col = moveVector[move].col; m.row = moveVector[move].row;

			// for(int i = 0; i < toDeleteMin.size(); i++)
			// 	state[toDeleteMin[i].col][toDeleteMin[i].row] = 0;

			if(bestVal > moveVector[move].score)
			{
				bestMove = moveVector[move];
				bestVal = moveVector[move].score;
			}
			if(beta > bestVal)
				beta = bestVal;

			if(bestVal <= alpha)
				break;

			for (int i =0; i<numCols; i++){
				delete [] nextGameState[i];
					}
				delete [] nextGameState;
		}
		return bestMove;
	}
}
