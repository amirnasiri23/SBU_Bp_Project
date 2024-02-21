#include <iostream>  // input and output
#include <cstdlib>  // generate random number
#include <ctime>
#include <conio.h>
#include <iomanip>  // output form
#include <string>  // string changes
#include <fstream>  // work with files
#include <vector>  // main field of game
#include <chrono>  // handel time of game loop
#include <thread>

using namespace std;  // standard namespace we use
//------------------------------------------------------------------------------------------------------
const string WHITE = "\033[0m";  // colors
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string PURPLE = "\033[35m";
const string LITE_BLUE = "\033[36m";

struct user {   // data of each user
	string name;
	int score;
	int level;
	int width;
	int height;
	double time;
};

int numOfPleayers[2] = {0, 0};  // how many playesr we have in each level

int block[2][4];  // each block of game
vector <vector <int> > field;  // main field
vector <user> scorsData1;  // save data from file for show leaderboard
vector <user> scorsData2;  // save data from file for show leaderboard

int FPS = 60;  // time of game loop
chrono::duration<double, ratio<1, 1>> frameTime = chrono::duration<double, ratio<1, 1>>(1.0 / FPS);

void newGame();  // prototypes
void pauseMenu();
void guideLineShow();
void creatBlock();
void showLeaderBoard();
void writeOnFile1(user);
void writeOnFile2(user);
void readFromFile1();
void readFromFile2();
bool isWithinBounds(int, int, user);
void exit();
//------------------------------------------------------------------------------------------------------
int main() {
	while (true) {
			system("cls");
			char userCoice;

		// menu
		cout << WHITE << "-----------------------------------------------------------------------------------------------" << endl;
		cout << LITE_BLUE << "===================Welcome Tetirs Game===================" << endl;
		cout << endl;
    	cout << BLUE << "-----------------Please Enter Your Choice-----------------" << endl;
		cout << endl;
    	cout << YELLOW << "1) New Game {Choose This To Start A New Game}" << endl;
    	cout << GREEN << "2) How To Play {Choose This To Learn The Game}" << endl;
    	cout << RED << "3) Leader Board {Choose This To See The Score Board}" << endl;
    	cout << PURPLE << "4) Exit {Choose This To Exit The Program}" << endl;

    	cout << WHITE << endl;

    	cout << WHITE << "Enter Your Order : ";
			cin >> userCoice;

		switch(userCoice) {
		case '1':
			creatBlock();
			newGame();
			break;
		case '2':
			guideLineShow();
			break;
		case '3':
			if (numOfPleayers[0] != 0 || numOfPleayers[1] != 0)
				showLeaderBoard();
			else cerr << "WE Dont have Any Players!" << endl;
			break;
		case '4':
			exit();
			break;
		default:
			cerr << RED << "Please Enter a Valid Order !" << endl;
			cout << endl;
			break;

		}
		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------

void newGame() {
	system("cls");
	// get user data
	user user;
	cout << LITE_BLUE << "Enter Your Name : ";
	cin >> user.name;

	cout << RED << "Choice Level Of Game (1 or 2) : ";
	cin >> user.level;

	cout << GREEN << "Enter Width Of Game Field (Min = 5) : ";
	cin >> user.width;

	cout << YELLOW << "Enter Height Of Game Field (Min = 5) : ";
	cin >> user.height;

	user.score = 0;
	user.time = 0;

	if (user.level == 1) { // speed is the diference of levels
		FPS = 2;
		numOfPleayers[0] += 1;
	}
	else {
		FPS = 5;
		numOfPleayers[1] += 1;
	}
	// initial frameTime for each level
	chrono::duration<double, ratio<1, 1>> frameTime = chrono::duration<double, ratio<1, 1>>(1.0 / FPS);

	system("cls");

	bool isLoose = false;

	// build field of game (walls are -10 and centre blocks are 0)
	for (int i = 0; i < user.height; i++) {
		vector <int> temp;
		temp.push_back(-10);

		for (int j = 0; j < user.width; j++)
			temp.push_back(0);

		temp.push_back(-10);
		field.push_back(temp);
	}
	vector <int> lastLine;
	for (int k = 0; k < user.width + 2; k++)
		lastLine.push_back(-10);
	field.push_back(lastLine);

	// start for geting users play time
	chrono::time_point<chrono::high_resolution_clock> gameStart = chrono::high_resolution_clock::now();

	creatBlock();

	while (!isLoose) { // game loope

	// start for geting loop time
	chrono::time_point<chrono::high_resolution_clock> frameStart = chrono::high_resolution_clock::now();  // time handeling

		// put block in the field
		for (int i = 0; i < 2; i++) {
			for (int j = (user.width) / 2; j < 4 + (user.width / 2); j++) {
				if (block[i][j - (user.width / 2)] != 0)
					field[i][j] = block[i][j - (user.width / 2)];
			}
		}

		// show game field for user
		for (int i = 0; i < user.height + 1; i++) {
			for (int j = 0; j < user.width + 2; j++) {
				if (field[i][j] == 0)
					cout << WHITE << " ";
				else if (field[i][j] == 1 || field[i][j] == -1)
					cout << RED << "\u2588";
				else if (field[i][j] == 2 || field[i][j] == -2)
					cout << BLUE << "\u2588";
				else if (field[i][j] == 3 || field[i][j] == -3)
					cout << YELLOW << "\u2588";
				else if (field[i][j] == 4 || field[i][j] == -4)
					cout << PURPLE << "\u2588";
				else if (field[i][j] == 5 || field[i][j] == -5)
					cout << GREEN << "\u2588";
				else if (field[i][j] == 6 || field[i][j] == -6)
					cout << LITE_BLUE << "\u2588";
				else if (field[i][j] == 7 || field[i][j] == -7)
					cout << WHITE << "\u2588";
				else if (field[i][j] == -10)
					cout << WHITE << "\u273F";
			}
			cout << endl;
		}

		// fall block to end
		for (int j = 1; j < user.width + 1; j++) {
							for (int i = user.height - 1; i >= 0; i--) {
								// 1 blocks form 1
								if (isWithinBounds(i + 1, j + 3, user)) {
								if (field[i][j] == 1 && field[i][j + 1] == 1 && field[i][j + 2] == 1 && field[i][j + 3] == 1) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 1][j + 2] == 0 && field[i + 1][j + 3] == 0) {
										field[i + 1][j] = 1;
										field[i + 1][j + 1] = 1;
										field[i + 1][j + 2] = 1;
										field[i + 1][j + 3] = 1;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i][j + 2] = 0;
										field[i][j + 3] = 0;
									}
								}}
								// 1 blocks form 2
								if (isWithinBounds(i + 3, j, user)) {
								if (field[i][j] == 1 && field[i - 1][j] == 1 && field[i + 1][j] == 1 && field[i + 2][j] == 1) {
									if (field[i + 3][j] == 0) {
										field[i + 3][j] = 1;
										field[i - 1][j] = 0;
									}
								}}
								// 2 blocks form 1
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 1][j + 1] == 2 && field[i + 1][j + 2] == 2) {
									if (field[i + 2][j] == 0 && field[i + 2][j + 1] == 0 && field[i + 2][j + 2] == 0) {
										field[i + 2][j] = 2;
										field[i + 2][j + 1] = 2;
										field[i + 2][j + 2] = 2;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
										field[i + 1][j + 2] = 0;
									}
								}}
								// 2 block form 2
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2) {
									if (field[i + 3][j] == 0 && field[i + 1][j + 1] == 0) {
										field[i + 3][j] = 2;
										field[i + 1][j + 1] = 2;
										field[i][j] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 2 blocks form 3
								if (isWithinBounds(i + 3, j - 1, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2 && field[i + 2][j - 1] == 2) {
									if (field[i + 3][j - 1] == 0 && field[i + 3][j] == 0) {
										field[i + 3][j - 1] = 2;
										field[i + 3][j] = 2;
										field[i + 2][j - 1] = 0;
										field[i][j] = 0;
									}
								}}
								// 2 blocks form 4
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i][j + 2] == 2 && field[i + 1][j + 2] == 2) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 2][j + 2] == 0) {
										field[i + 1][j] = 2;
										field[i + 1][j + 1] = 2;
										field[i + 2][j + 2] = 2;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i][j + 2] = 0;
									}
								}}
								// 3 blocks form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i - 1][j + 2] == 3) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i + 1][j] = 3;
										field[i + 1][j + 1] = 3;
										field[i + 1][j + 2] = 3;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i - 1][j + 2] = 0;
									}
								}}
								// 3 block form 2
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i + 1][j + 1] == 3 && field[i + 2][j + 1] == 3) {
									if (field[i + 1][j] == 0 && field[i + 3][j + 1] == 0) {
										field[i + 1][j] = 3;
										field[i + 3][j + 1] = 3;
										field[i][j] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 3 blocks form 3
								if (isWithinBounds(i + 3, j, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i + 1][j] == 3) {
									if (field[i + 2][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i + 2][j] = 3;
										field[i + 1][j + 1] = 3;
										field[i + 1][j + 2] = 3;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i][j + 2] = 0;
									}
								}}
								// 3 blocks form 4
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 3 && field[i + 1][j] == 3 && field[i + 2][j] == 3 && field[i + 2][j + 1] == 3) {
									if (field[i + 3][j] == 0 && field[i + 3][j + 1] == 0) {
										field[i + 3][j] = 3;
										field[i + 3][j + 1] = 3;
										field[i][j] = 0;
										field[i + 2][j + 1] = 0;
									}
								}}
								// 4 blocks
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 4 && field[i][j + 1] == 4 && field[i + 1][j] == 4 && field[i + 1][j + 1] == 4) {
									if (field[i + 2][j] == 0 && field[i + 2][j + 1] == 0) {
										field[i + 2][j] = 4;
										field[i + 2][j + 1] = 4;
										field[i][j] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 5 blocks form 1
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 5 && field[i][j + 1] == 5 && field[i - 1][j + 1] == 5 && field[i - 1][j + 2] == 5) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i][j + 2] == 0) {
										field[i + 1][j] = 5;
										field[i + 1][j + 1] = 5;
										field[i][j + 2] = 5;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
										field[i - 1][j + 2] = 0;
									}
								}}
								// 5 blocks form 2
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 5 && field[i + 1][j] == 5 && field[i + 1][j + 1] == 5 && field[i + 2][j + 1] == 5) {
									if (field[i + 2][j] == 0 && field[i + 3][j + 1] == 0) {
										field[i + 2][j] = 5;
										field[i + 3][j + 1] = 5;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
									}
								}}
								// 6 blocks form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 2] == 6) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i + 1][j] = 6;
										field[i + 1][j + 1] = 6;
										field[i + 1][j + 2] = 6;
										field[i][j] = 0;
										field[i][j + 2] = 0;
										field[i - 1][j + 1] = 0;
									}
								}}
								// 6 block form 2
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 1] == 6 && field[i + 1][j + 1] == 6) {
									if (field[i + 1][j] == 0 && field[i + 3][j + 1] == 0) {
										field[i + 1][j] = 6;
										field[i + 3][j + 1] = 6;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
									}
								}}
								// 6 block form 3
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i][j + 2] == 6 && field[i + 1][j + 1] == 6) {
									if (field[i + 1][j] == 0 && field[i + 2][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i + 1][j] = 6;
										field[i + 2][j + 1] = 6;
										field[i + 1][j + 2] = 6;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i][j + 2] = 0;
									}
								}}
								// 6 block form 4
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 6 && field[i - 1][j] == 6 && field[i + 1][j] == 6 && field[i][j + 1] == 6) {
									if (field[i + 2][j] == 0 && field[i + 1][j + 1] == 0) {
										field[i + 2][j] = 6;
										field[i + 1][j + 1] = 6;
										field[i - 1][j] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 7 block form 1
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i + 1][j + 1] == 7 && field[i + 1][j + 2] == 7) {
									if (field[i + 1][j] == 0 && field[i + 2][j + 1] == 0 && field[i + 2][j + 2] == 0) {
										field[i + 1][j] = 7;
										field[i + 2][j + 1] = 7;
										field[i + 2][j + 2] = 7;
										field[i][j] = 0;
										field[i + 1][j + 2] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 7 block form 2
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i - 1][j + 1] == 7 && field[i + 1][j] == 7) {
									if (field[i + 2][j] == 0 && field[i + 1][j + 1] == 0) {
										field[i + 2][j] = 7;
										field[i + 1][j + 1] = 7;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
									}
								}}
							}
						}

		// check for user clicking keyboard
		if (kbhit()) {
			char clickedKey = getch();  // input move of block
			switch (clickedKey) {
				case 'a':  //move block to Left
				case 'A':
						for (int j = 1; j < user.width + 1; j++) {
							for (int i = user.height - 1; i >= 0; i--) {
								// 1 blocks form 1
								if (isWithinBounds(i, j + 3, user)) {
								if (field[i][j] == 1 && field[i][j + 1] == 1 && field[i][j + 2] == 1 && field[i][j + 3] == 1) {
									if (field[i][j - 1] == 0) { 
											field[i][j - 1] = 1;
											field[i][j + 3] = 0;
									}
									}
								}
								// 1 blocks form 2
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 1 && field[i - 1][j] == 1 && field[i + 1][j] == 1 && field[i + 2][j] == 1) {
									if (field[i][j - 1] == 0 && field[i - 1][j - 1] == 0 && field[i + 1][j - 1] == 0 && field[i + 2][j - 1] == 0) {
										field[i][j] = 0;
										field[i - 1][j] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j] = 0;
										field[i][j - 1] = 1;
										field[i - 1][j - 1] = 1;
										field[i + 1][j - 1] = 1;
										field[i + 2][j - 1] = 1;
									}
								}}
								// 2 blocks form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 1][j + 1] == 2 && field[i + 1][j + 2] == 2) {
									if (field[i][j - 1] == 0 && field[i + 1][j - 1] == 0) {
										field[i][j - 1] = 2;
										field[i + 1][j - 1] = 2;
										field[i][j] = 0;
										field[i + 1][j + 2] = 0;
									}
								}}
								// 2 block form 2
								if (isWithinBounds(i, j + 3, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2) {
									if (field[i][j - 1] == 0 && field[i + 1][j - 1] == 0 && field[i + 2][j - 1] == 0) {
										field[i][j - 1] = 2;
										field[i + 1][j - 1] = 2;
										field[i + 2][j - 1] = 2;
										field[i][j + 1] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j] = 0;
									}
								}}
								// 2 blocks form 3
								if (isWithinBounds(i, j - 2, user)) {
								 if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2 && field[i + 2][j - 1] == 2) {
									if (field[i][j - 1] == 0 && field[i + 1][j - 1] == 0 && field[i + 2][j - 2] == 0) {
										field[i][j - 1] = 2;
										field[i + 1][j - 1] = 2;
										field[i + 2][j - 2] = 2;
										field[i][j] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j] = 0;
									}
								}}
								// 2 blocks form 4
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i][j + 2] == 2 && field[i + 1][j + 2] == 2) {
									if (field[i][j - 1] == 0 && field[i + 1][j + 1] == 0) {
										field[i][j - 1] = 2;
										field[i + 1][j + 1] = 2;
										field[i][j + 2] = 0;
										field[i + 1][j + 2] = 0;
									}
								}}
								// 3 blocks form 1
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i - 1][j + 2] == 3) {
									if (field[i - 1][j - 1] == 0 && field[i][j - 1] == 0) {
										field[i - 1][j - 1] = 3;
										field[i][j - 3] = 3;
										field[i - 1][j] = 0;
										field[i][j + 2] = 0;
									}
								}}
								// 3 block form 2
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i + 1][j + 1] == 3 && field[i + 2][j + 1] == 3) {
									if (field[i][j - 1] == 0 && field[i + 1][j] == 0 && field[i + 2][j] == 0) {
										field[i][j - 1] = 3;
										field[i + 1][j] = 3;
										field[i + 2][j] = 3;
										field[i][j + 1] = 0;
										field[i + 1][j + 1] = 0;
										field[i + 2][j + 1] = 0;
									}
								}}
								// 3 blocks form 3
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i + 1][j] == 3) {
									if (field[i][j - 1] == 0 && field[i + 1][j - 1] == 0) {
										field[i][j - 1] = 3;
										field[i + 1][j - 1] = 3;
										field[i][j + 2] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 3 blocks form 4
								if (isWithinBounds(i + 2, j - 1, user)) {
								if (field[i][j] == 3 && field[i + 1][j] == 3 && field[i + 2][j] == 3 && field[i + 2][j + 1] == 3) {
									if (field[i][j - 1] == 0 && field[i + 1][j - 1] == 0 && field[i + 2][j - 1] == 0) {
										field[i][j - 1] = 3;
										field[i + 1][j - 1] = 3;
										field[i + 2][j - 1] = 3;
										field[i][j] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j + 1] = 0;
									}
								}}
								// 4 blocks
								if (isWithinBounds(i + 1, j - 1, user)) {
								if (field[i][j] == 4 && field[i][j + 1] == 4 && field[i + 1][j] == 4 && field[i + 1][j + 1] == 4) {
									if (field[i][j - 1] == 0 && field[i + 1][j - 1] == 0) {
										field[i][j - 1] = 4;
										field[i + 1][j - 1] = 4;
										field[i][j + 2] = 0;
										field[i + 1][j + 2] = 0;
									}
								}}
								// 5 blocks form 1
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 5 && field[i][j + 1] == 5 && field[i - 1][j + 1] == 5 && field[i - 1][j + 2] == 5) {
									if (field[i - 1][j] == 0 && field[i][j - 1] == 0) {
										field[i - 1][j] = 5;
										field[i][j - 1] = 5;
										field[i][j + 1] = 0;
										field[i - 1][j + 2] = 0;
									}
								}}
								// 5 blocks form 2
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 5 && field[i + 1][j] == 5 && field[i + 1][j + 1] == 5 && field[i + 2][j + 1] == 5) {
									if (field[i][j - 1] == 0 && field[i + 1][j - 1] == 0 && field[i + 2][j] == 0) {
										field[i][j - 1] = 5;
										field[i + 1][j - 1] = 5;
										field[i + 2][j] = 5;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
										field[i + 2][j + 1] = 0;
									}
								}}
								// 6 blocks form 1
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 2] == 6) {
									if (field[i - 1][j] == 0 && field[i][j - 1] == 0) {
										field[i - 1][j] = 6;
										field[i][j - 1] = 6;
										field[i - 1][j + 1] = 0;
										field[i][j + 2] = 0;
									}
								}}
								// 6 block form 2
								if (isWithinBounds(i - 1, j + 1, user)) {
								if (field[i][j] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 1] == 6 && field[i + 1][j + 1] == 6) {
									if (field[i][j - 1] == 0 && field[i + 1][j] == 0 && field[i - 1][j] == 0) {
										field[i][j - 1] = 6;
										field[i + 1][j] = 6;
										field[i - 1][j] = 6;
										field[i][j + 1] = 0;
										field[i + 1][j + 1] = 0;
										field[i + 2][j + 1] = 0;
									}
								}}
								// 6 block form 3
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i][j + 2] == 6 && field[i + 1][j + 1] == 6) {
									if (field[i][j - 1] == 0 && field[i + 1][j] == 0) {
										field[i][j - 1] = 6;
										field[i + 1][j] = 6;
										field[i][j + 2] = 0;
										field[i + 1][j + 1] = 0;
									}
								}}
								// 6 block form 4
								if (isWithinBounds(i - 1, j - 1, user)) {
								if (field[i][j] == 6 && field[i - 1][j] == 6 && field[i + 1][j] == 6 && field[i][j + 1] == 6) {
									if (field[i - 1][j - 1] == 0 && field[i][j - 1] == 0 && field[i + 1][j - 1] == 0) {
										field[i - 1][j - 1] = 6;
										field[i][j - 1] = 6;
										field[i + 1][j - 1] = 6;
										field[i - 1][j] = 0;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
									}
								}}
								// 7 block form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i + 1][j + 1] == 7 && field[i + 1][j + 2] == 7) {
									if (field[i][j - 1] == 0 && field[i + 1][j] == 0) {
										field[i][j - 1] = 7;
										field[i + 1][j] = 7;
										field[i][j + 1] = 0;
										field[i + 1][j + 2] = 0;
									}
								}}
								// 7 block form 2
								if (isWithinBounds(i - 1, j - 1, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i - 1][j + 1] == 7 && field[i + 1][j] == 7) {
									if (field[i - 1][j] == 0 && field[i][j - 1] == 0 && field[i + 1][j - 1] == 0) {
										field[i - 1][j] = 7;
										field[i][j - 1] = 7;
										field[i + 1][j - 1] = 7;
										field[i - 1][j + 1] = 0;
										field[i][j + 1] = 0;
										field[i + 1][j] = 0;
									}
								}}
							}
						}
						break;
				case 'd':  // move blocks to Right
				case 'D':
						for (int j = user.width; j >= 1; j--) {
							for (int i = user.height - 1; i >= 0; i--) {
								// 1 blocks form 1
								if (isWithinBounds(i, j + 3, user)) {
								if (field[i][j] == 1 && field[i][j + 1] == 1 && field[i][j + 2] == 1 && field[i][j + 3] == 1) {
									if (field[i][j + 4] == 0) {
										field[i][j + 4] = 1;
										field[i][j] = 0;
									}
								}}
								// 1 blocks form 2
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 1 && field[i - 1][j] == 1 && field[i + 1][j] == 1 && field[i + 2][j] == 1) {
									if (field[i][j + 1] == 0 && field[i - 1][j + 1] == 0 && field[i + 1][j + 1] == 0 && field[i + 2][j + 1] == 0) {
										field[i][j] = 0;
										field[i - 1][j] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j] = 0;
										field[i][j + 1] = 1;
										field[i - 1][j + 1] = 1;
										field[i + 1][j + 1] = 1;
										field[i + 2][j + 1] = 1;
									}
								}}
								// 2 blocks form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 1][j + 1] == 2 && field[i + 1][j + 2] == 2) {
									if (field[i][j + 1] == 0 && field[i + 1][j + 3] == 0) {
										field[i][j + 1] = 2;
										field[i + 1][j + 3] = 2;
										field[i][j] = 0;
										field[i + 1][j] = 0;
									}
								}}
								// 2 block form 2
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2) {
									if (field[i][j + 2] == 0 && field[i + 1][j + 1] == 0 && field[i + 2][j + 1] == 0) {
										field[i][j + 2] = 2;
										field[i + 1][j + 1] = 2;
										field[i + 2][j + 1] = 2;
										field[i][j] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j] = 0;
									}
								}}
								// 2 blocks form 3
								if (isWithinBounds(i + 2, j - 1, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2 && field[i + 2][j - 1] == 2) {
									if (field[i][j + 1] == 0 && field[i + 1][j + 1] == 0 && field[i + 2][j + 1] == 0) {
										field[i][j + 1] = 2;
										field[i + 1][j + 1] = 2;
										field[i + 2][j + 1] = 2;
										field[i][j] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j - 1] = 0;
									}
								}}
								// 2 blocks form 4
								if (isWithinBounds(i, j + 3, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i][j + 2] == 2 && field[i + 1][j + 2] == 2) {
									if (field[i][j + 3] == 0 && field[i + 1][j + 3] == 0) {
										field[i][j + 3] = 2;
										field[i + 1][j + 3] = 2;
										field[i][j] = 0;
										field[i + 1][j + 2] = 0;
									}
								}}
								// 3 blocks form 1
								if (isWithinBounds(i - 1, j + 3, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i - 1][j + 2] == 3) {
									if (field[i - 1][j + 1] == 0 && field[i][j + 3] == 0) {
										field[i - 1][j + 1] = 3;
										field[i][j + 3] = 3;
										field[i - 1][j] = 0;
										field[i][j] = 0;
									}
								}}
								// 3 block form 2
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i + 1][j + 1] == 3 && field[i + 2][j + 1] == 3) {
									if (field[i][j + 2] == 0 && field[i + 1][j + 2] == 0 && field[i + 2][j + 2] == 0) {
										field[i + 1][j + 2] = 3;
										field[i][j + 2] = 3;
										field[i + 2][j + 2] = 3;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
										field[i + 2][j + 1] = 0;
									}
								}}
								// 3 blocks form 3
								if (isWithinBounds(i, j + 3, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i + 1][j] == 3) {
									if (field[i][j + 3] == 0 && field[i + 1][j + 1] == 0) {
										field[i][j + 3] = 3;
										field[i + 1][j + 1] = 3;
										field[i][j] = 0;
										field[i + 1][j] = 0;
									}
								}}
								// 3 blocks form 4
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 3 && field[i + 1][j] == 3 && field[i + 2][j] == 3 && field[i + 2][j + 1] == 3) {
									if (field[i][j + 1] == 0 && field[i + 1][j + 1] == 0 && field[i + 2][j + 2] == 0) {
										field[i][j + 1] = 3;
										field[i + 1][j + 1] = 3;
										field[i + 2][j + 2] = 3;
										field[i][j] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j] = 0;
									}
								}}
								// 4 blocks
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 4 && field[i][j + 1] == 4 && field[i + 1][j] == 4 && field[i + 1][j + 1] == 4) {
									if (field[i][j + 2] == 0 && field[i + 1][j + 2] == 0) {
										field[i][j + 2] = 4;
										field[i + 1][j + 2] = 4;
										field[i][j] = 0;
										field[i + 1][j] = 0;
									}
								}}
								// 5 blocks form 1
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 5 && field[i][j + 1] == 5 && field[i - 1][j + 1] == 5 && field[i - 1][j + 2] == 5) {
									if (field[i - 1][j + 3] == 0 && field[i][j + 2] == 0) {
										field[i - 1][j + 3] = 5;
										field[i][j + 2] = 5;
										field[i - 1][j + 1] = 0;
										field[i][j] = 0;
									}
								}}
								// 5 blocks form 2
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 5 && field[i + 1][j] == 5 && field[i + 1][j + 1] == 5 && field[i + 2][j + 1] == 5) {
									if (field[i][j + 1] == 0 && field[i + 1][j + 2] == 0 && field[i + 2][j + 1] == 0) {
										field[i][j + 1] = 5;
										field[i + 1][j + 2] = 5;
										field[i + 2][j + 1] = 5;
										field[i][j] = 0;
										field[i + 1][j] = 0;
										field[i + 2][j + 1] = 0;
									}
								}}
								// 6 blocks form 1
								if (isWithinBounds(i - 1, j + 3, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 2] == 6) {
									if (field[i - 1][j + 2] == 0 && field[i][j + 3] == 0) {
										field[i - 1][j + 2] = 6;
										field[i][j + 3] = 6;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
									}
								}}
								// 6 block form 2
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 1] == 6 && field[i + 1][j + 1] == 6) {
									if (field[i][j + 2] == 0 && field[i + 1][j + 2] == 0 && field[i + 2][j + 2] == 0) {
										field[i][j + 2] = 6;
										field[i + 1][j + 2] = 6;
										field[i + 2][j + 2] = 6;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
										field[i + 1][j + 1] = 0;
									}
								}}
								// 6 block form 3
								if (isWithinBounds(i, j + 3, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i][j + 2] == 6 && field[i + 1][j + 1] == 6) {
									if (field[i][j + 3] == 0 && field[i + 1][j + 2] == 0) {
										field[i][j + 3] = 6;
										field[i + 1][j + 2] = 6;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
									}
								}}
								// 6 block form 4
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 6 && field[i - 1][j] == 6 && field[i + 1][j] == 6 && field[i][j + 1] == 6) {
									if (field[i - 1][j + 1] == 0 && field[i][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i - 1][j + 1] = 6;
										field[i][j + 1] = 6;
										field[i + 1][j + 2] = 6;
										field[i - 1][j] = 0;
										field[i][j] = 0;
										field[i + 1][j] = 0;
									}
								}}
								// 7 block form 1
								if (isWithinBounds(i + 1, j + 3, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i + 1][j + 1] == 7 && field[i + 1][j + 2] == 7) {
									if (field[i][j + 2] == 0 && field[i + 1][j + 3] == 0) {
										field[i][j + 2] = 7;
										field[i + 1][j + 3] = 7;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
									}
								}}
								// 7 block form 2
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i - 1][j + 1] == 7 && field[i + 1][j] == 7) {
									if (field[i - 1][j + 2] == 0 && field[i][j + 2] == 0 && field[i + 1][j + 1] == 0) {
										field[i - 1][j + 2] = 7;
										field[i][j + 2] = 7;
										field[i + 1][j + 1] = 7;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
										field[i + 1][j] = 0;
									}
								}}
							}
						}
						break;
				case 's':  // speed (move blocks to down)
				case 'S':
						for (int j = 1; j < user.width + 1; j++) {
							for (int i = user.height - 1; i >= 0; i--) {
								// 1 blocks form 1
								if (isWithinBounds(i + 1, j + 3, user)) {
								if (field[i][j] == 1 && field[i][j + 1] == 1 && field[i][j + 2] == 1 && field[i][j + 3] == 1) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 1][j + 2] == 0 && field[i + 1][j + 3] == 0) {
										field[i + 1][j] = 1;
										field[i + 1][j + 1] = 1;
										field[i + 1][j + 2] = 1;
										field[i + 1][j + 3] = 1;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i][j + 2] = 0;
										field[i][j + 3] = 0;
									}
								}}
								// 1 blocks form 2
								if (isWithinBounds(i + 3, j, user)) {
								if (field[i][j] == 1 && field[i - 1][j] == 1 && field[i + 1][j] == 1 && field[i + 2][j] == 1) {
									if (field[i + 3][j] == 0) {
										field[i + 3][j] = 1;
										field[i - 1][j] = 0;
									}
								}}
								// 2 blocks form 1
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 1][j + 1] == 2 && field[i + 1][j + 2] == 2) {
									if (field[i + 2][j] == 0 && field[i + 2][j + 1] == 0 && field[i + 2][j + 2] == 0) {
										field[i + 2][j] = 2;
										field[i + 2][j + 1] = 2;
										field[i + 2][j + 2] = 2;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
										field[i + 1][j + 2] = 0;
									}
								}}
								// 2 block form 2
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2) {
									if (field[i + 3][j] == 0 && field[i + 1][j + 1] == 0) {
										field[i + 3][j] = 2;
										field[i + 1][j + 1] = 2;
										field[i][j] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 2 blocks form 3
								if (isWithinBounds(i + 3, j - 1, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2 && field[i + 2][j - 1] == 2) {
									if (field[i + 3][j - 1] == 0 && field[i + 3][j] == 0) {
										field[i + 3][j - 1] = 2;
										field[i + 3][j] = 2;
										field[i + 2][j - 1] = 0;
										field[i][j] = 0;
									}
								}}
								// 2 blocks form 4
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i][j + 2] == 2 && field[i + 1][j + 2] == 2) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 2][j + 2] == 0) {
										field[i + 1][j] = 2;
										field[i + 1][j + 1] = 2;
										field[i + 2][j + 2] = 2;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i][j + 2] = 0;
									}
								}}
								// 3 blocks form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i - 1][j + 2] == 3) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i + 1][j] = 3;
										field[i + 1][j + 1] = 3;
										field[i + 1][j + 2] = 3;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i - 1][j + 2] = 0;
									}
								}}
								// 3 block form 2
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i + 1][j + 1] == 3 && field[i + 2][j + 1] == 3) {
									if (field[i + 1][j] == 0 && field[i + 3][j + 1] == 0) {
										field[i + 1][j] = 3;
										field[i + 3][j + 1] = 3;
										field[i][j] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 3 blocks form 3
								if (isWithinBounds(i + 3, j, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i + 1][j] == 3) {
									if (field[i + 2][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i + 2][j] = 3;
										field[i + 1][j + 1] = 3;
										field[i + 1][j + 2] = 3;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i][j + 2] = 0;
									}
								}}
								// 3 blocks form 4
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 3 && field[i + 1][j] == 3 && field[i + 2][j] == 3 && field[i + 2][j + 1] == 3) {
									if (field[i + 3][j] == 0 && field[i + 3][j + 1] == 0) {
										field[i + 3][j] = 3;
										field[i + 3][j + 1] = 3;
										field[i][j] = 0;
										field[i + 2][j + 1] = 0;
									}
								}}
								// 4 blocks
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 4 && field[i][j + 1] == 4 && field[i + 1][j] == 4 && field[i + 1][j + 1] == 4) {
									if (field[i + 2][j] == 0 && field[i + 2][j + 1] == 0) {
										field[i + 2][j] = 4;
										field[i + 2][j + 1] = 4;
										field[i][j] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 5 blocks form 1
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 5 && field[i][j + 1] == 5 && field[i - 1][j + 1] == 5 && field[i - 1][j + 2] == 5) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i][j + 2] == 0) {
										field[i + 1][j] = 5;
										field[i + 1][j + 1] = 5;
										field[i][j + 2] = 5;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
										field[i - 1][j + 2] = 0;
									}
								}}
								// 5 blocks form 2
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 5 && field[i + 1][j] == 5 && field[i + 1][j + 1] == 5 && field[i + 2][j + 1] == 5) {
									if (field[i + 2][j] == 0 && field[i + 3][j + 1] == 0) {
										field[i + 2][j] = 5;
										field[i + 3][j + 1] = 5;
										field[i][j] = 0;
										field[i + 1][j + 1] = 0;
									}
								}}
								// 6 blocks form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 2] == 6) {
									if (field[i + 1][j] == 0 && field[i + 1][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i + 1][j] = 6;
										field[i + 1][j + 1] = 6;
										field[i + 1][j + 2] = 6;
										field[i][j] = 0;
										field[i][j + 2] = 0;
										field[i - 1][j + 1] = 0;
									}
								}}
								// 6 block form 2
								if (isWithinBounds(i + 3, j + 1, user)) {
								if (field[i][j] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 1] == 6 && field[i + 1][j + 1] == 6) {
									if (field[i + 1][j] == 0 && field[i + 3][j + 1] == 0) {
										field[i + 1][j] = 6;
										field[i + 3][j + 1] = 6;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
									}
								}}
								// 6 block form 3
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i][j + 2] == 6 && field[i + 1][j + 1] == 6) {
									if (field[i + 1][j] == 0 && field[i + 2][j + 1] == 0 && field[i + 1][j + 2] == 0) {
										field[i + 1][j] = 6;
										field[i + 2][j + 1] = 6;
										field[i + 1][j + 2] = 6;
										field[i][j] = 0;
										field[i][j + 1] = 0;
										field[i][j + 2] = 0;
									}
								}}
								// 6 block form 4
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 6 && field[i - 1][j] == 6 && field[i + 1][j] == 6 && field[i][j + 1] == 6) {
									if (field[i + 2][j] == 0 && field[i + 1][j + 1] == 0) {
										field[i + 2][j] = 6;
										field[i + 1][j + 1] = 6;
										field[i - 1][j] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 7 block form 1
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i + 1][j + 1] == 7 && field[i + 1][j + 2] == 7) {
									if (field[i + 1][j] == 0 && field[i + 2][j + 1] == 0 && field[i + 2][j + 2] == 0) {
										field[i + 1][j] = 7;
										field[i + 2][j + 1] = 7;
										field[i + 2][j + 2] = 7;
										field[i][j] = 0;
										field[i + 1][j + 2] = 0;
										field[i][j + 1] = 0;
									}
								}}
								// 7 block form 2
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i - 1][j + 1] == 7 && field[i + 1][j] == 7) {
									if (field[i + 2][j] == 0 && field[i + 1][j + 1] == 0) {
										field[i + 2][j] = 7;
										field[i + 1][j + 1] = 7;
										field[i][j] = 0;
										field[i - 1][j + 1] = 0;
									}
								}}
							}
						}
						break;
			  	case 'p':
				case 'P':
						pauseMenu();
						break;
				case 'r':  //rotate block
				case 'R':
						for (int i = 2; i < user.height - 2; i++) {
							for (int j = 0; j < user.width + 1; j++) {
								// 1 blocks form 1
								if (isWithinBounds(i + 2, j + 3, user)) {
								if (field[i][j] == 1 && field[i][j + 1] == 1 && field[i][j + 2] == 1 && field[i][j + 3] == 1) {
									field[i - 1][j + 2] = 1;
									field[i + 1][j + 2] = 1;
									field[i + 2][j + 2] = 1;
									field[i][j] = 0;
									field[i][j + 1] = 0;
									field[i][j + 3] = 0;
								}}
								// 1 blocks form 2
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 1 && field[i - 1][j] == 1 && field[i + 1][j] == 1 && field[i + 2][j] == 1) {
									field[i][j - 1] = 1;
									field[i][j + 1] = 1;
									field[i][j + 2] = 1;
									field[i - 1][j] = 0;
									field[i + 1][j] = 0;
									field[i + 2][j] = 0;
								}}
								// 2 blocks form 1
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 1][j + 1] == 2 && field[i + 1][j + 2] == 2) {
									field[i][j + 1] = 2;
									field[i + 2][j] = 2;
									field[i + 2][j + 1] = 2;
									field[i][j] = 0;
									field[i + 1][j] = 0;
									field[i + 1][j + 2] = 0;

								}}
								// 2 block form 2
								if (isWithinBounds(i + 2, j + 2, user)) {
							 	if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2) {
									field[i + 1][j - 1] = 2;
									field[i + 1][j + 1] = 2;
									field[i][j - 1] = 2;
									field[i][j] = 0;
									field[i][j + 1] = 0;
									field[i][j + 2] = 0;
								}}
								// 2 blocks form 3
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 2 && field[i + 1][j] == 2 && field[i + 2][j] == 2 && field[i + 2][j - 1] == 2) {
									field[i + 1][j - 1] = 2;
									field[i + 1][j + 1] = 2;
									field[i + 2][j + 1] = 2;
									field[i][j] = 0;
									field[i + 2][j] = 0;
									field[i + 2][j - 1] = 0;
								}}
								// 2 blocks form 4
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 2 && field[i][j + 1] == 2 && field[i][j + 2] == 2 && field[i + 1][j + 2] == 2) {
									field[i - 1][j + 1] = 2;
									field[i - 1][j + 2] = 2;
									field[i + 1][j + 1] = 2;
									field[i][j] = 0;
									field[i + 1][j + 2] = 0;
									field[i][j + 2] = 0;
								}}
								// 3 blocks form 1
								if (isWithinBounds(i - 1, j + 2, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i - 1][j + 2] == 3) {
									field[i - 1][j] = 3;
									field[i - 1][j + 1] = 3;
									field[i + 1][j + 1] = 3;
									field[i][j] = 0;
									field[i - 1][j + 2] = 0;
									field[i][j + 2] = 0;
								}}
								// 3 block form 2
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i + 1][j + 1] == 3 && field[i + 2][j + 1] == 3) {
									field[i + 1][j] = 3;
									field[i + 1][j + 2] = 3;
									field[i + 2][j] = 3;
									field[i][j] = 0;
									field[i + 2][j + 1] = 0;
									field[i][j + 1] = 0;
								}}
								// 3 blocks form 3
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 3 && field[i][j + 1] == 3 && field[i][j + 2] == 3 && field[i + 1][j] == 3) {
									field[i - 1][j + 1] = 3;
									field[i + 1][j + 1] = 3;
									field[i + 1][j + 2] = 3;
									field[i][j] = 0;
									field[i + 1][j] = 0;
									field[i][j + 2] = 0;
								}}
								// 3 block form 4
								if (isWithinBounds(i + 2, j + 1, user)) {
								if (field[i][j] == 3 && field[i + 1][j] == 3 && field[i + 2][j] == 3 && field[i + 2][j + 1] == 3) {
									field[i][j + 1] = 3;
									field[i + 1][j + 1] = 3;
									field[i + 1][j - 1] = 3;
									field[i][j] = 0;
									field[i + 2][j] = 0;
									field[i + 2][j + 1] = 0;
								}}
								// for block number 4 ---> noting happend in rotate !
								// 5 blocks form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 5 && field[i][j + 1] == 5 && field[i - 1][j + 1] == 5 && field[i - 1][j + 2] == 5) {
									field[i - 1][j] = 5;
									field[i + 1][j + 1] = 5;
									field[i - 1][j + 2] = 0;
									field[i - 1][j + 1] = 0;
								}}
								// 5 blocks form 2
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 5 && field[i + 1][j] == 5 && field[i + 1][j + 1] == 5 && field[i + 2][j + 1] == 5) {
									field[i + 1][j + 2] = 5;
									field[i + 2][j] = 5;
									field[i + 2][j + 1] = 5;
									field[i][j] = 0;
									field[i + 1][j] = 0;
								}}
								// 6 blocks form 1
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 2] == 6) {
									field[i + 1][j + 1] = 6;
									field[i][j + 2] = 0;
								}}
								// 6 block form 2
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 6 && field[i - 1][j + 1] == 6 && field[i][j + 1] == 6 && field[i + 1][j + 1] == 6) {
									field[i][j + 2] = 6;
									field[i - 1][j + 1] = 0;
								}}
								// 6 block form 3
								if (isWithinBounds(i + 1, j + 2, user)) {
								if (field[i][j] == 6 && field[i][j + 1] == 6 && field[i][j + 2] == 6 && field[i + 1][j + 1] == 6) {
									field[i - 1][j + 1] = 6;
									field[i][j] = 0;
								}}
								// 6 block form 4
								if (isWithinBounds(i - 1, j - 1, user)) {
								if (field[i][j] == 6 && field[i - 1][j] == 6 && field[i + 1][j] == 6 && field[i][j + 1] == 6) {
									field[i][j - 1] = 6;
									field[i + 1][j] = 0;
								}}
								// 7 block form 1
								if (isWithinBounds(i + 2, j + 2, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i + 1][j + 1] == 7 && field[i + 1][j + 2] == 7) {
									field[i + 1][j] = 7;
									field[i + 2][j] = 7;
									field[i][j] = 0;
									field[i + 1][j + 2] = 0;
								}}
								// 7 block form 2
								if (isWithinBounds(i + 1, j + 1, user)) {
								if (field[i][j] == 7 && field[i][j + 1] == 7 && field[i - 1][j + 1] == 7 && field[i + 1][j] == 7) {
									field[i][j - 1] = 7;
									field[i + 1][j + 1] = 7;
									field[i][j + 1] = 0;
									field[i - 1][j + 1] = 0;
								}}
							}
						}
						break;
			}
		}

		bool isArrived = false;
		for (int j = 1; j < user.width + 2; j++) {   // when a block arrive to buttom of field
			for (int i = 0; i < user.height + 1; i++) {
				if (field[i - 1][j] < 0) {
					field[i][j] *= -10;
					isArrived = true;
				}
			}
		}

		if (isArrived) {
			system("cls");
			creatBlock();
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			// show next block under the field
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[i][j] == 0)
						cout << " ";
					else if (block[i][j] == 1)
						cout << RED << "\u2588";
					else if (block[i][j] == 2)
						cout << BLUE << "\u2588";
					else if (block[i][j] == 3)
						cout << YELLOW << "\u2588";
					else if (block[i][j] == 4)
						cout << PURPLE << "\u2588";
					else if (block[i][j] == 5)
						cout << GREEN << "\u2588";
					else if (block[i][j] == 6)
						cout << LITE_BLUE << "\u2588";
					else if (block[i][j] == 7)
						cout << WHITE << "\u2588";
				}
				cout << endl;
			}
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
		}

		// do we have a line full of block
		int howManyBL = 0;
		int fullRow = 0;
		bool weHaveFull = false;
		for (int i = 0; i < user.height + 1; i++) {
			howManyBL = 0;
			for (int j = 1; j < user.width + 2; j++) {
				if (field[i][j] != 0)
				howManyBL++;
			}
			if (howManyBL == user.width) {
				fullRow = i;
				weHaveFull = true;
				break;
			}
		}

		// remove full line from board
		if (weHaveFull) {
			for (int j = 1; j < user.width + 2; j++)
			field[fullRow][j] = 0;

			for (int j = 1; j < user.width + 2; j++) {
				for (int i = fullRow; i > 0; i--)
					field[i][j] = field[i - 1][j];
			}
			user.score += user.width;
		}

		// check that we have a calumn full of blocks (means Loose!)
		for (int i = 0; i < user.height + 1; i++) {
			int howManyBlock = 0;
			for (int j = 1; j < user.width + 1; j++) {
				if (field[i][j] != 0)
					howManyBlock++;
			}
			if (howManyBlock == user.height)
				isLoose = true;
			break;
		}

		chrono::time_point<chrono::high_resolution_clock> frameEnd = chrono::high_resolution_clock::now();
		chrono::duration<double, ratio<1, 1>> frameDuration = frameEnd - frameStart;

		// stop game if loop take less time from frameTime
		if (frameDuration < frameTime)
        	this_thread::sleep_for(frameTime - frameDuration);
	}
	chrono::time_point<chrono::high_resolution_clock> gameFinish = chrono::high_resolution_clock::now();

	chrono::duration<double, ratio<1, 1>> userTime = gameFinish - gameStart;

	// user.time = std::chrono::duration<double, std::ratio<1> > userTime;

	// write data on file
	if (user.level == 1) writeOnFile1(user);
	else writeOnFile2(user);

}
//------------------------------------------------------------------------------------------------------
void pauseMenu() {

	char pauseChoice;
	
	cout << RED << "////////////PAUSE MENU////////////" << endl;
	cout << GREEN << "1) Unpause" << endl;
	cout << BLUE << "2) Exit" << endl;
	cout << endl;
	
	cout << WHITE << "Enter Your Choice : ";
	cin >> pauseChoice;

	system("pause");
	
	switch (pauseChoice) {
		case '1':
					break;
		case '2':
					exit();
					break;
	  default:
					pauseMenu();
					break;
	}
}
//------------------------------------------------------------------------------------------------------
void guideLineShow() {
	system("cls");

	// guideLine menu
	cout << WHITE << "When a line be full of blocks you get point for each block \n";
	cout << LITE_BLUE << "Game Has Two Levels (1 or 2)\n" << endl;

	cout << WHITE << "Movment : " << endl;
	cout << RED << "a) Move Block To Left" << endl;
	cout << BLUE << "d) Move Block To Right" << endl;
	cout << endl;
	cout << WHITE << "Control Speed : " << endl;
	cout << YELLOW << "s) Increase Speed Of Blocek" << endl;
	cout << endl;
	cout << WHITE << "Pause : " << endl;
	cout << GREEN << "p) Open Pause Menu" << endl;
	cout << endl;
	cout << PURPLE << "R) Rotate Block" << endl;
	cout << endl;

	char guideLineChose;
	cout << WHITE << "---------------------------------------------------------\n" << endl;
	cout << BLUE << "1) New Game" << endl;
	cout << GREEN << "2) Exit" << endl;
	cout << WHITE << "What Next : ";
	cin >> guideLineChose;

	switch (guideLineChose) {
		case '1':
						newGame();
						break;
		case '2':
						exit();
						break;
		default:
						guideLineShow();
						break;
	}
}
//------------------------------------------------------------------------------------------------------
void creatBlock() {  // change global array that names block

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = 0;
		}
	}

	int whatShape = rand() % 7;

	switch (whatShape) {
		case 0:
					for (int j = 0; j < 4; j++)
						block[1][j] = 1;
					break;
		case 1:
					block[0][0] = 2;
					for (int j = 0; j < 3; j++)
						block[1][j] = 2;
					break;
		case 2:
					block[0][2] = 3;
					for (int j = 0; j < 3; j++)
						block[1][j] = 3;
					break;
		case 3:
					block[0][0] = 4;
					block[1][0] = 4;
					block[0][1] = 4;
					block[1][1] = 4;
					break;
		case 4:
					block[0][1] = 5;
					block[0][2] = 5;
					block[1][0] = 5;
					block[1][1] = 5;
					break;
		case 5:
					block[0][1] = 6;
					for (int j = 0; j < 3; j++)
						block[1][j] = 6;
					break;
		case 6:
					block[0][0] = 7;
					block[0][1] = 7;
					block[1][1] = 7;
					block[1][2] = 7;
					break;
	}
}

