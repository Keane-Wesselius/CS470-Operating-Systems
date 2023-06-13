#include "tictaktoe.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>  


int main() {
    key_t key = ftok("tictaktoe.h", 'A');
    int shmid = shmget(key, sizeof(struct shared), 0666 | IPC_CREAT);
    struct shared* shm = (struct shared*)shmat(shmid, NULL, 0);



    while (shm->numPlay < shm->maxPlay)
    {
        if (checkWinner(shm->gameBoard, shm->boardsize) == "X")
        {
            cout << "P2 process lost..." << endl;
            break;
        }

        if (checkWinner(shm->gameBoard, shm->boardsize) == "O")
        {
            cout << "P2 process won!" << endl;
            break;
        }

        
        

        if (shm->turn % 2 == 1) {

            if (checkWinner(shm->gameBoard, shm->boardsize) == "X")
            {
                cout << "P2 process lost..." << endl;
                break;
            }
            if (checkWinner(shm->gameBoard, shm->boardsize) == "O")
            {
                cout << "P2 process won!" << endl;
                break;
            }
            if (shm->numPlay == shm->maxPlay)
            {
                break;
            }

            cout << "P2's Turn!" << endl;
            playerPlay(shm->gameBoard, 'O', shm->boardsize);
            shm->numPlay += 1;
            shm->turn += 1;
        }
        
        
    }

    if (shm->numPlay == shm->maxPlay && checkWinner(shm->gameBoard, shm->boardsize) == "Nope")
    {
        cout << "Draw!" << endl;
    }


    //detatch and //release the shared memory
    shmdt(shm);
    //shmctl(shmid, IPC_RMID, NULL);
    return 0;
}