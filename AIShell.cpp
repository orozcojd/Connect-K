#include "AIShell.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
	this->deadline=0;
	this->numRows=numRows;
	this->numCols=numCols;
	this->gravityOn=gravityOn;
	this->gameState=gameState;
	this->lastMove=lastMove;
	this->Winner = -2;
	// this->AImove = 0;
}

AIShell::AIShell(const AIShell &toCopy)
{
	std::cout<<"******************Copy constructor called*****************"<<std::endl;
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
	int** cloned =NULL ;
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


/**********************************************************/

int AIShell::countTotalWins(int** state) 
{


// -------vvvvvvvv--------Check Vertical Variables-------vvvv-------//

	int aiWinner = 0; 		//used to count for potential connect k (includes empty)
	int otherWinner = 0; 	// used to count for potential connect k (includes empty)

	int totalAi = 0; 		//used to count for total pieces
	int totalOther = 0; 	// used to count for total pieces

	int totalPotentialAIWins = 0;
	int totalPotentialOtherWins = 0;

	int singleAi = 0;
	int singleOther = 0;
// -------vvvvvvvv--------Check Horizeontal Variables-------vvvv-------//

	int aiWinnerHorizontal = 0;
	int otherWinnerHorizontal = 0;

	int totalAiHorizontal = 0;
	int totalOtherHorizontal = 0;

	int totalPotAiHorizontal = 0;
	int totalPotOtherHorizontal= 0;



// -------vvvvvvvv--------Check Diagonal Left Right Variables-------vvvv-------//




// -------vvvvvvvv--------Check Diagonal Right Left Variables-------vvvv-------//



// -------vvvvvvvv--------Giant For Loop Nested If Statements-------vvvv-------//

	//count number of wins in columns
	for(int col = 0; col < numCols; col++)
	{
		aiWinner = 0; otherWinner = 0;
		for(int row = 0; row < numRows; row++)
		{
			int tempRow = row;
			int tempCol = col;
			if(state[col][row] == 1)
			{
				++aiWinner;
					while((++tempRow < numRows) && state[col][tempRow] == 1)
					{
						otherWinner = 0;
						aiWinner++;
						if(aiWinner == k - 3 || aiWinner == 2)
						{
							totalPotentialAIWins = 10 * 50;
							if(totalAi < totalPotentialAIWins)
								totalAi = totalPotentialAIWins;
						}
						if(aiWinner == k - 2)
						{
							totalPotentialAIWins = 10 * 60;
							if(totalAi < totalPotentialAIWins)
								totalAi = totalPotentialAIWins;
						}
						if(aiWinner == k - 1)
						{
							totalPotentialAIWins = 10 * 80;
							if(totalAi < totalPotentialAIWins)
								totalAi = totalPotentialAIWins;
						}
						if (aiWinner >= k)
						{
							std::cout<<"statement  this one right here"<<std::endl;
							return 9999;
							
						}
					}
				if(tempRow < row && state[col][tempRow] == -1)
					totalAi -= 50;
				totalPotentialAIWins = 0;
				aiWinner = 0;
			}
			
			else if(state[col][row] == -1)
			{	
				++otherWinner;
				aiWinner = 0;
				tempRow = row;
				
				while((++tempRow < numRows) && state[col][tempRow] == -1)
				{
					aiWinner = 0;
					otherWinner++;

					if(otherWinner == k - 3 || otherWinner == 2)
					{
						totalPotentialOtherWins = 10 * 50;
						if(totalOther < totalPotentialOtherWins)
							totalOther = totalPotentialOtherWins;
					}

					if(otherWinner == k - 2)
					{
						totalPotentialOtherWins = 10 * 60;
						if(totalOther < totalPotentialOtherWins)
							totalOther = totalPotentialOtherWins;
					}
					if(otherWinner == k - 1)
					{
						totalPotentialOtherWins = 10 * 80;
						if(totalOther < totalPotentialOtherWins)
							totalOther = totalPotentialOtherWins;
					}
					if (otherWinner >= k)
					{
						return -9999;
					}
				}
				
				if(tempRow < row && state[col][tempRow] == 1)
					totalOther -= 50;
				totalPotentialOtherWins = 0;
			}
			else if(state[col][row] == 0)
				{
					singleAi+= 10;
					singleOther+=10;
				}
		}

	}

	//HORIZONTAL WINS
	for(int row = 0; row < numRows; row++)
	{
		aiWinnerHorizontal = 0; otherWinnerHorizontal = 0;
		for(int col = 0; col < numCols; col++)
		{
			int tempCol = col;
			int tempRow = row;
			if(state[col][row] == 1)
			{
				aiWinnerHorizontal++;
				// MAY NOT NEED THIS
				while((--tempCol >= 0) && state[tempCol][row] == 1)
				{
					otherWinnerHorizontal = 0;
					aiWinnerHorizontal++;

					if(aiWinnerHorizontal == k - 3 || aiWinnerHorizontal == 2)
					{
						totalPotAiHorizontal = 10 * 50;
						if(totalAiHorizontal <  totalPotAiHorizontal)
							totalAiHorizontal = totalPotAiHorizontal;
					}
					if(aiWinnerHorizontal == k - 2)
					{
						totalPotAiHorizontal = 10 * 60;
						if(totalAiHorizontal <  totalPotAiHorizontal)
							totalAiHorizontal = totalPotAiHorizontal;
					}
					if(aiWinnerHorizontal == k - 1)
					{
						totalPotAiHorizontal = 10 * 80;
						if(totalAiHorizontal <  totalPotAiHorizontal)
							totalAiHorizontal = totalPotAiHorizontal;
					}
					if (aiWinnerHorizontal >= k)
					{
						std::cout<<"statement here this one"<<std::endl;
						return 9999;
					}
				}

				if(tempCol >= 0 && state[tempCol][row] == -1)
					totalAiHorizontal -= 50 ;
				++singleAi;
				tempCol = col;

				while((++tempCol < numCols) && state[tempCol][row] == 1){
					otherWinnerHorizontal = 0;
					aiWinnerHorizontal++;

					if(aiWinnerHorizontal == k - 3 || aiWinnerHorizontal == 2)
					{
						totalPotAiHorizontal = 10 * 50;
						if(totalAiHorizontal <  totalPotAiHorizontal)
						   totalAiHorizontal = totalPotAiHorizontal;
					}

					if(aiWinnerHorizontal == k - 2)
					{
						totalPotAiHorizontal = 10 * 60;
						if(totalAiHorizontal <  totalPotAiHorizontal)
						   totalAiHorizontal = totalPotAiHorizontal;
					}

					if(aiWinnerHorizontal == k - 1)
					{
						totalPotAiHorizontal = 10 * 80;
						if(totalAiHorizontal <  totalPotAiHorizontal)
						   totalAiHorizontal = totalPotAiHorizontal;
					}
					if (aiWinnerHorizontal >= k)
					{
						std::cout<<"statement here one two"<<std::endl;
						std::cout<<aiWinnerHorizontal<<std::endl;

						return 9999;
					}
				}

				if(tempCol < numCols && state[tempCol][row] == -1)
					totalAiHorizontal -= 100;
				++singleOther;
				aiWinnerHorizontal = 0;


			}

			else if(state[col][row] == -1)
			{	
				tempCol = col;
				aiWinnerHorizontal = 0;
				++otherWinnerHorizontal;

				while((--tempCol >= 0) && state[tempCol][row] == -1)
				{
					aiWinnerHorizontal = 0;
					++otherWinnerHorizontal;
						
					if(otherWinnerHorizontal == k - 3 || otherWinnerHorizontal == 2)
					{
						totalPotOtherHorizontal = 10 * 40;
						if(totalOtherHorizontal < totalPotOtherHorizontal)
							totalOtherHorizontal = totalPotOtherHorizontal;
					}

					if(otherWinnerHorizontal == k - 2)
					{
						totalPotOtherHorizontal = 10 * 60;
						if(totalOtherHorizontal < totalPotOtherHorizontal)
							totalOtherHorizontal = totalPotOtherHorizontal;
					}
					if(otherWinnerHorizontal == k - 1)
					{
						totalPotOtherHorizontal = 10 * 80;
						if(totalOtherHorizontal < totalPotOtherHorizontal)
							totalOtherHorizontal = totalPotOtherHorizontal;
					}
					if (otherWinnerHorizontal >= k)
					{
						if(totalOtherHorizontal < totalPotOtherHorizontal)
						{
							std::cout<<"statement here ------"<<std::endl;
							return -9999;
						}
					}

				}
				if(tempCol >= 0 && state[tempCol][row] == 1)
					totalAiHorizontal -= 100;
				aiWinnerHorizontal = 0;

				tempCol = col;
				while((++tempCol < numCols) && state[tempCol][row] == -1){

					aiWinnerHorizontal = 0;
					++otherWinnerHorizontal;
						
					if(otherWinnerHorizontal == k - 3 || otherWinnerHorizontal == 2)
					{
						totalPotOtherHorizontal = 10 * 40;
						if(totalOtherHorizontal < totalPotOtherHorizontal)
							totalOtherHorizontal = totalPotOtherHorizontal;
					}

					if(otherWinnerHorizontal == k - 2)
					{
						totalPotOtherHorizontal = 5 * 60;
						if(totalOtherHorizontal < totalPotOtherHorizontal)
							totalOtherHorizontal = totalPotOtherHorizontal;
					}
					if(otherWinnerHorizontal == k - 1)
					{
						totalPotOtherHorizontal = 10 * 80;
						if(totalOtherHorizontal < totalPotOtherHorizontal)
							totalOtherHorizontal = totalPotOtherHorizontal;
					}
					if (otherWinnerHorizontal >= k)
					{
						if(totalOtherHorizontal < totalPotOtherHorizontal)
						{
							std::cout<<"statement here"<<std::endl;
							return -9999;
						}
					}
				}
				if(tempCol < numCols && state[tempCol][row] == 1)
					totalAiHorizontal -= 100;
			}
			else if(state[col][row] == 0)
				{
					singleAi+= 10;
					singleOther+=10;
				}
		}

	}

	return  totalAi + totalAiHorizontal + singleAi - singleOther - totalOther - totalOtherHorizontal ;
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
	//int rand(void);
	//this part should be filled in by the student to implement the AI
	//Example of a move could be: Move move(1, 2); //this will make a move at col 1, row 2
	
	// std::tuple<int, int> intialMove = evaluateGame(gameState);
 // 	 if(std::get<0>(intialMove) == 0){
 // 	 	std::cout<<"THIS IS TRUE ----------------"<<std::endl;
 // 	 	return createRandomMove(gameState);
 // 	 }


 	Move m;
 	std::cout<<"MAKING MOVE" << std::endl;
 	m = getBestMove(gameState, 3, 0, -9999, 9999);

 	return m;
}

Move AIShell::getBestMove(int** state, int depth, int turn, int alpha, int beta){
	std::vector<Move> moveVector = availableMoves(state);
	int bestVal;
	Move bestMove;
	if(moveVector.size() == numRows * numCols - 1 || moveVector.size() == numRows * numCols - 2)
	{
		if(state[numCols/2][numRows/2] == NO_PIECE)
			return Move(numCols/2, numRows/2);
		else
			return Move(numCols/2 - 1, numRows/2 - 1);
	}
	if(depth == 0 || moveVector.size() == 0)
	{
		Move m(countTotalWins(state));
		std::cout<< "--------------------SCORE GIVEN OF: "<<m.score<<" ---------------------------"<<std::endl;
		return m;
	}
	if(turn % 2 == 0 )
	{
		std::cout<<"turn: "<< turn << std::endl;
		int count = 0;
		bestVal = alpha;
		Move m;
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
			std::cout<<"THE TURN IS --------------  AI "<<turn<< std::endl;
			m = getBestMove(nextGameState, depth-1, ++turn, bestVal, beta);
			std::cout<<"---PRINTING MOVE DIMENSIONS: "<<moveVector[move].col << " " <<moveVector[move].row << " "<< moveVector[move].score << std::endl;
			m.col = moveVector[move].col; m.row = moveVector[move].row;
			// std::cout<<bestVal<<"<-----------------------------"<<std::endl;
			if(bestVal < m.score)
			{
				bestMove = m;
				bestVal = m.score;
			}

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
		// std::cout<<"turn: "<< turn << std::endl;
		bestVal = beta;
		Move m;
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
			m = getBestMove(nextGameState, depth-1, ++turn, alpha, bestVal);
			m.col = moveVector[move].col; m.row = moveVector[move].row;
			if(bestVal > m.score)
			{
				bestMove = m;
				bestVal = m.score;
			}
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
