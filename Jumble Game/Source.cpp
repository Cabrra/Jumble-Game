#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <list>
#include <algorithm>
#include <vector>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "HTable.h"	
#include "BST.h"

class DTSTimer
{
	unsigned int startTime;
public:
	DTSTimer()
	{
		reset();
	}

	void reset()
	{
		startTime = clock();
	}

	unsigned int getElapsedTime()
	{
		return (clock() - startTime);
	}

	static void delay(const unsigned int milli)
	{
		unsigned int st = clock();
		while (clock() - st < milli){}
	}

};

struct MScores{
	char name[50];
	int score;

	friend ostream& operator<<(ostream & os, MScores const& score);
};

ostream& operator<<(ostream & os, MScores const& score)
{
	return os << "\tName: " << score.name << "\tScore: " << score.score << '\n';
}

bool SortByScoreDesc(MScores const& left, MScores const& right)
{
	return left.score > right.score;
}

// best so far:
const unsigned int NUMHASHBUCKETS = 2677; // because it's prime // 4 empty buckets and range 0-21 -> (alpha / (v.length() ^ 42)) % 2677

unsigned int myHash(const string& v){  // 11 buckets empty and range 0 - 20 -> (alpha / (v.length() ^ 42 )) % 2969;
	unsigned int alpha = 1;

	for (unsigned int i = 0; i < v.length(); i++){
		alpha *= v[i];
	}
	alpha = (alpha / (v.length() ^ 42)) % NUMHASHBUCKETS; // meaning of life (42)
	//(alpha / (v.length() ^ 1455)) % 2671; // 1455 is the result of the multiplication of my name Jagoba Marcos

	return alpha;
}

bool RepeatOp();
void Tittle();
int Factorial(int x);

