#include <iostream>    //librarys
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

const string RESET = "\033[0m";    //defined colors
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string PURPLE = "\033[35m";
const string LITE_BLUE = "\033[36m";

struct user {      //for each user
    string userName;
    int userPoint;
    bool isWinner;
};

int board[4][4]; //main board of the game
user gamer;   //defined a user for calculate the score
user player;   //defined a user for sort and show scores
user scoreArr[5]; //main leader board
int numberOFPlayer = 0;

void AddRandomNumber();   //prototypes
void newGame();
int randplace1();
int newNumber();
void move();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();
void writeOnFile();
void readFromeFile();
void showLeaderBoard();
void exit();


int newNumber() {  //generate a random new number ((2 2 2) or 4)
    int newNumber;
    int randomNum = rand() % 12 + 1;
    if (randomNum % 4 == 0)
        newNumber = 4;
    else
        newNumber = 2;
    return newNumber;
}

int randplace1() {  //generate a random number for place of new number
    int randomNum1 = rand() % 4;
    return randomNum1;
}
void newGame() {  //the main function for each player game
    int numOfNumbers = 0;

    cout << "Enter Your Name (with Out Space Please Use _ Insted) : ";
    cin >> gamer.userName;

    gamer.userPoint = 0;
    system("cls");

    bool isWin = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            board[i][j] = 0;
    }

    AddRandomNumber();
    while (!isWin) {
        numOfNumbers = 0;
        for (int i = 0; i < 4; i++) {  //count of nums in board except 0
            for (int j = 0; j < 4; j++)
                if (board[i][j] != 0)
                    numOfNumbers++;
        }

        if (numOfNumbers <= 15)
            AddRandomNumber();

        bool flag = false;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == board[i + 1][j] || board[i][j] == board[i][j + 1])
                    flag = true;
            }
        }
        if (numOfNumbers == 16) {  //if flag still false and there are 16 numbers it means loose
            if (!flag) {
                system("cls");
                cout << "You Loooose !!!!!\n" << endl;
                gamer.isWinner = false;
                cout << endl;
                writeOnFile();
                readFromeFile();
                break;
            }
        }
// -----------------------------------------------------------------------------------------------------------------------
        // show the board of game
        cout << "\u250D";
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                cout << RESET << "\u2501";
            }
            cout << "\u2500";
        }
        for (int k = 0; k < 4; k++) {
                cout << RESET << "\u2501";
        }
        cout << "\u2511";
        cout << endl;

        for (int i = 0; i < 4; i++) {
                cout << RESET << "\u2502";
                for (int j = 0; j < 4; j++) {
                    if (board[i][j] == 0) {
                        cout << setw(4) << " ";
                        cout << RESET << "\u2502";
                    }
                    else if (board[i][j] == 2 || board[i][j] == 128) {
                        cout << RED << setw(4) << board[i][j];
                        cout << RESET << "\u2502";
                    }
                    else if (board[i][j] == 4 || board[i][j] == 256) {
                        cout << YELLOW << setw(4) << board[i][j];
                        cout << RESET << "\u2502";
                    }
                    else if (board[i][j] == 8 || board[i][j] == 512) {
                        cout << GREEN << setw(4) << board[i][j];
                        cout << RESET << "\u2502";
                    }
                    else if (board[i][j] == 16 || board[i][j] == 1024) {
                        cout << LITE_BLUE << setw(4) << board[i][j];
                        cout << RESET << "\u2502";
                    }
                    else if (board[i][j] == 32 || board[i][j] == 2048) {
                        cout << PURPLE << setw(4) << board[i][j];
                        cout << RESET << "\u2502";
                    }
                    else if (board[i][j] == 64) {
                        cout << BLUE << setw(4) << board[i][j];
                        cout << RESET << "\u2502";
                    }
                }
                cout << endl;
        }

        cout << "\u2515";
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                cout << RESET << "\u2501";
            }
            cout << "\u2500";
        }
        for (int k = 0; k < 4; k++) {
                cout << RESET << "\u2501";
        }
        cout << "\u2519";
        cout << endl;
// -----------------------------------------------------------------------------------------------------------------------
        move();
        system("cls");
        for (int i = 0; i < 4; i++) {  //is there a 2048 tile (means win)
            for (int j = 0; j < 4; j++)
                if (board[i][j] == 2048)
                    isWin = true;
        }
    }
    if (isWin) {
        system("cls");
        cout << "You Wiiiiiiin !!!!!!!\n" << endl;
        gamer.isWinner = true;
        cout << endl;
        writeOnFile();
        readFromeFile();
        cout << endl;
    }

}
void AddRandomNumber() {  //to put new number on random place
    int num = newNumber();
    int place1 = randplace1();
    int place2 = randplace1();

    if (board[place1][place2] == 0)  //tile of 0 is an empty tile
        board[place1][place2] = num;
    else
        AddRandomNumber();
}

