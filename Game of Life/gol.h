#pragma once

struct Data
{
	int rows = 0;
	int cols = 0;
	int* pData = nullptr;
	int** ppRows = nullptr;
};

void create(Data& data, int rows, int cols);
void release(Data& data);
void randomState(Data& data, int liveCells);
void wype(Data& data);
void swap(Data& from, Data& to);
void evaluate(Data& from, Data& to);
