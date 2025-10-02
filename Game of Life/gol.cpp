
#include <cstdlib>

#include "gol.h"


void create(Data& data, int rows, int cols)
{
	data.rows = rows;
	data.cols = cols;
	data.pData = new int [rows * cols] {};
	data.ppRows = new int* [rows] {};

	for (int r = 0; r < rows; r++)
	{
		data.ppRows[r] = data.pData + r * cols;
	}
}

void wype(Data& data)
{
	for (int i = 0; i < data.rows * data.cols; i++)
	{
		data.pData[i] = 0;
	}
}

void release(Data& data)
{
	data.rows = 0;
	data.cols = 0;

	if (data.ppRows != nullptr)
	{
		delete[] data.ppRows;
		data.ppRows = nullptr;
	}
	if (data.pData != nullptr)
	{
		delete[] data.pData;
		data.pData = nullptr;
	}
}

void randomState(Data& data, int liveCells)
{
	int selectedIndex;
	int lastIndex = data.rows * data.cols - 1;
	int* indexes = new int[data.rows * data.cols];
	for (int i = 0; i < data.rows * data.cols; i++)
	{
		indexes[i] = i;
		data.pData[i] = 0;
	}

	while (liveCells > 0)
	{
		int ii = rand() % (lastIndex + 1);
		selectedIndex = indexes[ii];
		indexes[ii] = indexes[lastIndex];
		lastIndex--;

		data.pData[selectedIndex] = 1;

		liveCells--;
	}

	delete[] indexes;
}

void swap(Data& from, Data& to)
{
	Data hlp = to;
	to = from;
	from = hlp;
}

void applyRules(int* from, int* to, int sum)
{
	*to = 0;

	if (*from == 1)
	{
		if (1 < sum && sum < 4)
		{
			*to = 1;
		}
	}
	else
	{
		if (sum == 3)
		{
			*to = 1;
		}
	}
}

void evaluate(Data& from, Data& to)
{
	//wype(to);
	int sum = 0;

	//--- stred, (nie kraje, rohy)

	for (int r = 1; r + 1 < from.rows; r++)
	{
		for (int c = 1; c + 1 < from.cols; c++)
		{
			sum =
				*(from.ppRows[r - 1] + c - 1) +
				*(from.ppRows[r - 1] + c    ) +
				*(from.ppRows[r - 1] + c + 1) +

				*(from.ppRows[r] + c - 1) +
				*(from.ppRows[r] + c + 1) +

				*(from.ppRows[r + 1] + c - 1) +
				*(from.ppRows[r + 1] + c    ) +
				*(from.ppRows[r + 1] + c + 1);

			applyRules(from.ppRows[r] + c, to.ppRows[r] + c, sum);
		}
	}

	//--- horny okraj, (nie rohy)

	for (int i = 1; i + 1 < from.cols; i++)
	{
		sum =
			*(from.ppRows[0] + i - 1) +
			*(from.ppRows[0] + i + 1) +

			*(from.ppRows[1] + i - 1) +
			*(from.ppRows[1] + i    ) +
			*(from.ppRows[1] + i + 1);
			
		applyRules(from.ppRows[0] + i, to.ppRows[0] + i, sum);
	}

	//--- dolny okraj, (nie rohy)

	for (int i = 1; i + 1 < from.cols; i++)
	{
		sum =
			*(from.ppRows[from.rows - 1] + i - 1) +
			*(from.ppRows[from.rows - 1] + i + 1) +

			*(from.ppRows[from.rows - 2] + i - 1) +
			*(from.ppRows[from.rows - 2] + i    ) +
			*(from.ppRows[from.rows - 2] + i + 1);

		applyRules(from.ppRows[from.rows - 1] + i, to.ppRows[to.rows - 1] + i, sum);
	}

	//--- lavy okrak, (nie rohy)

	for (int i = 1; i + 1 < from.rows; i++)
	{
		sum =
			*(from.ppRows[i - 1]) +
			*(from.ppRows[i + 1]) +

			*(from.ppRows[i - 1] + 1) +
			*(from.ppRows[i    ] + 1) +
			*(from.ppRows[i + 1] + 1);

		applyRules(from.ppRows[i], to.ppRows[i], sum);
	}

	//--- pravy okraj, (nie rohy)

	for (int i = 1; i + 1 < from.rows; i++)
	{
		sum =
			*(from.ppRows[i - 1] + from.cols - 1) +
			*(from.ppRows[i + 1] + from.cols - 1) +

			*(from.ppRows[i - 1] + from.cols - 2) +
			*(from.ppRows[i    ] + from.cols - 2) +
			*(from.ppRows[i + 1] + from.cols - 2);

		applyRules(from.ppRows[i] + from.cols - 1, to.ppRows[i] + to.cols - 1, sum);
	}

	//--- horny lavy roh

	sum =
		*(from.ppRows[0] + 1) +

		*(from.ppRows[1]    ) +
		*(from.ppRows[1] + 1);

	applyRules(from.ppRows[0], to.ppRows[0], sum);

	//--- horny pravy roh

	sum =
		*(from.ppRows[0] + from.cols - 2) +

		*(from.ppRows[1] + from.cols - 2) +
		*(from.ppRows[1] + from.cols - 1);

	applyRules(from.ppRows[0] + from.cols - 1, to.ppRows[0] + to.cols - 1, sum);

	//--- dolny lavy roh

	sum =
		*(from.ppRows[from.rows - 2]    ) +
		*(from.ppRows[from.rows - 2] + 1) +

		*(from.ppRows[from.rows - 1] + 1);

	applyRules(from.ppRows[from.rows - 1], to.ppRows[to.rows - 1], sum);

	//--- dolny pravy roh

	sum =
		*(from.ppRows[from.rows - 2] + from.cols - 2) +
		*(from.ppRows[from.rows - 2] + from.cols - 1) +

		*(from.ppRows[from.rows - 1] + from.cols - 2);

	applyRules(from.ppRows[from.rows - 1] + from.cols - 1, to.ppRows[to.rows - 1] + to.cols - 1, sum);
}