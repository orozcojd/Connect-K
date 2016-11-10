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
			score = 5 * 10;
		}
		if(tempCounter == k - 3)
		{
			score = 5 * 20;
		}

		if(tempCounter == k - 2)
		{
			score = 5 * 50;
		}

		if(tempCounter == k - 1)
		{
			score = 5 * 80;
		}
	
}
std::tuple<int, int> AIShell::countVerticalWins(int** state, int col, int row)
{	
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	if(state[col][row] == 1)
	{
		++aiCount;
		while((row + 1 < numRows) && state[col][row + 1] == 1)
		{ 
			++row; ++aiCount;	
		}
		if(aiCount == k)
			{
				return std::make_tuple(9999999999, 0);
			}
		evaluatePoints(aiCount, aiScore);
	}

	if(state[col][row] == -1)
	{
		++otherCount;
		while((row + 1 < numRows) && state[col][row + 1] == -1)
		{
			++row; ++otherCount;
		}
		if(otherCount == k)
			{
				return std::make_tuple(0, -9999999999);
			}
		evaluatePoints(otherCount, otherScore);
	}
	return std::make_tuple(aiScore, -otherScore);

}



std::tuple<int, int> AIShell::countHorizontalWins(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; 
	if(state[col][row] == 1)
	{
		++aiCount;
		while(tempCol - 1 >= 0 && state[tempCol - 1][row] == 1)
		{
			++aiCount; --tempCol;
		}
		tempCol = col; 
		while(tempCol + 1 < numCols && state[tempCol + 1][row] == 1)
		{
			++aiCount; ++tempCol;
		}
		if(aiCount == k)
		{
			return std::make_tuple(9999999999, 0);
		}
		evaluatePoints(aiCount, aiScore);

	}
	else if(state[col][row] == -1)
	{
		++otherCount;
		while(tempCol - 1 >= 0 && state[tempCol - 1][row] == -1)
		{
			++otherCount; --tempCol;
		}
		tempCol = col; 
		while(tempCol + 1 < numCols && state[tempCol + 1][row] == -1)
		{
			++otherCount; ++tempCol;
		}
		
		if(otherCount == k)
		{
			return std::make_tuple(0, -9999999999);
		}
		evaluatePoints(otherCount, otherScore);
	}

		return std::make_tuple(aiScore, -otherScore);

}


void AIShell::diagonalLRLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state)
{
		while((tempRow + 1 < numRows && tempCol + 1 < numCols) && state[tempCol + 1][tempRow + 1] == 1)
		{
			++count; ++tempRow; ++tempCol;
		}
		tempCol = col; tempRow = row;

		while((tempRow - 1 > numRows && tempCol - 1 > numCols) && state[tempCol - 1][tempRow - 1] == 1)
		{
			++count; ++tempRow; ++tempCol;
		}
}

std::tuple<int, int> AIShell::countDiagonalWinsLR(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; int tempRow = row;

	if(state[col][row] == 1)
	{
		++aiCount;
		
		diagonalLRLoop(tempCol, tempRow, col, row, aiCount, state);

		if(aiCount == k)
		{
			return std::make_tuple(9999999999, 0);
		}
		evaluatePoints(aiCount, aiScore);
	}
	else if(state[col][row] == -1)
	{
		++otherCount;
		diagonalLRLoop(tempCol, tempRow, col, row, otherCount, state);

		if(otherCount == k)
		{
			return std::make_tuple(0, -9999999999);
		}
		evaluatePoints(otherCount, otherScore);

	}
	return std::make_tuple(aiScore, -otherScore);
}


void AIShell::diagonalRLLoop(int& tempCol, int& tempRow, int col, int row, int& count, int** state)
{
		while((tempRow + 1 < numRows && tempCol - 1 > numCols) && state[tempCol - 1][tempRow + 1] == 1)
		{
			++count; ++tempRow; ++tempCol;
		}
		if(state[tempCol][tempRow] == -1)
			count = 0;
		//tempCol = col; tempRow = row;

		while((tempRow - 1 > numRows && tempCol + 1 > numCols) && state[tempCol + 1][tempRow - 1] == 1)
		{
			++count; ++tempRow; ++tempCol;
		}

}

std::tuple<int, int> AIShell::countDiagonalWinsRL(int** state, int col, int row)
{
	int aiCount = 0; int otherCount = 0;
	int aiScore = 0; int otherScore = 0;
	int tempCol = col; int tempRow = row;

	if(state[col][row] == 1)
	{
		++aiCount;
		
		diagonalRLLoop(tempCol, tempRow, col, row, aiCount, state);
		if(aiCount == 0)
			aiScore = 1;
		if(aiCount == k)
		{
			return std::make_tuple(9999999999, 0);
		}
		evaluatePoints(aiCount, aiScore);
	}
	else if(state[col][row] == -1)
	{
		++otherCount;
		diagonalRLLoop(tempCol, tempRow, col, row, otherCount, state);

		if(otherCount == k)
		{
			return std::make_tuple(0, -9999999999);
		}
		evaluatePoints(otherCount, otherScore);

	}
	return std::make_tuple(aiScore, -otherScore);
}

int AIShell::countTotalWins(int** state) 
{
	std::cout<<"CALLING COUNT TOTAL WINS!!!!!!!!!!!"<<std::endl;
	int aiScore = 0; int otherScore = 0;
	int aiHScore = 0; int otherHScore = 0;
	for(int col = 0; col < numCols; col++)
		for(int row = 0; row < numRows; row++)
		{

			//Vertical Win count
			std::tuple<int, int> vertScore = countVerticalWins(state, col, row);
			aiScore += std::get<0>(vertScore);
			otherScore += std::get<1>(vertScore);

			//Horizontal wins count
			std::tuple<int, int> horizScore = countHorizontalWins(state, col, row);
			aiHScore += std::get<0>(horizScore);
			otherHScore += std::get<1>(horizScore);

			//Diagonal wins left to right
			std::tuple<int, int> diagLRScore = countDiagonalWinsLR(state, col, row);
			aiHScore += std::get<0>(diagLRScore);
			otherHScore += std::get<1>(diagLRScore);

			//Diagonal wins right to left
			std::tuple<int, int> diagRLScore = countDiagonalWinsRL(state, col, row);
			aiHScore += std::get<0>(diagRLScore);
			otherHScore += std::get<1>(diagRLScore);

			//countDiagonalWinsLR(state, col row);
			if(aiScore >= 9999999999)
				return 9999999999;
			else if(otherScore == -9999999999)
				return -9999999999;
		}		
		return aiScore + otherScore + aiHScore + otherHScore;
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
 	m = getBestMove(gameState, 1, 0, -9999999999, 9999999999);

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
		Move m(countTotalWins(state));
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
