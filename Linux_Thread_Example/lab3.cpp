#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "problem1.h"
#include "problem2.h"
#include <pthread.h>
using namespace std;


const int MAX_ELEMENTS = 100;
pthread_mutex_t mutex;
bool p1IsDone = false;
bool p2IsDone = false;

void* arrayChecker(void* arg)
{
    p2Struct* theStruct = (p2Struct*)arg;
    while (!p2IsDone) {
        pthread_mutex_lock(&theStruct->mutex);
        p2IsDone = checkSorted(*theStruct->numbers, theStruct->n);
        pthread_mutex_unlock(&theStruct->mutex);
    }
    //return NULL;
    pthread_exit(NULL);
}

void* threadSorter(void* arg) {
    p2Struct* theStruct = (p2Struct*)arg;
    while (!p2IsDone) {
        int index = rand() % theStruct->n;
        pthread_mutex_lock(&theStruct->mutex);
        arraySort(*theStruct->numbers, theStruct->n, index);
        cout << "right after the sort" << endl;
        cout << "Thread#" << theStruct->threadNum << " accessed the array at index:" << index << endl;
        printArray(*theStruct->numbers, theStruct->n);
        fflush(stdout);
        pthread_mutex_unlock(&theStruct->mutex);
        fflush(stdout);
    }
    //return NULL;
    pthread_exit(NULL);
}

void* threadChecker(void* arg)
{
    p1Struct* theStruct = (p1Struct*)arg;
    while (!p1IsDone) {
        pthread_mutex_lock(&theStruct->mutex);
        p1IsDone = matrixIsDone(*theStruct->matrix, theStruct->n);
        pthread_mutex_unlock(&theStruct->mutex);
    }
    //return NULL;
    pthread_exit(NULL);
}

void* threadFlipper(void* arg) {
    p1Struct* theStruct = (p1Struct*)arg;
    while (!p1IsDone) {
        int i = rand() % theStruct->n;
        int j = rand() % theStruct->n;
        pthread_mutex_lock(&theStruct->mutex);
        int answer = matrixNeighbors(*theStruct->matrix, i, j, theStruct->n);
        cout << "Thread#" << theStruct->threadNum << " put value " << answer << " at position: (" << i << "," << j << ")" << endl;
        printMatrix(*theStruct->matrix, theStruct->n);
        //fflush(stdout);
        pthread_mutex_unlock(&theStruct->mutex);
    }
    pthread_exit(NULL);
    //return NULL;
}


bool isPositiveInteger(string userInput) {
    if (userInput.find_first_not_of("0123456789") != string::npos || userInput == "") {
        return false;
    }
    return true;
}

bool validateInput(string userInput, int min, int max)
{
    int option = 0;
    if (isPositiveInteger(userInput)) {
        option = stoi(userInput);
        if (option >= min && option <= max) {
            return true;
        }
    }
    return false;

}


void problem1(int n)
{

    string input = "";
    int threadCount = 0;
    //Get the number of threads
    while (true) {
        cout << "Enter the number of threads to use for problem 1: ";
        getline(cin, input);
        if (!validateInput(input, 1, 100))
        {
            cout << "Valid number of threads range from 1 - 100" << endl;
            continue;
        }
        threadCount = stoi(input);
        break;
    }

    //Create the matrix
    vector<int> matrix = createMatrix(n);
    cout << "Original matrix before any changes:" << endl;
    printMatrix(matrix, n);
    
    //Check if the matrix size is 1, if it is there is no work to be done
    if (n == 1)
    {
        cout << "Matrix of size 1 is already completed" << endl;
        return;
    }



    //Create the supervisor thread that checks if the problem has been solved
    pthread_t supervisor;
    bool* threadResult;
    p1Struct supervisorStruct;
    supervisorStruct.matrix = &matrix;
    supervisorStruct.n = n;
    supervisorStruct.mutex = mutex;
    //pthread_create(&supervisor, NULL, (void* (*)(void*))matrixIsDone, (void*)&supervisorStruct);
    pthread_create(&supervisor, NULL, threadChecker, (void*)&supervisorStruct);
    /*cout << "SUP Threads Created!" << endl;
    fflush(stdout);*/

    //Create the threads that flip the values
    pthread_t threads[MAX_ELEMENTS];
    for (int currentThread = 0; currentThread < threadCount; currentThread++)
    {
        p1Struct *currStruct = new p1Struct;
        currStruct->n = n;
        currStruct->matrix = &matrix;
        currStruct->threadNum = currentThread;
        currStruct->mutex = mutex;
        pthread_create(&threads[currentThread], NULL, threadFlipper, (void*)currStruct);
    }
    /*cout << "Flip Threads Created!" << endl;
    fflush(stdout);*/

        
    //Join the threads the flip the values
    for (int currentThread = 0; currentThread < threadCount; currentThread++)
    {
        pthread_join(threads[currentThread], NULL);
    }
    /*cout << "Flip Threads Joined!" << endl;
    fflush(stdout);*/


    //Join the supervisor thread
    pthread_join(supervisor, NULL);
    /*cout << "SUP Threads JOINED!" << endl;
    fflush(stdout);*/

    //Final print of the finished matrix
    if (p1IsDone)
    {
        cout << "Problem 1 is complete! Here is the finished matrix:" << endl;
        printMatrix(matrix, n);
        p1IsDone = false;
        return;
    }
}


