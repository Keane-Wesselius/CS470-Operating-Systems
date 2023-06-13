#pragma once
#include <vector>
#include <pthread.h>
using namespace std;

struct p1Struct
{
    vector<int> *matrix;
    int n;
    int threadNum;
    pthread_mutex_t mutex;
};

vector<int> createMatrix(int n)
{
    vector<int> v;
    for (int i = 0; i < n * n; i++)
    {
        v.push_back(rand() % 2);
    }

    return v;

}

void printMatrix(vector<int> v, int n)
{
    cout << "Matrix:" << endl;
    for (unsigned int i = 0; i < v.size(); i++)
    {
        if (i % n == 0 && i != 0)
        {
            cout << endl;
        }
        cout << v[i] << " ";
    }
    cout << endl;
}


int convert2dTo1d(int i, int j, int n)
{
    return i * n + j;
}




int matrixNeighbors(vector<int>& v, int i, int j, int n)
{
    int valueCheck = 0;
    //Top left corner case //3 checks
    if (i == 0 && j == 0)
    {
        //value right
        valueCheck += v[convert2dTo1d(i, j + 1, n)];
        //value bottom right
        valueCheck += v[convert2dTo1d(i + 1, j + 1, n)];
        //value bottom
        valueCheck += v[convert2dTo1d(i + 1, j, n)];


        if (valueCheck >= 2)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }

    }



    //Top right corner case
    else if (i == 0 && j == n - 1)
    {
        //value bottom
        valueCheck += v[convert2dTo1d(i + 1, j, n)];
        //value bottom left
        valueCheck += v[convert2dTo1d(i + 1, j - 1, n)];
        //value left
        valueCheck += v[convert2dTo1d(i, j - 1, n)];

        if (valueCheck >= 2)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }
    }



    //Bottom left corner case
    else if (i == n - 1 && j == 0)
    {
        //value top
        valueCheck += v[convert2dTo1d(i - 1, j, n)];
        //value top right
        valueCheck += v[convert2dTo1d(i - 1, j + 1, n)];
        //value right
        valueCheck += v[convert2dTo1d(i, j + 1, n)];


        if (valueCheck >= 2)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }
    }



    //Bottom right corner case
    else if (i == n - 1 && j == n - 1)
    {
        //value top left
        valueCheck += v[convert2dTo1d(i - 1, j - 1, n)];
        //value top
        valueCheck += v[convert2dTo1d(i - 1, j, n)];
        //value left
        valueCheck += v[convert2dTo1d(i, j - 1, n)];

        if (valueCheck >= 2)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }
    }



    //top mid case
    else if (i == 0)
    {
        //value right
        valueCheck += v[convert2dTo1d(i, j + 1, n)];
        //value bottom right
        valueCheck += v[convert2dTo1d(i + 1, j + 1, n)];
        //value bottom
        valueCheck += v[convert2dTo1d(i + 1, j, n)];
        //value bottom left
        valueCheck += v[convert2dTo1d(i + 1, j - 1, n)];
        //value left
        valueCheck += v[convert2dTo1d(i, j - 1, n)];

        if (valueCheck >= 3)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }
    }



    //bottom mid case
    else if (i == n - 1)
    {
        //value top left
        valueCheck += v[convert2dTo1d(i - 1, j - 1, n)];
        //value top
        valueCheck += v[convert2dTo1d(i - 1, j, n)];
        //value top right
        valueCheck += v[convert2dTo1d(i - 1, j + 1, n)];
        //value right
        valueCheck += v[convert2dTo1d(i, j + 1, n)];
        //value left
        valueCheck += v[convert2dTo1d(i, j - 1, n)];

        if (valueCheck >= 3)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }
    }



    //left Middle case
    else if (j == 0)
    {
        //value top
        valueCheck += v[convert2dTo1d(i - 1, j, n)];
        //value top right
        valueCheck += v[convert2dTo1d(i - 1, j + 1, n)];
        //value right
        valueCheck += v[convert2dTo1d(i, j + 1, n)];
        //value bottom right
        valueCheck += v[convert2dTo1d(i + 1, j + 1, n)];
        //value bottom
        valueCheck += v[convert2dTo1d(i + 1, j, n)];

        if (valueCheck >= 3)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }

    }



    //right mid case
    else if (j == n - 1)
    {
        //value top left
        valueCheck += v[convert2dTo1d(i - 1, j - 1, n)];
        //value top
        valueCheck += v[convert2dTo1d(i - 1, j, n)];
        //value bottom
        valueCheck += v[convert2dTo1d(i + 1, j, n)];
        //value bottom left
        valueCheck += v[convert2dTo1d(i + 1, j - 1, n)];
        //value left
        valueCheck += v[convert2dTo1d(i, j - 1, n)];

        if (valueCheck >= 3)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }
    }



    //Middle case
    else
    {
        //value top left
        valueCheck += v[convert2dTo1d(i - 1, j - 1, n)];
        //value top
        valueCheck += v[convert2dTo1d(i - 1, j, n)];
        //value top right
        valueCheck += v[convert2dTo1d(i - 1, j + 1, n)];
        //value right
        valueCheck += v[convert2dTo1d(i, j + 1, n)];
        //value bottom right
        valueCheck += v[convert2dTo1d(i + 1, j + 1, n)];
        //value bottom
        valueCheck += v[convert2dTo1d(i + 1, j, n)];
        //value bottom left
        valueCheck += v[convert2dTo1d(i + 1, j - 1, n)];
        //value left
        valueCheck += v[convert2dTo1d(i, j - 1, n)];

        if (valueCheck == 4)
        {
            int flipped = rand() % 2;
            v[convert2dTo1d(i, j, n)] = flipped;
            return flipped;
        }
        else if (valueCheck >= 5)
        {
            v[convert2dTo1d(i, j, n)] = 1;
            return 1;
        }
        else
        {
            v[convert2dTo1d(i, j, n)] = 0;
            return 0;
        }
    }
}//End of matrixNeighbors()



bool matrixIsDone(vector<int> v, int n)
{
    int check = v[0];
    for (int i = 1; i < v.size(); i++)
    {
        if (v[i] != check)
        {
            return false;
        }
    }
    return true;
}