int main(int argc, char ** argv)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(static_cast<unsigned int>(time(nullptr)));
	////////////////////////////////////////////////////////////////////////////////////////////////
	// LEAK DETECTION
	////////////////////////////////////////////////////////////////////////////////////////////////
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// this function call will set a breakpoint at the location of a leaked block
	// set the parameter to the identifier for a leaked block
	//_CrtSetBreakAlloc();

	HTable<string> myHTable(NUMHASHBUCKETS, myHash);

	BST<string> guessed = BST<string>();
	ifstream dictionary;
	int myScore = 0;

	dictionary.open("engldict.txt");

	if (dictionary.is_open()){
		while (!dictionary.eof()){
			string word;

			getline(dictionary, word, '\n');

			if (!(word.find('\'') != string::npos) && word.length() > 2 && word.length() < 7)
				myHTable.insert(word);

		}
		dictionary.close();
	}

	string original;
	string shuffle;
	string lettersLeft;

	bool end = false;
	DTSTimer myTime;
	string player;

	Tittle();
	do{
		cout << "Enter your name: ";
		getline(cin, player);
	} while (strlen(player.c_str()) < 1);

	system("cls");

	while (!end){
		string userInput;
		while (original.length() != 6){
			int random = rand() % NUMHASHBUCKETS;
			int randomIndex = rand() % 500;
			original = myHTable.GiveListValue(NUMHASHBUCKETS, random, randomIndex);
		}

		shuffle = original;

		do{
			for (int i = 0; i < 24; i++){
				int temp = rand() % 6;
				int temp2 = rand() % 6;
				std::swap(shuffle[temp], shuffle[temp2]);
			}
		} while (shuffle == original);

		lettersLeft = shuffle;

		bool gameover = false;

		myTime.reset();
		while (myTime.getElapsedTime() < 60000){

			LockWindowUpdate(GetConsoleWindow());

			if (_kbhit())
			{
				char ch = _getch();

				if (GetAsyncKeyState(VK_ESCAPE)){
					userInput.clear();
					lettersLeft = shuffle;
				}

				if (GetAsyncKeyState(VK_BACK)){

					if (strlen(userInput.c_str()) > 0){
						int repeat = -1;
						for (unsigned int i = 0; i < strlen(userInput.c_str()); i++){
							if (userInput[strlen(userInput.c_str()) - 1] == userInput[i])
								repeat++;
						}
						for (unsigned int i = 0; i < strlen(shuffle.c_str()); i++){
							if (userInput[strlen(userInput.c_str()) - 1] == shuffle[i] && repeat == 0){
								lettersLeft[i] = userInput[strlen(userInput.c_str()) - 1];
								string aux = userInput;
								userInput.clear();
								for (unsigned int j = 0; j < strlen(aux.c_str()) - 1; j++)
									userInput += aux[j];
								break;
							}
							else if (userInput[strlen(userInput.c_str()) - 1] == shuffle[i] && repeat != 0)
								repeat--;
						}
					}
				}

				if (-32 == ch)
					_getch();

				// did the user press a-z?
				else if (isalpha(ch))
				{
					ch = tolower(ch);
				}

				bool found = false;

				for (unsigned int i = 0; i < strlen(lettersLeft.c_str()); i++){
					if (lettersLeft.find(ch) != std::string::npos){
						found = true;
						break;
					}
				}
				if (found){
					for (unsigned int i = 0; i < strlen(shuffle.c_str()); i++){
						if (ch == lettersLeft[i]){
							lettersLeft[i] = ' ';
							break;
						}
					}

					userInput += ch;
				}

				if (myHTable.find(userInput) != -1 && found && strlen(userInput.c_str()) >= 3){
					if (!guessed.find(userInput)){
						guessed.insert(userInput);
						//userInput.clear();
						//lettersLeft = shuffle;
						if (userInput == original){
							myScore += Factorial(strlen(userInput.c_str()) - 1);
							break;
						}
						else
							myScore += Factorial(strlen(userInput.c_str()) - 1);
						found = false;
					}
				}
			}

			system("cls");
			SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "Time left: ";
			if ((60 - myTime.getElapsedTime() / 1000) <= 20)
				SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_INTENSITY);
			else
				SetConsoleTextAttribute(out, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			cout << 60 - myTime.getElapsedTime() / 1000;
			
			SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "\tScore: "; 
			SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << myScore;
			SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "\tName: ";
			SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << player << "\n\n";

			SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "letters left: ";
			SetConsoleTextAttribute(out, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << lettersLeft << "\n";

			SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "\nYour input: ";
			SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << userInput << "\n\n";

			SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "Guessed words:\n";
			SetConsoleTextAttribute(out, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			guessed.printInOrder();

			SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "\n\n(Press <ESC> to clear input)";

			LockWindowUpdate(NULL);
			Sleep(10);
			if (myTime.getElapsedTime() > 60000)
				gameover = true;
		}
		if (gameover){
			system("cls");
			cout << "The word was: " << original << "\n\n";
			if (!RepeatOp())
				end = true;
			else
				myScore = 0;
		}
		guessed.clear();
		original.clear();
		userInput.clear();
		shuffle.clear();
	}

	system("cls");
	// save game: I do the save here to avoid multiple entries in a single game.

	ofstream binout;
	MScores myTotal;
	strcpy_s(myTotal.name, sizeof (player), player.c_str());
	myTotal.score = myScore;

	binout.open("scores.bin", ios_base::app | ios_base::binary);

	if (binout.is_open())
	{
		binout.write((char *)myTotal.name, (sizeof myTotal));
		binout.close();
	}

	// open file
	COORD position;

	position.X = 23;
	position.Y = 5;
	SetConsoleCursorPosition(out, position);

	cout << "HIGHSCORES";
	vector<MScores> vecScores;
	ifstream binfin;;

	binfin.open("scores.bin", ios_base::in | ios_base::binary);
	if (binfin.is_open()){
		binfin.seekg(0, binfin.end);
		int count = (int)binfin.tellg();
		count /= sizeof(MScores);

		binfin.seekg(0, binfin.beg);

		for (int i = 0; i < count; ++i)
		{
			MScores temp;
			binfin.read((char*)&temp, sizeof(MScores));
			vecScores.push_back(temp);
		}

		binfin.close();

	}

	//for (unsigned int i = 0; i < vecScores.size(); ++i)
	//{
	//	cout << "Name: " << vecScores[i].name << "\tScore:  " << vecScores[i].score << "\n";
	//}

	list<MScores> scoreList(vecScores.begin(), vecScores.end());

	scoreList.sort(SortByScoreDesc);
	int count = 0;
	position.Y++;
	for each (MScores score in scoreList){
		if (count == 5)
			break;
		position.X = 15;
		position.Y++;
		SetConsoleCursorPosition(out, position);

		cout << count+1 << ") " << score;
		count++;
	}

	position.X = 15;
	position.Y = 33;
	SetConsoleCursorPosition(out, position);


	SetConsoleTextAttribute(out, FOREGROUND_GREEN | FOREGROUND_INTENSITY);	
	cout << "GOOD BYE!!";

	position.X = 20;
	position.Y = 34;
	SetConsoleCursorPosition(out, position);

	SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	system("pause");
	return 0;
}

bool RepeatOp(){

	cout << "Do you want to replay? <Y>es/<N>o ";

	while (true){
		char ch = _getch();

		if (ch == 'Y' || ch == 'y')
			return true;
		if (ch == 'N' || ch == 'n')
			return false;
	}

}

void Tittle(){

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	
	
	SetConsoleTextAttribute(out, FOREGROUND_RED);
	cout << "       ___         ___           ___           ___           ___       ___     \n";
	cout << "      /\\  \\       /\\__\\         /\\__\\         /\\  \\         /\\__\\     /\\  \\    \n";
	cout << "      \\:\\  \\     /:/  /        /::|  |       /::\\  \\       /:/  /    /::\\  \\   \n";
	cout << "  ___ /::\\__\\   /:/  /        /:|:|  |      /:/\\:\\  \\     /:/  /    /:/\\:\\  \\  \n";
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << " /\\  /:/\\/__/  /:/  /  ___   /:/|:|__|__   /::\\~\\:\\__\\   /:/  /    /::\\~\\:\\  \\ \n";
	cout << " \\:\\/:/  /    /:/__/  /\\__\\ /:/ |::::\\__\\ /:/\\:\\ \\:|__| /:/__/    /:/\\:\\ \\:\\__\\ \n";
	cout << "  \\::/  /     \\:\\  \\ /:/  / \\/__/~~/:/  / \\:\\~\\:\\/:/  / \\:\\  \\    \\:\\~\\:\\ \\/__/ \n";
	cout << "   \\/__/       \\:\\  /:/  /        /:/  /   \\:\\ \\::/  /   \\:\\  \\    \\:\\ \\:\\__\\  \n";
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "                \\:\\/:/  /        /:/  /     \\:\\/:/  /     \\:\\  \\    \\:\\ \\/__/  \n";
	cout << "                 \\::/  /        /:/  /       \\::/__/       \\:\\__\\    \\:\\__\\    \n";
	cout << "                  \\/__/         \\/__/         ~~            \\/__/     \\/__/    \n";


	cout << "      ___           ___           ___           ___           ___       ___     \n";
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "     /\\  \\         /\\__\\         /\\__\\         /\\  \\         /\\__\\     /\\  \\    \n";
	cout << "    /::\\  \\       /:/  /        /::|  |       /::\\  \\       /:/  /    /::\\  \\   \n";
	cout << "   /:/\\:\\  \\     /:/  /        /:|:|  |      /:/\\:\\  \\     /:/  /    /:/\\:\\  \\  \n";
	cout << "  /::\\~\\:\\__\\   /:/  /  ___   /:/|:|__|__   /::\\~\\:\\__\\   /:/  /    /::\\~\\:\\  \\ \n";
	SetConsoleTextAttribute(out, FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << " /:/\\:\\ \\:|__| /:/__/  /\\__\\ /:/ |::::\\__\\ /:/\\:\\ \\:|__| /:/__/    /:/\\:\\ \\:\\__\\\n";
	cout << " \\:\\~\\:\\/:/  / \\:\\  \\ /:/  / \\/__/~~/:/  / \\:\\~\\:\\/:/  / \\:\\  \\    \\:\\~\\:\\ \\/__/\n";
	cout << "  \\:\\ \\::/  /   \\:\\  /:/  /        /:/  /   \\:\\ \\::/  /   \\:\\  \\    \\:\\ \\:\\__\\  \n";
	cout << "   \\:\\/:/  /     \\:\\/:/  /        /:/  /     \\:\\/:/  /     \\:\\  \\    \\:\\ \\/__/  \n";
	SetConsoleTextAttribute(out, FOREGROUND_BLUE);
	cout << "    \\::/__/       \\::/  /        /:/  /       \\::/__/       \\:\\__\\    \\:\\__\\    \n";
	cout << "     ~~            \\/__/         \\/__/         ~~            \\/__/     \\/__/ \n\n";

	SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int Factorial(int x){

	if (x <= 1) return 1;

	int temp = x * Factorial(x - 1);
	return temp;
}