// to check moves are avaliable or not
bool isWithinBounds(int x, int y, user user) {
	return x >= 0 && x < user.height && y > 0 && y < (user.width + 2);
}
//------------------------------------------------------------------------------------------------------
// write on level 1 datafile
void writeOnFile1(user user) {
	ofstream writer ("ScoreBoard1.txt", ios::app);
	if (writer.is_open()) {
		writer << user.name << " " << user.score << " " << user.time << endl;
		writer.close();
	}
	else 
		cerr << "Error : Unable to open the ScoreBoard1 File!" << endl;
}
// write on level 2 datafile
void writeOnFile2(user user) {
	ofstream writer ("ScoreBoard2.txt", ios::app);
	if (writer.is_open()) {
		writer << user.name << " " << user.score << " " << user.time << endl;
		writer.close();
	}
	else 
		cerr << "Error : Unable to open the ScoreBoard2 File!" << endl;
}
// read from level 1 datalist
void readFromFile1() {
	ifstream reader("ScoreBoard1.txt");
	if (reader.is_open()) {
		user temp;

		while (reader >> temp.name >> temp.score >> temp.time)
			scorsData1.push_back(temp);
	}
}
// read from level 2 datalist
void readFromFile2() {
	ifstream reader("ScoreBoard1.txt");
	if (reader.is_open()) {
		user temp;

		while (reader >> temp.name >> temp.score >> temp.time)
			scorsData2.push_back(temp);
	}
}
// LeaderBoard
void showLeaderBoard() {
	readFromFile1();
	readFromFile2();

	bool swapped;
	user temp;

	bool swapped2;
	user temp2;

	// sort with bobel sort alghoritm
	for (int k = 0; k < numOfPleayers[0] - 1; k++) {
		swapped = false;
		for (int x = 0; x < numOfPleayers[0] - k - 1; x++) {
			if (scorsData1[x].score < scorsData1[x + 1].score) {

				temp = scorsData1[x];
				scorsData1[x] = scorsData1[x + 1];
				scorsData1[x + 1] = temp;

				swapped = true;
			}
		}
		if (!swapped)
			break;
	}
	for (int k = 0; k < numOfPleayers[1] - 1; k++) {
		swapped2 = false;
		for (int x = 0; x < numOfPleayers[1] - k - 1; x++) {
			if (scorsData1[x].score < scorsData1[x + 1].score) {

				temp2 = scorsData1[x];
				scorsData1[x] = scorsData1[x + 1];
				scorsData1[x + 1] = temp2;

				swapped2 = true;
			}
		}
		if (!swapped2)
			break;
	}

	system("cls");
	cout << BLUE << setw(10) << "LEVEL1 : " << "          " << PURPLE << setw(10) << "NAME" << "          " << GREEN << setw(10) << "SCORE" << "          " << RED << setw(10) << "TIME" << endl;
	for (int c = 0; c < numOfPleayers[0]; c++) {
		if (scorsData1[c].score != 0)
			cout << setw(10) << " " << "          " << PURPLE << setw(10) << scorsData1[c].name << "          " << GREEN << setw(10) << scorsData1[c].score << "          " << RED << setw(10) << scorsData1[c].time << endl;
	}

	cout << BLUE << setw(10) << "LEVEL2 : " << "          " << PURPLE << setw(10) << "NAME" << "          " << GREEN << setw(10) << "SCORE" << "          " << RED << setw(10) << "TIME" << endl;
	for (int c = 0; c < numOfPleayers[1]; c++) {
		if (scorsData2[c].score != 0)
			cout << setw(10) << " " << "          " << PURPLE << setw(10) << scorsData2[c].name << "          " << GREEN << setw(10) << scorsData2[c].score << "          " << RED << setw(10) << scorsData2[c].time << endl;
	}

}
void exit() {
	exit(0);
}
