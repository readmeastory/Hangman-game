#include <iostream>
#include <string>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

//making hangman game
//
// 
//I copied and pasted some parts of the code from Github and it's okay to do this sometimes for personal projects
//especially if some part is really long and I understand it or mostly understand it 
//and other times it can be good to do everything entirely by me. I understood most of the code I was copy and pasting
//so, that can be a good reason sometimes to copy and paste parts of other people's code. 
//After copy and pasting some parts I still tried to add onto it on my own before copy and pasting other parts
// and I'll also keep looking over it to learn more things and I'll keep making some changes to try to make it better. 
//Like I changed the names of the variables to make it make more sense and I'm trying
//to make more changes in other ways to make it run better and more efficiently and also to make it more personal and make 
//more sense to me.

using std::cout;
using std::cin;
using std::string;

class Hangman {
    string word_from_host;
    string guessed_right_characters;
    string hint;
    int player_1_wins = 0, player_2_wins = 0;
    int rounds_played = 1;
    string player_1_name, player_2_name;

public:
    HANDLE H_Console = GetStdHandle(STD_OUTPUT_HANDLE); // handle to the output device

    /* function to set cursor position on output window */
        //parameters (x,y) coordinates
    void gotoXY(int X, int Y)
    {
        COORD position;
        position.X = X;
        position.Y = Y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
    }
	/*
			*function to print boxes on the screen*
			*parameters : (x,y) coordinates ; length and width of the box
		*/
	void printBox(int X, int Y, int width, int length)
	{
		/*
		U_L_C = Upper Left Corner
		U_R_C = Upper Right Corner
		L_L_C = Lower Left Corner
		L_R_C = Lower Right Corner
		H_L	= Horizontal Line
		V_L = Vertical Line
		*/

		char U_L_C = 201, U_R_C = 187, L_L_C = 200, L_R_C = 188;

		char H_L = 205, V_L = 186;

		width -= 2;	length -= 2;

		//Printing horizontal line
		gotoXY(X, Y);
		cout << U_L_C;

		for (int i = 0; i < width; i++)
			cout << H_L;

		cout << U_R_C;

		//Printing vertical lines

		for (int i = 0; i < length; i++)
		{
			gotoXY(X, Y + (i + 1));
			cout << V_L;
			gotoXY(X + (width + 1), Y + (i + 1));
			cout << V_L;
		}

		//Printing horizontal line
		gotoXY(X, Y + length);
		cout << L_L_C;

		for (int i = 0; i < width; i++)
			cout << H_L;

		cout << L_R_C;
	}

	// function to clear the screen without clearing the border
	void clearScreen()
	{
		//	Clearing screen from point (3,3) to (77,27)
		for (int i = 0; i < 24; i++)
		{
			gotoXY(3, 1 + i);

			for (int j = 0; j < 74; j++)
				cout << " ";

		}
	}



	// function to display the start page of the game
	void hangmanGame()
	{

		while (true)
		{
			clearScreen();

			char logo[7][75];

			strcpy_s(logo[0], "HH    HH     AA     NN    NN     GGG    MM          MM     AA     NN    NN");
			strcpy_s(logo[1], "HH    HH   AA  AA   NNN   NN   GG   GG  MMM        MMM   AA  AA   NNN   NN");
			strcpy_s(logo[2], "HH    HH  AA    AA  NNNN  NN  GG        MMMM      MMMM  AA    AA  NNNN  NN");
			strcpy_s(logo[3], "HHHHHHHH  AAAAAAAA  NN NN NN  GG        MM MM    MM MM  AAAAAAAA  NN NN NN");
			strcpy_s(logo[4], "HH    HH  AA    AA  NN  NNNN  GG   GGG  MM  MM  MM  MM  AA    AA  NN  NNNN");
			strcpy_s(logo[5], "HH    HH  AA    AA  NN   NNN   GG   GG  MM   MMMM   MM  AA    AA  NN   NNN");
			strcpy_s(logo[6], "HH    HH  AA    AA  NN    NN     GGG    MM    MM    MM  AA    AA  NN    NN");

			//Printing logo
			for (int i = 0; i < 7; i++)
			{
				SetConsoleTextAttribute(H_Console, 12);
				gotoXY(3, 1 + i);
				puts(logo[i]);
			}

			SetConsoleTextAttribute(H_Console, 9);

			printBox(28, 8, 30, 14); // box for the menu

			printBox(14, 22, 22, 4); //player1 score box

			printBox(46, 22, 22, 4); //player2 score box

			//menu starts
			/*---------------------------MENU-------------------------*/
			gotoXY(31, 10);
			cout << "(1)->  Play game ";

			gotoXY(31, 12);
			cout << "(2)->  Change Names ";

			gotoXY(31, 14);
			cout << "(3)-> How to play";

			gotoXY(31, 16);
			cout << "(4)-> Store Word and Hint";

			gotoXY(31, 18);
			cout << "(5)->    Exit    ";

			//menu ends

			/*---------------------------MENU-------------------------*/

			gotoXY(18, 23);
			cout << player_1_name << " : " << player_1_wins; //display player 1 name and score

			gotoXY(50, 23);
			cout << player_2_name << " : " << player_2_wins; //display player 2 name and score

			//Selecting option
			gotoXY(0, 29);
			char ch;
			cin >> ch;

			if (ch == '1')
				startGame(); //play game

			else if (ch == '2') // take player 1 and player 2 name as input
			{
				SetConsoleTextAttribute(H_Console, 12);
				clearScreen();
				gotoXY(31, 16);
				cout << "Player 1 : ";
				gotoXY(31, 18);
				std::getline(cin, player_1_name);
				gotoXY(31, 20);
				cout << "Player 2 : ";
				gotoXY(31, 22);
				std::getline(cin, player_2_name);
				continue;
			}

			else if (ch == '3')
				instructions(); //display instructions

			else if (ch == '4') //take word to be guessed and hint as input
			{
				SetConsoleTextAttribute(H_Console, 12);
				clearScreen();
				char word1[100];
				gotoXY(31, 16);
				cout << "Enter the secret word"; //enter word to be guessed
				gotoXY(31, 18);
				cin >> word1;
				storetheword(word1); //store word
				gotoXY(31, 20);
				cout << "Enter a hint : \n"; //enter a hint for the other player
				gotoXY(31, 22);
				getline(cin, hint); //store hint
				continue;
			}

			else if (ch == '5')
				exit(0); //exit program

			else
				continue;

		}

	}