void problem2(int n)
{
    string input = "";
    int threadCount = 0;
    //Get the number of threads
    while (true) {
        cout << "Enter the number of threads to use for problem 2: ";
        getline(cin, input);
        if (!validateInput(input, 1, 100))
        {
            cout << "Valid number of threads range from 1 - 100" << endl;
            continue;
        }
        threadCount = stoi(input);
        break;
    }



    vector<int> numArray;
    for (int i = 0; i < n; i++)
    {
        numArray.push_back(rand());
    }
    cout << "Array of numbers before sorting:" << endl;
    printArray(numArray, n);

    if (n == 1)
    {
        cout << "Array already sorted!" << endl;
        printArray(numArray, n);
        return;
    }


    pthread_t supervisor;
    bool* threadResult;
    p2Struct supervisorStruct;
    supervisorStruct.numbers = &numArray;
    supervisorStruct.n = n;
    supervisorStruct.mutex = mutex;
    pthread_create(&supervisor, NULL, arrayChecker, (void*)&supervisorStruct);
    /*out << "SUuuuuuuuup Threads Created!" << endl;
    fflush(stdout);*/

    pthread_t threads[MAX_ELEMENTS];
    for (int currentThread = 0; currentThread < threadCount; currentThread++)
    {
        p2Struct* currStruct = new p2Struct;
        currStruct->n = n;
        currStruct->numbers = &numArray;
        currStruct->threadNum = currentThread;
        currStruct->mutex = mutex;
        pthread_create(&threads[currentThread], NULL, threadSorter, (void*)currStruct);
    }
    /*cout << "Flip Threads Created!" << endl;
    fflush(stdout);*/

    for (int currentThread = 0; currentThread < threadCount; currentThread++)
    {
        pthread_join(threads[currentThread], NULL);
    }

    pthread_join(supervisor, NULL);


    if (p2IsDone)
    {
        cout << "Problem 2 is complete! Here is the sorted array:" << endl;
        printArray(numArray, n);
        p2IsDone = false;
        return;
    }
}







int main(int argc, char* argv[])
{
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    string clArguement = "";
    if (argc > 1)
    {
        clArguement = argv[1];
        if (!validateInput(clArguement, 1, MAX_ELEMENTS))
        {
            cout << "Error: You need to provide an integer greater than zero and less than or equal to 100" << endl;
            return -1;
        }
    }
    else
    {
        cout << "Error: You need to provide an integer greater than zero and less than or equal to 100" << endl;
        return -1;
    }
    int n = stoi(clArguement);
    //int n = 5;

    while (true)
    {
        cout << "Select an option:" << endl;
        cout << "Problem 1: Enter 1" << endl;
        cout << "Problem 2: Enter 2" << endl;
        cout << "Exit: Enter 0" << endl;

        string input = "";
        getline(cin, input);
        if (!validateInput(input, 0, 2))
        {
            cout << "Invalid option try again" << endl;
            continue;
        }
        

        if (input == "0")
        {
            pthread_mutex_destroy(&mutex);
            exit(0);
        }

        if (input == "1")
        {
            problem1(n);
        }

        if (input == "2")
        {
            problem2(n);
        }

    }
}

