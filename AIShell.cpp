#include "AIShell.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <tuple>

std::vector<Move> toDelete;

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
int AIShell::countVerticalWins(int** state, int col, int row)
{	
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	if(state[col][row] == 1)
	{
		aiCount++;
		while((row + 1 < numRows) && state[col][row + 1] == 1)
		{ 
			++row; ++aiCount;	
		}
		if(aiCount == k)
			{
				return 999999999;
			}
		evaluatePoints(aiCount, aiScore);
	}

	return aiScore;

}

int AIShell::countHorizontalWins(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; 
	bool leftBlocked = false; bool rightBlocked = false;
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
			return 999999999;
		}
		evaluatePoints(aiCount, aiScore);
	}
		if(leftBlocked && rightBlocked)
			aiScore = 0;

		return aiScore;

}


void AIShell::diagonalLRLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state)
{
	bool topDiagBocked = false; bool bottomDiagBlocked = false;

	while((tempRow + 1 < numRows && tempCol + 1 < numCols) && state[tempCol + 1][tempRow + 1] == 1)
	{
		++count; ++tempRow; ++tempCol;
	}
	//Checks to see if diagonal top right is blocked
	if(tempRow < numRows && tempCol < numCols)
		if(state[tempCol][tempRow] == -1)
			topDiagBocked = true;
	else if(tempRow == numRows || tempCol == numCols)
		topDiagBocked = true;

	//Reset column and row to iriginal position before checking the bottom left diagonal
	tempCol = col; tempRow = row;
	while((tempRow - 1 > numRows && tempCol - 1 > numCols) && state[tempCol - 1][tempRow - 1] == 1)
	{
		++count; --tempRow; --tempCol;
	}
	//Checks to see if diagonal bottom left is blocked
	if(tempCol >= 0 && tempRow >= 0)
		if(tempRow < numRows || tempCol < numCols)
			bottomDiagBlocked = true;
	else if(tempRow < numRows || tempCol < numCols)
		bottomDiagBlocked = true;

	//If both diagonals are blocking, don't count this score
	if(topDiagBocked && bottomDiagBlocked)
		count = 0;
}

int AIShell::countDiagonalWinsLR(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; int tempRow = row;

	if(state[col][row] == 1)
	{
		aiCount++;
		diagonalLRLoop(tempCol, tempRow, col, row, aiCount, state);

		if(aiCount == k)
		{
			return 999999999;
		}
		evaluatePoints(aiCount, aiScore);
	}

	return aiScore;
}


void AIShell::diagonalRLLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state)
{
	bool leftBlocked = false; bool rightBlocked = false;	
	while((tempCol - 1 > numCols && tempRow + 1 < numRows) && state[tempCol - 1][tempRow + 1] == 1)
	{
		++count; --tempCol; ++tempRow;
	}

	if(tempRow < numRows && tempCol >= 0)
		if(state[tempCol][tempRow] == - 1)
			leftBlocked = true;
	else if(tempRow == numRows || tempCol < 0)
		leftBlocked = true;

	tempCol = col; tempRow = row;
	while((tempCol + 1 > numCols && tempRow - 1 > numRows) && state[tempCol + 1][tempRow - 1] == 1)
	{
		++count; ++tempCol; --tempRow;
	}

	if(tempRow >= 0 && tempCol < numCols)
		if(state[tempCol][tempRow] == -1)
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
		
		diagonalRLLoop(tempCol, tempRow, col, row, aiCount, state);
		if(aiCount == k)
		{
			return 999999999;
		}
		evaluatePoints(aiCount, aiScore);
	}

	return aiScore;
}