	//function that initializes game
	void startGame()
	{

		if (word_from_host == "" || word_from_host.size() <= 1)  // if initially no word is entered or word has only one letter, user is asked to enter a valid word
		{
			SetConsoleTextAttribute(H_Console, 12);
			clearScreen();
			gotoXY(25, 12);
			cout << "Enter a valid word first!";
			gotoXY(25, 14);
			cout << "Press any key to return to Menu";
			char any;
			cin >> any;
			return;
		}
		while (true)
		{
			rounds_played ++; //total rounds is incremented by one
			playGame(); // game starts
//				int check = 0;
			char chek;
			cin >> chek; // after the round, control returns here, from where it is redirected back to hangmanGame()
			break;
		}
		hangmanGame();

	}



	//function where letters are guessed
	void playGame()
	{
		clearScreen();

		int lives = 0, check = 0;

		char guess[100], input; // a variable guess is used to encrypt the word to be guessed , input is the guessed character

		string word; //word to be guessed

		//Getting and storing word
		word = getWord();

		//Converting word into astericks
		encryptWord(word, guess);

		//Boxes to display tries left, the hangman, etc
		printBox(10, 3, 19, 4);
		printBox(56, 3, 14, 4);
		printBox(46, 9, 24, 17);

		while (true)
		{

			//Printing word to guess
			gotoXY(12, 4);
			cout << "Word : ";
			puts(guess);

			gotoXY(12, 6);
			cout << "Hint : "; //display hint
			cout << hint;

			//Printing remaining lives
			gotoXY(58, 4);
			cout << "Lives : " << (7 - lives);

			//Displaying man hanging
			hanging(lives);

			if (lives == 7)	//If lives are finished
				break;

			if (checkWord(word, guess))
			{	//If word has been guessed
				check = 1;
				break;
			}

			//Taking guess
			gotoXY(12, 11);
			cout << "Enter your guess : ";
			char input;
			cin >> input;

			gotoXY(14, 14);

			if (checkGuess(word, guess, input))
			{
				//If guessed character is found
				cout << "                        ";
				gotoXY(14, 14);
				cout << "Good guess!";
			}

			else
			{
				//If guessed character isn't found
				cout << "One step closer to death";
				lives++; //lives is incremented and once it reaches 7, player loses
			}

		}

		//Printing result
		printBox(10, 18, 25, 8);
		SetConsoleTextAttribute(H_Console, 12);
		gotoXY(18, 20);

		if (check == 1)
		{	//if word is guessed correctly check is set to 1
			cout << "YOU WIN!";

			/* if count is odd, player 1 is currently playing, otherwise it's player 2*/
			if (!(rounds_played & 1)) // if count is odd
				player_1_wins++; //increment score of player 1 by 1

			else //if count is even
				player_2_wins += 1; //increment score of player 2 by 1
		}

		else //if player has run of tries and word isn't correct
			cout << "YOU LOSE!";

		SetConsoleTextAttribute(H_Console, 9);
	}


	/*function that returns word to be guessed*/
	string getWord()
	{
		return word_from_host;
	}



	//function to mask/encrypt word to be guessed
	// parameters word and partially guessed word
	void encryptWord(string word, char* guess)
	{
		int i = 1;

		//Storing first character of word as it is
		guess[0] = word[0];

		//Storing all other characters of word as astericks
		while (word[i] != '\0')
		{
			guess[i] = '*';
			i++;
		}

		//Storing NULL at the last index
		guess[i] = '\0';
	}


