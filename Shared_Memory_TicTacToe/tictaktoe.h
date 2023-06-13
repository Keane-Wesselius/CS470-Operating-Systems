#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

const int MAXBOARDSIZE = 20;

//Struct containing the data that is shared between the 2 processes
struct shared
{
    char gameBoard[MAXBOARDSIZE][MAXBOARDSIZE];
    int boardsize;
    int maxPlay;
    int numPlay = 0;
    int turn = 0;
    bool wait = true;
    int semid;
};

//Checks if user input is an integer or not
bool isNumber(string userInput) {
    if (userInput.find_first_not_of("0123456789") != string::npos || userInput == "") {
        return false;
    }
    return true;
}

//Checks if the user input is greater than zero
bool isPositive(int boardSize) {
    if (boardSize > 0) {
        return true;
    }
    return false;
}

//Gets the users turn input and tokenizes it 
vector<string> getTokens(string s) {
    vector<string> v;
    string delimiter = " ";

    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        if (token != "" && token != " ") {
            v.push_back(token);
        }
        s.erase(0, pos + delimiter.length());
    }
    v.push_back(s);
    return v;
}

//gets the next token from the tokenized user input
string getNextToken(vector<string>& v) {
    if (!v.size() > 0) {
        return "error";
    }
    else {
        string nextToken = v.front();
        v.erase(v.begin());
        return nextToken;
    }
}

//Makes sure the boardsize entered by the user is valid
bool validateInput(string userInput)
{
    int boardSize = 0;
        if (isNumber(userInput)) {
            boardSize = stoi(userInput);
            if (boardSize > 2 && boardSize <= MAXBOARDSIZE) {
                return true;
            }
        }
        return false;

}

//void createBoard(shared s, int n)
//{
//    char board[MAXBOARDSIZE][MAXBOARDSIZE];
//    for (int i = 0; i < n; i++)
//    {
//        //*board[i] = new char board[];
//        for (int j = 0; j < n; j++)
//        {
//            s->gameboard[i][j] = ' ';
//        }
//    }
//    return board;
//}

//Prints the board to the console
void printBoard(char board[][MAXBOARDSIZE], int bs)
{
    //Sets up the top part of the board with the X coordinates
    cout << "  ";
    for (int i = 0; i < bs; i++)
    {
        cout << "|" << i;
    }
    cout << "|" << endl;


    for (int i = 0; i < bs; i++)
    {
        //Divides the rows with -----
        for (int j = 0; j < bs; j++)
        {
            cout << "--";
        }
        cout << "---" << endl;
        //Shows the Y coordinates
        cout << i << " ";
        //Displays the Xs and Os representing the players moves
        for (int j = 0; j < bs; j++)
        {
            cout << "|" << board[i][j];
        }
        cout << "|";
        cout << endl;
    }
    //Displays the bottom --- board divider
    for (int j = 0; j < bs; j++)
    {
        cout << "--";
    }
    cout << "---" << endl;
}

//Checks to see if someone won
string checkWinner(char board[][MAXBOARDSIZE], int bs)
{
    int numToWin = bs;
    int oCount = 0;
    int xCount = 0;

    //Checks horizontals for a win
    for (int i = 0; i < bs; i++)
    {
        oCount = 0;
        xCount = 0;
        for (int j = 0; j < bs; j++)
        {
            if (board[i][j] == 'O')
            {
                oCount += 1;
            }
            if (board[i][j] == 'X')
            {
                xCount += 1;
            }
        }

        if (xCount == numToWin)
        {
            return "X";
        }
        if (oCount == numToWin)
        {
            return "O";
        }

    }

    //Checks verticles for a win
    for (int i = 0; i < bs; i++)
    {
        xCount = 0;
        oCount = 0;
        for (int j = 0; j < bs; j++)
        {
            if (board[j][i] == 'O')
            {
                oCount += 1;
            }
            if (board[j][i] == 'X')
            {
                xCount += 1;
            }
        }

        if (xCount == numToWin)
        {
            return "X";
        }
        if (oCount == numToWin)
        {
            return "O";
        }
    }

    //Checks for southeast horizontal win
    xCount = 0;
    oCount = 0;
    int j = 0;
    for (int i = 0; i < bs; i++)
    {
        if (board[i][j] == 'O')
        {
            oCount += 1;
        }
        if (board[i][j] == 'X')
        {
            xCount += 1;
        }
        j++;
    }
    if (xCount == numToWin)
    {
        return "X";
    }
    if (oCount == numToWin)
    {
        return "O";
    }

    //Checks for northeast horizontal win
    xCount = 0;
    oCount = 0;
    j = bs - 1;
    for (int i = 0; i < bs; i++)
    {
        if (board[i][j] == 'O')
        {
            oCount += 1;
        }
        if (board[i][j] == 'X')
        {
            xCount += 1;
        }
        j--;
    }
    if (xCount == numToWin)
    {
        return "X";
    }
    if (oCount == numToWin)
    {
        return "O";
    }

    //No winner was found
    return "Nope";
}

//Unused logic for a computer player
//void computerPlay(char board[][MAXBOARDSIZE], string piece, int bs)
//{
//    int i = rand() % bs;
//    int j = rand() % bs;
//
//    while (board[i][j] != " ")
//    {
//        i = rand() % bs;
//        j = rand() % bs;
//    }
//
//    &board[i][j] = piece;
//}

//Gets the users move to play a piece on their turn
void playerPlay(char board[][MAXBOARDSIZE], char piece, int bs)
{
    string s = "";
    string sI = "";
    string sJ = "";
    while (true) {
        cout << "Enter your move in the form: Row Column" << endl;
        getline(cin, s);

        vector<string> play = getTokens(s);
        if (play.size() < 2)
        {
            cout << "You need to enter 2 numbers that correspond to the coordinates on the board" << endl;
            continue;
        }

        sI = getNextToken(play);
        sJ = getNextToken(play);
        if (!isNumber(sI) || !isNumber(sJ))
        {
            cout << "You need to enter 2 numbers that correspond to the coordinates on the board" << endl;
            continue;
        }

        int i = stoi(sI);
        int j = stoi(sJ);
        if (!((i >= 0 && i < bs) || (j >= 0 && j < bs)))
        {
            cout << "You need to enter 2 numbers that correspond to the coordinates on the board" << endl;
            continue;
        }

        if (board[i][j] != ' ')
        {
            cout << "You need to enter coordinates that has not been played yet" << endl;
            continue;
        }
        board[i][j] = piece;
        return;
    }
}

