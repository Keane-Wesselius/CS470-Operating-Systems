#pragma once
#include <pthread.h>
#include <vector>

struct p2Struct
{
	vector<int>* numbers;
	int n;
	int threadNum;
	pthread_mutex_t mutex;
};

bool checkSorted(vector<int> numbers, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		if (numbers[i] > numbers[i + 1])
		{
			return false;
		}
	}
	return true;
}

void arraySort(vector<int>& numbers, int n, int index)
{
	int pivot = numbers[index];
	int i = 0, j = n - 1;
	while (i <= j)
	{
		while (i <= j && numbers[i] < pivot) i++;
		while (i <= j && numbers[j] >= pivot) j--;
		if (i <= j)
			swap(numbers[i++], numbers[j--]);
	}
}

void printArray(vector<int> numbers, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		cout << numbers[i] << ", ";
	}

	cout << numbers[n - 1] << endl;
}