void move() { //to get users order for move
    char userMove = getch();
    switch (userMove) {
        case 'w':
        case 'W':
        case 72:
            moveUp();
            break;
        case 'a':
        case 'A':
        case 75:
            moveLeft();
            break;
        case 's':
        case 'S':
        case 80:
            moveDown();
            break;
        case 'd':
        case 'D':
        case 77:
            moveRight();
            break;
    }
}

void moveUp() { //to move tiles Up
    int temp1;
    for (int i = 1; i < 4; i++) {
            for( int j = 0; j < 4; j++) {

                if (board[i][j] == 0)
                    continue;

                if (board[i - 1][j] == 0 || board[i - 1][j] == board[i][j]) {
                    if (board[i - 1][j] != 0) {
                        gamer.userPoint += board[i - 1][j];
                        gamer.userPoint += board[i][j];
                    }
                    board[i - 1][j] += board[i][j];
                    board[i][j] = 0;
                    i = 1;
                    j = 0;
                }
            }
    }
    if (board[0][0] == 0) {
        for (int f = 1; f < 4; f++){
            temp1 = board[f][0];
            board[f][0] = board[f - 1][0];
            board[f - 1][0] = temp1;
        }
    }
}

void moveDown() {  //to move tiles Down
        for (int i = 0; i < 4 - 1; i++) {
            for (int j = 0; j < 4; j++) {

                if (board[i][j] == 0)
                    continue;

                if (board[i + 1][j] == 0 || board[i + 1][j] == board[i][j]) {
                    if (i < 2) {
                        if (board[i + 2][j] != board[i][j]) {
                            if (board[i - 1][j] != 0) {
                                gamer.userPoint += board[i + 1][j];
                                gamer.userPoint += board[i][j];
                            }
                            board[i + 1][j] += board[i][j];
                            board[i][j] = 0;
                                i = 0;
                            j = 0;
                        }
                        else {
                            if (board[i - 1][j] != 0) {
                                gamer.userPoint += board[i + 1][j];
                                gamer.userPoint += board[i][j];
                            }
                            board[i + 2][j] += board[i][j];
                            board[i][j] = 0;
                            i = 0;
                            j = 0;
                        }
                    }
                    if (i == 2) {
                        if (board[i - 1][j] != 0) {
                            gamer.userPoint += board[i + 1][j];
                            gamer.userPoint += board[i][j];
                        }
                        board[i + 1][j] += board[i][j];
                        board[i][j] = 0;
                        i = 0;
                        j = 0;
                    }
                }
            }
        }
}

void moveRight() {  //to move tiles Right
    for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4 - 1; j++) {

                if (board[i][j] == 0 )
                    continue;

                if (board[i][j + 1] == 0 || board[i][j + 1] == board[i][j]) {
                    if (j < 2) {
                        if (board[i][j + 2] != board[i][j]) {
                            if (board[i][j - 1] != 0) {
                            gamer.userPoint += board[i][j + 1];
                            gamer.userPoint += board[i][j];
                        }
                        board[i][j + 1] += board[i][j];
                        board[i][j] = 0;
                        i = 0;
                        j = 0;
                        }
                        else {
                            if (board[i][j - 1] != 0) {
                            gamer.userPoint += board[i][j + 1];
                            gamer.userPoint += board[i][j];
                        }
                        board[i][j + 2] += board[i][j];
                        board[i][j] = 0;
                        i = 0;
                        j = 0;
                        }
                    }
                    if (j == 2) {
                        if (board[i][j - 1] != 0) {
                            gamer.userPoint += board[i][j + 1];
                            gamer.userPoint += board[i][j];
                        }
                        board[i][j + 1] += board[i][j];
                        board[i][j] = 0;
                        i = 0;
                        j = 0;
                    }
                }
            }
        }
}


void moveLeft() {  //to move tiles Left
    int temp2;
    for (int i = 0; i < 4; i++) {
            for (int j = 1; j < 4; j++) {

                if (board[i][j] == 0)
                    continue;

                if (board[i][j - 1] == 0 || board[i][j - 1] == board[i][j]) {
                    if (board[i][j - 1] != 0) {
                        gamer.userPoint += board[i][j - 1];
                        gamer.userPoint += board[i][j];
                    }
                    board[i][j - 1] += board[i][j];
                    board[i][j] = 0;
                    i = 0;
                    j = 1;

                }
            }
        }
    if (board[0][0] == 0) {
        for (int f = 1; f < 4; f++){
            temp2 = board[0][f];
            board[0][f] = board[0][f - 1];
            board[0][f - 1] = temp2;
        }
    }

}

