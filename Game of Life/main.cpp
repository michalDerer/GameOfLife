
//#include <windows.h>
//#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>

#include "curses.h"
#include "gol.h"

/*
╔══╗
║  ║
╚══╝
*/



/// <summary>
/// input musi byt v hraniciach parametrov
/// </summary>
/// <param name="input"></param>
/// <param name="min">inclusive</param>
/// <param name="max">inclusive</param>
/// <returns></returns>
bool isValidInput(int input, int min, int max)
{
	return min <= input && input <= max;
}

int getInt(int row, int col,int limitMin, int limitMax, const char* question)
{
	char input[10];
	int result = 0;

	do
	{
		try
		{
			clear();
			mvprintw(row, col, question);
			getnstr(input, sizeof(input) - 1);
			result = std::stoi(std::string(input));
		}
		catch (std::exception e)
		{
			result = -1;
		}
	} while (!isValidInput(result, limitMin, limitMax));

	return result;
}


void draw(Data& data)
{
	clear();

	//--- Border

	mvprintw(0, 0, "╔");
	//mvaddch(0, 0, ACS_ULCORNER);

	mvprintw(0, data.cols + 1, "╗");
	//mvaddch(0, data.size + 1, ACS_URCORNER);


	mvprintw(data.rows + 1, 0, "╚");
	//mvaddch(data.size + 1, 0, ACS_LLCORNER);
	mvprintw(data.rows + 1, data.cols + 1, "╝");
	//mvaddch(data.size + 1, data.size + 1, ACS_LRCORNER);

	for (int i = 1; i < data.cols + 1; i++)
	{
		mvprintw(0, i, "═");
		//mvhline(0, i, ACS_HLINE, 1);
		mvprintw(data.rows + 1, i, "═");
		//mvhline(data.size + 1, i, ACS_HLINE, 1);
	}

	for (int i = 1; i < data.rows + 1; i++)
	{
		mvprintw(i, 0, "║");
		//mvaddch(i, 0, ACS_VLINE);
		mvprintw(i, data.cols + 1, "║");
		//mvaddch(i, data.size + 1, ACS_VLINE);
	}

	//--- Inside border

	for (int r = 0; r < data.rows; r++)
	{
		for (int c = 0; c < data.cols; c++)
		{
			//mvprintw(r + 1, c + 1, *(data.rows[r] + c) > 0 ? "X" : " ");
			mvaddch(r + 1, c + 1, ' ' | (*(data.ppRows[r] + c) > 0 ? COLOR_PAIR(1) : 0));
		}
	}

	refresh();
}

void gameLoop(Data& d1, Data& d2, std::atomic<bool>& exit)
{
	int fps = 4;
	float sleepDur = (1.0f / (float)fps) * 1000.0f;


	while (!exit.load())
	{
		auto last = std::chrono::steady_clock::now();

		draw(d1);
		evaluate(d1, d2);
		swap(d1, d2);
		
		auto now = std::chrono::steady_clock::now();
		auto delta = std::chrono::duration<float, std::milli>(now - last).count();
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(300));
		std::this_thread::sleep_for(
			std::chrono::milliseconds(
				(int)std::max(0.0f,sleepDur - delta)
			)
		);
	}
}

int main()
{  
	bool softReset = false;
	std::atomic<bool> exitGameLoop{ false };
	bool exitProgram = false;
	int rows = 0;
	int cols = 0;
	int startLiveCellsCount = 0;
	int ch;

	Data d1;
	Data d2;


	initscr();
	cbreak();   
	start_color();
	curs_set(0);
	//nodelay(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_RED);

	do
	{

		if (softReset)
		{
			softReset = false;
			wype(d1);
			wype(d2);
		}
		else
		{
			release(d1);
			release(d2);

			echo();
			rows = getInt(0, 0, 0, 99, "Zadaj pocet riadkov:");
			cols = getInt(0, 0, 0, 99, "Zadaj pocet stlpcov:");

			std::string prefix("Zadaj pocet zivych buniek (0 az ");
			std::string sufix("):");
			std::string result = prefix + std::to_string(rows * cols) + sufix;
			startLiveCellsCount = getInt(0, 0, 0, rows * cols, result.c_str());

			clear();

			mvprintw(0, 0, "Zadal si pocet riadkov: %i, pocet stlpcov %i a pocet zivych buniek na zaciatku %i", rows, cols, startLiveCellsCount);
			getch();

			create(d1, rows, cols);
			create(d2, rows, cols);
		}

		randomState(d1, startLiveCellsCount);

		noecho();
		exitGameLoop.store(false);
		std::thread drawLoopThread(gameLoop, std::ref(d1), std::ref(d2), std::ref(exitGameLoop));

		while (true)
		{
			ch = getch();

			if (ch == 'q') 
			{    
				exitProgram = true;
				break;
			}
			else if (ch == 'r')
			{
				softReset = true;
				break;
			}
			else if (ch == 't')
			{
				break;
			}
		}

		exitGameLoop.store(true);
		drawLoopThread.join();

	} while (!exitProgram);

	clear();
	endwin();
}











//int main()
//{
//	//SetConsoleOutputCP(CP_UTF8);
//	srand(time(nullptr)); 
//
//	int size = 28;
//	Data d1;
//	Data d2;
//
//	create(d1, size, size * 2);
//	create(d2, size, size * 2);
//	randomState(d1, 256);
//	//randomState(d2, 2);
//
//	initscr();
//	start_color();          // enable colors
//	noecho();				// don't echo typed characters
//	curs_set(0);			// Hide cursor
//	cbreak();				// disable line buffering
//	//keypad(stdscr, TRUE); // enable function/arrow keys
//	nodelay(stdscr, TRUE);	// **non-blocking input**
//	
//	// Define a color pair (foreground, background)
//	init_pair(1, COLOR_WHITE, COLOR_WHITE);   // white text on blue background
//
//	while (true) 
//	{
//		repaint(d1);
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		evaluate(d1, d2);
//		swap(d1, d2);
//
//		int ch = getch();   // read a key
//		
//		if (ch != ERR && (ch == 'y' || ch == 'z'))
//		{
//			break;
//		}
//	}
//	
//	nodelay(stdscr, FALSE);								// restore blocking mode for final key
//	mvprintw(d1.rows + 2, 0, "Press key to exit");
//	getch();											// Wait for a key press
//
//	endwin();											// Cleanup
//
//	release(d1);
//	release(d2);
//}