int AIShell::countTotalWins(int** state, int turn) 
{
	std::cout<<"CALLING COUNT TOTAL WINS!!!!!!!!!!!"<<std::endl;
	int aiScore = 0; int otherScore = 0;
	int aiHScore = 0; int otherHScore = 0;
	int diagLR = 0; int diagRL = 0;
	for(int col = 0; col < numCols; col++)
		for(int row = 0; row < numRows; row++)
		{

			//Vertical Win count
			int vertScore = countVerticalWins(state, col, row);
			if(vertScore > aiScore)
				aiScore = vertScore;
			

			//Horizontal wins count
			int horizScore = countHorizontalWins(state, col, row);
			if(horizScore > aiHScore)
				aiHScore = horizScore;
			

			//Diagonal wins left to right
			int diagLRScore = countDiagonalWinsLR(state, col, row);
			if(diagLRScore > diagLR)
				diagLR = diagLRScore;
			

			//Diagonal wins right to left
			int diagRLScore = countDiagonalWinsRL(state, col, row);
			if(diagRLScore > diagRL)
				diagRL = diagRLScore;
			

			//countDiagonalWinsLR(state, col row);
			if(aiScore >= 999999999)
				return 999999999;
			else if(otherScore == -999999999)
				return -999999999;
		}	 	
		// if(turn % 2 == 0)
			return aiScore + aiHScore +diagLR + diagRL;
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
 	m = getBestMove(gameState, 2, 0, -999999999, 999999999);

 	return m;
}

Move AIShell::getBestMove(int** state, int depth, int turn, int alpha, int beta){
	std::vector<Move> moveVector = availableMoves(state);
	std::vector<Move> toDelete;
	int bestVal;
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
		bestVal = alpha;
		Move m;
		for(int move = 0; move < moveVector.size(); move++)
		{
			std::cout<<"PRINT THE MOVES OUT FROM THE MOVE LIST"<<std::endl;
			std::cout<<moveVector[move].col << " " <<moveVector[move].row << " "<< moveVector[move].score << std::endl;
			//int** nextGameState = clone(state);
			state[moveVector[move].col][moveVector[move].row] = AI_PIECE;

			std::cout<<"--------------------------AI----------------------------------"<<std::endl;
			// for(int col = 0; col < numCols; col++){
	 	// 			for(int row = 0; row < numRows; row++)
	 	// 			{
	 	// 				std::cout<<nextGameState[col][row] <<" " ;
	 	// 			}
	 	// 			std::cout<<std::endl;
	 	// 		}
			toDelete.push_back(Move(moveVector[move].col, moveVector[move].row));
			m = getBestMove(state, depth - 1, ++turn, bestVal, beta);
			m.col = moveVector[move].col; m.row = moveVector[move].row;
			for(int i = 0; i < toDelete.size(); i++)
				state[toDelete[i].col][toDelete[i].row] = 0;
			// std::cout<<bestVal<<"<-----------------------------"<<std::endl;
			if(bestVal < m.score)
			{
				bestMove = m;
				bestVal = m.score;
			}

			if(beta <= bestVal)
				break;
			// for (int i =0; i<numCols; i++){
			// 	delete [] nextGameState[i];
			// 		}
			// 	delete [] nextGameState;
		}
		return bestMove;
	}
	else
	{
		bestVal = beta;
		Move m;
		int count = 0;
		std::vector<Move> moveVector = availableMoves(state);
		for(int move = 0; move < moveVector.size(); move++){
		//	int** nextGameState = clone(state);
			state[moveVector[move].col][moveVector[move].row] = HUMAN_PIECE;
			// std::cout<<"----------------------------HUMAN---------------------------------"<<std::endl;
			// for(int col = 0; col < numCols; col++){
	 	// 			for(int row = 0; row < numRows; row++)
	 	// 			{
	 	// 				std::cout<<nextGameState[col][row] <<" " ;
	 	// 			}
	 	// 			std::cout<<std::endl;
	 	// 		}
			// std::cout<<"THE TURN IS --------------  HUMAN "<<turn<<std::endl;
			toDelete.push_back(Move(moveVector[move].col, moveVector[move].row));
			m = getBestMove(state, depth-1, ++turn, alpha, bestVal);
			m.col = moveVector[move].col; m.row = moveVector[move].row;
			for(int i = 0; i < toDelete.size(); i++)
				state[toDelete[i].col][toDelete[i].row] = 0;

			if(bestVal > m.score)
			{
				bestMove = m;
				bestVal = m.score;
			}
			if(bestVal <= alpha)
				break;

			// for (int i =0; i<numCols; i++){
			// 	delete [] nextGameState[i];
			// 		}
			// 	delete [] nextGameState;
		}
		return bestMove;
	}
}