void writeOnFile() {  //to write each player name and score on the file
    ofstream Score_Board("Score_Board.txt");

    Score_Board << gamer.userName << " ";
    Score_Board << gamer.userPoint << " ";
    Score_Board << gamer.isWinner << endl;

}

void readFromeFile() {  //to read name and score of users frome file
    ifstream Score_Board("Score_Board.txt");

    Score_Board >> player.userName;
    scoreArr[numberOFPlayer - 1].userName = player.userName;

    Score_Board >> player.userPoint;
    scoreArr[numberOFPlayer - 1].userPoint = player.userPoint;

    Score_Board >> player.isWinner;
    scoreArr[numberOFPlayer - 1].isWinner = player.isWinner;
}

void sortAndShow() { //main function for show the leader board
    bool swapped;

    user temp;

    for (int i = 0; i < 5 - 1; i++) {  //sort the users
        swapped = false;

        for (int j = 0; j < 5 - i - 1; j++) {

            if (scoreArr[j].userPoint < scoreArr[j + 1].userPoint) {

                temp = scoreArr[j];
                scoreArr[j] = scoreArr[j + 1];
                scoreArr[j + 1] = temp;

                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }

    system("cls");

    string form;

    if (numberOFPlayer == 0) {
        cout << RED << "We Dont Have Any Players !" << endl;
        cout << RESET << endl;
    }
    
    if (scoreArr[0].userPoint != 0) {
        cout << PURPLE << setw(10) << "NAME" << "         " << BLUE << setw(10) << "SCORE" << "         " << RESET << setw(10) << "HE WAS" << endl;
        cout << endl;

        if (scoreArr[0].isWinner) {  //is best guy a fack or real champion ?
            form = "WINNER";
            cout << LITE_BLUE << setw(10) << scoreArr[0].userName << "         " << YELLOW << setw(10) << scoreArr[0].userPoint << "         " << GREEN << setw(10) << form << endl;
        }
        else {
            form = "FACE_WINNER";
            cout << LITE_BLUE << setw(10) << scoreArr[0].userName << "         " << YELLOW << setw(10) << scoreArr[0].userPoint << "         " << GREEN << setw(10) << form << endl;
        }
    }

    for (int i = 1; i < 5; i++) {  //oder guys are winner or losser and show scores
        if (scoreArr[i].userPoint != 0) {
            if (scoreArr[i].isWinner) {
                form = "WINNER";
                cout << LITE_BLUE << setw(10) << scoreArr[i].userName << "         " << YELLOW << setw(10) << scoreArr[i].userPoint << "         " << GREEN << setw(10) << form << endl;
            }
            else {
                form = "LOOSER";
                cout << LITE_BLUE << setw(10) << scoreArr[i].userName << "         " << YELLOW << setw(10) << scoreArr[i].userPoint << "         " << RED << setw(10) << form << endl;
            }
        }
    }
    cout << endl;
    cout << endl;
}


void exit() {  //main function for exit
    exit(1);
}

int main(){
    system("cls");
    srand(static_cast<unsigned int>(time(nullptr)));

    while (true) {
        cout << LITE_BLUE << "------------Please Enter Your Choice------------" << endl;  //Menu
        cout << RED << "1) New Game ---> (Choose This TO Start a New Game)" << endl;
        cout << YELLOW << "2) Leaderboard ---> (Choose This TO See The Score Board)" << endl;
        cout << GREEN << "3) Exit ---> (Choose This TO Exit The Game)" << endl;
        cout << endl;

        cout << RESET << "Remember Move With : " << endl; //Guide
        cout << PURPLE << "               " << "W  w  UpKey" << endl;
        cout << LITE_BLUE << "A  a  LeftKey" << "                 " ;
        cout << GREEN << "D  d  RightKey" << endl;
        cout << BLUE << "               " << "S  s  DownKey" << endl;
        cout << RESET << endl;

        char userOrder;
        cout << "Enter Your Order : ";
        cin >> userOrder;

        switch (userOrder) {
            case '1':
                numberOFPlayer++;
                system("cls");
                newGame();
                break;
            case '2':
                sortAndShow();
                break;
            case '3':
                exit();
                break;
            default :
                cout << endl;
                cout << endl;
                system("cls");
                cerr << RED << "Please Enter a Valid Order !\n";
                cout << endl;
                cout << endl;
                break;
        }
    }
}
