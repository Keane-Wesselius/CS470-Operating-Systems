#include "tictaktoe.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>  


int main(int argc, char* argv[])
{
    

    //srand(time(NULL));
    /*bool winner = false;
    bool loser = false;
    bool draw = false;*/
    string userInput = "";
    int boardSize;
    if (argc > 1)
    {
        userInput = argv[1];
    }
    else
    {
        cout << "Error: You need to provide a positive integer as an arguement for the board size" << endl;
        return -1;
        //cout << "Enter the size of the NxN Board:" << endl;
        //getline(cin, userInput);
    }


    
    if (validateInput(userInput))
    {
        boardSize = stoi(userInput);
    }
    else
    {
        cout << "ERROR: invalid arguement, valid arguement for Boardsize is an integer from 3 to 20" << endl;
        exit(-1);
    }
    //vector<vector<string>> gameBoard = createBoard(boardSize);




    //Share memory for the struc between the two processes
    key_t key = ftok("tictaktoe.h", 'A');
    int shmid = shmget(key, sizeof(struct shared), 0666 | IPC_CREAT);
    struct shared* shm = (struct shared*)shmat(shmid, NULL, 0);


    //Populates the board with empty values to begin with
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            shm->gameBoard[i][j] = ' ';
        }
    }


    shm->maxPlay = boardSize * boardSize;
    shm->boardsize = boardSize;

    shm->wait = false;

    



    //JUST TESTING
    while (shm->numPlay < shm->maxPlay)
    {
        

        if (checkWinner(shm->gameBoard, shm->boardsize) == "X")
        {
            cout << "P1 process won!" << endl;
            break;
        }
        if (checkWinner(shm->gameBoard, shm->boardsize) == "O")
        {
            cout << "P1 process lost..." << endl;
            break;
        }

        if (shm->turn % 2 == 0) {

            if (checkWinner(shm->gameBoard, shm->boardsize) == "X")
            {
                cout << "P1 process won!" << endl;
                break;
            }
            if (checkWinner(shm->gameBoard, shm->boardsize) == "O")
            {
                cout << "P1 process lost..." << endl;
                break;
            }
            if (shm->numPlay == shm->maxPlay)
            {
                break;
            }

            printBoard(shm->gameBoard, shm->boardsize);
            cout << "P1's Turn" << endl;
            playerPlay(shm->gameBoard, 'X', shm->boardsize);
            printBoard(shm->gameBoard, shm->boardsize);
            shm->numPlay += 1;
            shm->turn += 1;
        }
    }

    if (checkWinner(shm->gameBoard, shm->boardsize) == "O")
    {
        printBoard(shm->gameBoard, shm->boardsize);
    }
    if (shm->numPlay == shm->maxPlay && checkWinner(shm->gameBoard, shm->boardsize) == "Nope")
    {
        cout << "Draw!" << endl;
    }
    
    fflush(stdout);
    ////frees the gameboard 2d array
    //for (int i = 0; i < shm->boardsize; i++)
    //{
    //    delete[] shm->gameBoard[i];
    //}
    //delete[] shm->gameBoard;

    //detatch and release the shared memory
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