	//function to display how close the person is to being hanged depending on number of tries/lives left
	//parameter : number of wrong guesses so far
	void hanging(int wrong)
	{

		static char hang[12][20];

		if (wrong == 0)
		{
			strcpy_s(hang[0], " _____________    ");
			strcpy_s(hang[1], " |           |    ");
			strcpy_s(hang[2], " |           |    ");
			strcpy_s(hang[3], "             |    ");
			strcpy_s(hang[4], "             |   ");
			strcpy_s(hang[5], "             |    ");
			strcpy_s(hang[6], "             |   ");
			strcpy_s(hang[7], "             |    ");
			strcpy_s(hang[8], "             |    ");
			strcpy_s(hang[9], "             |    ");
			strcpy_s(hang[10], "             |    ");
			strcpy_s(hang[11], "         ---------");
		}

		else if (wrong == 1)
			strcpy_s(hang[3], " O           |    ");

		else if (wrong == 2)
			strcpy_s(hang[4], " |           |   ");

		else if (wrong == 3)
			strcpy_s(hang[4], "/|           |   ");

		else if (wrong == 4)
			strcpy_s(hang[4], "/|\\          |   ");

		else if (wrong == 5)
			strcpy_s(hang[5], " |           |    ");

		else if (wrong == 6)
			strcpy_s(hang[6], "/            |   ");

		else
			strcpy_s(hang[6], "/ \\          |   ");

		//Displaying man hanging

		for (int i = 0; i < 12; i++)
		{
			if (wrong == 7)
				SetConsoleTextAttribute(H_Console, 12);
			gotoXY(49, 11 + i);	puts(hang[i]);
		}

		if (wrong == 7)
			SetConsoleTextAttribute(H_Console, 9);
	}

	//function to check if guessed letter is correct
	//parameters word, partially complete guessed word, guessed letter
	bool checkGuess(string word, char* guess, char ch)
	{
		bool check = false;
		int i = 0;

		//Converting character to uppercase
		ch = toupper(ch);

		char temp; //temp variable to store uppercase of word

		while (word[i] != '\0')
		{
			temp = toupper(word[i]);

			if (temp == ch)
			{
				//If character is found in word
				guess[i] = word[i]; //store character in guess, thus revealing all occurences of the character guessed
				check = true; //set check as true, i.e., guessed character is valid
			}
			i++; //increment i to check next index
		}
		return check;
	}

	// check if guessed word is same as word to be guessed
	// parameters, word and guessed word
	bool checkWord(string word, char* guess)
	{
		int i = 0;
		while (word[i] != '\0') {
			if (word[i] != guess[i])	//If any character didn't matched
				return false;
			i++;
		}
		return true;
	}

	//function to print instructions on the screen
	void instructions()
	{
		SetConsoleTextAttribute(H_Console, 12);
		clearScreen();
		char about[19][78];
		strcpy_s(about[0], "                                :: HANGMAN ::                                ");
		strcpy_s(about[1], "                                                                             ");
		strcpy_s(about[2], "                   In this game, you have to guess a word!                   ");
		strcpy_s(about[3], "             													               ");
		strcpy_s(about[4], "                                                                             ");
		strcpy_s(about[5], "                 You will have 7 lives for guessing the word                 ");
		strcpy_s(about[6], "                        If you guessed the word right                        ");
		strcpy_s(about[7], "                       	you win the round!				               ");
		strcpy_s(about[8], "                                                                             ");
		strcpy_s(about[9], "                But if you're not able to guess the word and                 ");
		strcpy_s(about[10], "                   lives finish then you lose the round!                    ");
		strcpy_s(about[11], "                                                                            ");
		strcpy_s(about[12], "                     First, Player 1 must enter word to be                   ");
		strcpy_s(about[13], "                                                                             ");
		strcpy_s(about[14], "                          guessed, along with the hint                       ");
		strcpy_s(about[15], "                                                                             ");
		strcpy_s(about[16], "                         Player 2 will try to guess the word                 ");
		strcpy_s(about[17], "                                                                             ");
		strcpy_s(about[18], "                          Positions swap after each round                    ");
		//printing Instruction
		for (int i = 0; i < 19; i++) {
			gotoXY(1, 5 + i); puts(about[i]);
		}
		gotoXY(0, 29);
		char a;
		cin >> a;
		hangmanGame();
	}




	void storetheword(string x)//simply storing the secret phrase in a private member
	{
		word_from_host = x;
	}

};

void draw(string word) {
    system("cls");
    int size = word.size();
    string lines(size, '_');
    cout << "The word you have to guess is " << size << " characters long.\n";
    cout << lines << "\n";
    cout << " __ \n";
    cout << "|  |\n";
    cout << "|\n";
    cout << "|\n";
    cout << "|\n";
    cout << "|_\n";
}

//bool input() {

//}

//bool game_over() {

//}

int main()
{
    //Hangman instance;
    //int word_size, guesses = 0, allowed_guesses;
    //bool guessed_right, game_over = false;
    //cout << "For host, what is the word you want to make the other player guess?\n";
    //cin >> word_from_host;
    //while (game_over == false) {
        //draw(word_from_host);
    //}
	Hangman instance; //create an object instance of class hangman

	system("title HANGMAN");	//Setting title
	system("mode 80,30");		//Setting screen mode
	system("color 09");			//Setting color

	instance.printBox(0, 0, 80, 30);		//Printing box border

	instance.hangmanGame();						//Displaying menu

}
