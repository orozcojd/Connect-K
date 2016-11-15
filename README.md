# Discrete-Project
## This is a connect-k game; any sized board connecting k pieces to win

This game uses a mini-max algorithm with alpha beta pruning and iterative deepening to increase
the search time.
It has a heuristic function that loops over winning streaks on the board in all directions and 
gives each k-n a weight to help determine the better move. 
It also blocks moves when the other player is about to win.

## To run this project on Windows:
### g++ ConnectK.cpp Move.cpp AIShell.cpp -o myAI 
### java -jar ConnectK_1.8.jar cpp:myAI.exe

## To run this project on Linux:
### g++ ConnectK.cpp Move.cpp AIShell.cpp -o myAI
### java -jar ConnectK_1.8.jar cpp:myAI
