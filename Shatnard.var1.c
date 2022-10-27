#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
// #include <conio.h>
#include "include/console.h";
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

struct DICE
{
	int first;
	int second;
	int is_double; // whether the two dice are equal (a double)
	int previous_move; // the player's previous move; 0 means no move has been made yet
} dice ;

const struct PIECE
{
	char type;
	int number;
} empty = { 0, 0 };

struct PLAYER
{
	int remaining_pieces;// pieces not yet borne off (to O)
	int coins; // number of coins the player has
	int hole_turns; // remaining turns trapped in a hole
	int prisoner_count; // number of imprisoned pieces
	struct PIECE prisoner_pieces[15];
}p1 = { 15, 0, 0 , 0, { 0 } }, p2 = { 15, 0, 0 , 0, { 0 } };

struct ROOM
{
	char type; // 'R' for Road, 'C' for Castle
	int status;// 2: has a coin   1: has a hole   0: empty
	int piece_count;// number of pieces currently in this room
	int resident; // which player's pieces occupy this room
	struct PIECE pieces[5];// the pieces held in this room
} board[24] = {
{ 'C', 0, 2, 1, { { 'K', 1 }, { 'Q', 1 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },				{ 'C', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }} },
{ 'C', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },					{ 'C', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },
{ 'C', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },					{ 'C', 0, 5, 2, { { 'C', 2 }, { 'S', 2 }, { 'H', 2 }, { 'S', 2 }, { 'C', 2 } } },
{ 'R', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },					{ 'R', 0, 3, 2, { { 'S', 2 }, { 'E', 2 }, { 'S', 2 }, { 0, 0 }, { 0, 0 } } },
{ 'R', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },					{ 'R', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },
{ 'R', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },					{ 'R', 0, 5, 1, { { 'S', 1 }, { 'H', 1 }, { 'S', 1 }, { 'S', 1 }, { 'E', 1 } } },
{ 'R', 0, 5, 2, { { 'S', 2 }, { 'H', 2 }, { 'S', 2 }, { 'S', 2 }, { 'E', 2 } } },		{ 'R', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },
{ 'R', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },					{ 'R', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },
{ 'R', 0, 3, 1, { { 'S', 1 }, { 'E', 1 }, { 'S', 1 }, { 0, 0 }, { 0, 0 } } },			{ 'R', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },
{ 'C', 0, 5, 1, { { 'C', 1 }, { 'S', 1 }, { 'H', 1 }, { 'S', 1 }, { 'C', 1 } } },		{ 'C', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },
{ 'C', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },					{ 'C', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },
{ 'C', 0, 0, 0, { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } },					{ 'C', 0, 2, 2, { { 'K', 2 }, { 'Q', 2 }, { 0, 0 }, { 0, 0 }, { 0, 0 } } }
}; // initial placement of pieces   0-5: Player2's Castle   6-11: Road Number 2   12-17: Road Number 1   18-23: Player1's Castle

int turn = 1;

int are_equal(struct PIECE a1, struct PIECE a2)// check whether two pieces are equal
{
	if (a1.type == a2.type && a2.number == a2.number)
		return 1;
	return 0;
}

void piece_to_string(struct PIECE m, char *piece_str)
{
 
	piece_str[0] = m.type;
	piece_str[1] = '_';
	piece_str[2] = (char)m.number + '0';// 'number' is an integer converted to a char digit
	if (are_equal(m, empty))
		piece_str[0] = piece_str[1] = piece_str[2] = ' ';
}

struct PIECE string_to_piece(char s[])
{
	struct PIECE m = { toupper(s[0]), s[2] - '0' };
	return m;
}

void room_to_string(int index,char *room_str)
{
	if (index >= 6 && index <= 17)// follows how the rooms are ordered in the array
		room_str[0] = 'R';
	else
		room_str[0] = 'C';

	if (index <= 11)
	{
		room_str[1] = '1';
		room_str[3] = (index % 6) + 1 + '0'; // follows how the rooms are ordered in the array
	}
	else
	{
		int t = ( 23 - index ) % 6 + 1;
		room_str[1] = '2';
		room_str[3] = t + '0'; // convert the integer digit to a char digit
	}
	room_str[2] = '_';
	room_str[4] = '\0';
}

int string_to_room(char *room_str)
{
	char region = toupper( room_str[0] ) ;
	int field_number = room_str[1] - '0', room_number = room_str[3] - '0';
	room_str[4] = '\0';
	if (room_str[2] != '_'){
		return -2; // validate that the input format is correct
	}
	if (region == 'O')
	{
		if (turn == 1)
			return 24;
		else
			return -1;
	}
	if (region == 'C')
	{
		if (room_number >= 1 && room_number <= 6)
		{
			if (field_number == 2)
				return room_number - 1;
			if (field_number == 1)
				return 24 - room_number;
		}
	}

	if (region == 'R')
	{
		if (room_number >= 1 && room_number <= 6)
		{
			if (field_number == 2)
				return 5 + room_number;
			if (field_number == 1)
				return 12 + (6 - room_number);
		}
	}
	return -2;
}

void display()
{
	int i, j;
	char piece_str[4] = { 0 };
	printf("Player2's Castle\tRoad Number 2\n");
	for (j = 1; j <= 2; j++)
	{
		for (i = 1; i <= 6; i++)
		{
			printf(" %d ", i);
		}
		putchar(' ');
	}
	putchar('\n');

	for (j = 0; j <= 4; j++)
	{
		for (i = 0; i <= 11; i++)
		{
			if (i == 6)
				putchar('|');
			piece_to_string(board[i].pieces[j],piece_str);
			printf("%s", piece_str);
		}
		putchar('\n');
	}
	
	for (j = 0; j <= 1; j++)
	{
		for (i = 0; i <= 11; i++)
		{
			if (i == 6)
				putchar('|');
			printf("   ");
		}
		putchar('\n');
	}
	for (j = 4; j >= 0; j--)
	{
		for (i = 23; i >= 12; i--)
		{
			if (i == 17)
				putchar('|');
			piece_to_string(board[i].pieces[j], piece_str);
			printf("%s", piece_str);
		}
		putchar('\n');
	}

	for (j = 1; j <= 2; j++)
	{
		for (i = 1; i <= 6; i++)
		{
			printf(" %d ", i);
		}
		putchar(' ');
	}
	putchar('\n');
	printf("Player1's Castle\tRoad Number 1\n\n\n");
}

int is_valid_piece(char m)
{
	switch (toupper(m))
	{
	case 'Q':
	case 'K':
	case 'C':
	case 'S':
	case 'H':
	case 'E':
		return 1;
	default:
		return 0;
	}
	return 0;
}

int can_bear_off(int source)
{
	int i;
	if (turn == 1)// if all of a player's pieces are home in the castle, none may remain elsewhere
	{
		if (source < 18)
			return 0;
		for (i = source - 1; i >= 0; i--)
			if (board[i].resident == 1)
				return 0;
	}
	else
	{
		if (source > 5)
			return 0;
		for (i = source + 1; i <= 23; i++)
			if (board[i].resident == 2)
				return 0;
	}
	if (dice.previous_move != 1 && dice.first > source)
	{
		dice.previous_move = 1;
		return 1;
	}
	else if (dice.previous_move != 2 && dice.second > source)
	{
		dice.previous_move = 2;
		return 1;
	}
	return 0;
}

int validate_move(int source, int destination) // 0: invalid move   1: valid move   -1: win
{
	int i,j;
	// handle re-entering an imprisoned piece onto the board
	if (source == 24)
	{

		if (board[destination].piece_count == 5)
		{
			return 0;
		}
		if (turn == 1)
		{

			if (destination != dice.first-1 && destination != dice.second-1)
				return 0;
			if (dice.previous_move != 1 && destination == dice.first-1)
			{
				dice.previous_move = 1;
			}
			else if (dice.previous_move != 2 && destination == dice.second-1)
			{
				dice.previous_move = 2;
			}
			else
				return 0;
			if (board[destination].resident != 2)
			{
				board[destination].pieces[board[destination].piece_count].type = (char) (source - 24); // see the explanation of this formula in main()
				board[destination].pieces[board[destination].piece_count].number = 1;
				board[destination].resident = 1;
				board[destination].piece_count++;
				for (i = 0; i < p1.prisoner_count; i++)
				{
					if (p1.prisoner_pieces[i].type == (char)(source - 24))
					{
						for (j = 0; i + j < p1.prisoner_count; j++)
						{
							p1.prisoner_pieces[i + j].type = p1.prisoner_pieces[i + j + 1].type;
							p1.prisoner_pieces[i + j].number = p1.prisoner_pieces[i + j + 1].number;
						}
					}
				}
			}
			else if (board[destination].piece_count == 1)
			{
				char t = board[destination].pieces[0].type;
				board[destination].pieces[0].type = (char)(source - 24);
				board[destination].pieces[0].number = 1;
				board[destination].resident = 1;
				for (i = 0; i < p1.prisoner_count; i++)
				{
					if (p1.prisoner_pieces[i].type == (char)(source - 24))
					{
						for (j = 0; i + j < p1.prisoner_count; j++)
						{
							p1.prisoner_pieces[i + j].type = p1.prisoner_pieces[i + j + 1].type;
							p1.prisoner_pieces[i + j].number = p1.prisoner_pieces[i + j + 1].number;
						}
					}
				}
				if (p2.coins == 0)
				{
					p2.prisoner_pieces[p2.prisoner_count].type = t;
					p2.prisoner_pieces[p2.prisoner_count].number = 2;
					p2.prisoner_count++;
				}
				else
				{
					int i;
					for (i = 23; i >= 18; i--) // find the first empty room in the opponent's castle
						if (board[i].piece_count == 0)
							break;
					if (i >= 18) // if the opponent's castle has an empty room
					{
						char answer;
						printf("\nPlayer2, do you want to use your coins? ( number of your coins: %d )\n", p2.coins);
						printf("1. No 2. Yes\n");
						while ((answer = getch()) != '1' && answer != '2')
							printf("Wrong answer,try again!\n");
						if (answer == '2')
						{
							p2.coins--;
							board[i].pieces[0].type = t;
							board[i].pieces[0].number = 2;
							board[i].resident = 2;
							board[i].piece_count++;
						}
					}
				}
			}
		}
		else
		{
			if (destination != dice.first - 1 && 24 - destination != dice.second)
				return 0;
			if (dice.previous_move != 1 && 24 - destination == dice.first - 1)
			{
				dice.previous_move = 1;
			}
			else if (dice.previous_move != 2 && 24 - destination == dice.second - 1)
			{
				dice.previous_move = 2;
			}
			else
				return 0;
			if (board[destination].resident != 2)
			{
				board[destination].pieces[board[destination].piece_count].type = (char)(source - 24); // see the explanation of this formula in main()
				board[destination].pieces[board[destination].piece_count].number = 1;
				board[destination].resident = 1;
				board[destination].piece_count++;
				for (i = 0; i < p2.prisoner_count; i++)
				{
					if (p2.prisoner_pieces[i].type == (char)(source - 24))
					{
						for (j = 0; i + j < p2.prisoner_count; j++)
						{
							p2.prisoner_pieces[i + j].type = p2.prisoner_pieces[i + j + 1].type;
							p2.prisoner_pieces[i + j].number = p2.prisoner_pieces[i + j + 1].number;
						}
					}
				}
			}
			else if (board[destination].piece_count == 1)
			{
				char t = board[destination].pieces[0].type;
				board[destination].pieces[0].type = (char)(source - 24);;
				board[destination].pieces[0].number = 1;
				board[destination].resident = 1;
				for (i = 0; i < p2.prisoner_count; i++)
				{
					if (p2.prisoner_pieces[i].type == (char)(source - 24))
					{
						for (j = 0; i + j < p2.prisoner_count; j++)
						{
							p2.prisoner_pieces[i + j].type = p2.prisoner_pieces[i + j + 1].type;
							p2.prisoner_pieces[i + j].number = p2.prisoner_pieces[i + j + 1].number;
						}
					}
				}
				if (p2.coins == 0)
				{
					p2.prisoner_pieces[p2.prisoner_count].type = t;
					p2.prisoner_pieces[p2.prisoner_count].number = 2;
					p2.prisoner_count++;
				}
				else
				{
					int i;
					for (i = 23; i >= 18; i--) // find the first empty room in the opponent's castle
						if (board[i].piece_count == 0)
							break;
					if (i >= 18) // if the opponent's castle has an empty room
					{
						char answer;
						printf("\nPlayer2, do you want to use your coins? ( number of your coins: %d )\n", p2.coins);
						printf("1. No 2. Yes\n");
						while ((answer = getch()) != '1' && answer != '2')
							printf("Wrong answer,try again!\n");
						if (answer == '2')
						{
							p2.coins--;
							board[i].pieces[0].type = t;
							board[i].pieces[0].number = 2;
							board[i].resident = 2;
							board[i].piece_count++;
						}
					}
				}
			}
		}
	}


	// handle ordinary moves
	if (board[source].piece_count == 0 || (destination != 24 && board[destination].piece_count == 5))
		return 0;
	if (board[source].resident != turn)
		return 0;


	if (turn == 1)
	{

		if (dice.previous_move != 1 && destination == source + dice.first)
		{
			dice.previous_move = 1;
		}
		else if (dice.previous_move != 2 && destination == source + dice.second)
		{
			dice.previous_move = 2;
		}
		else if (can_bear_off(source))
		{
			if (destination != 24)
				return 0;

		}
		else if (destination != source + dice.first && destination != source + dice.second)
		{
			return 0;
		}

		board[source].piece_count--;
		if (board[source].piece_count == 0)
			board[source].resident = 0;
		if (destination == 24)
		{
			p1.remaining_pieces--;
			if (p1.remaining_pieces == 0)
				return -1;
		}
		else if (board[destination].resident != 2)
		{
			board[destination].pieces[ board[destination].piece_count ].type = board[source].pieces[ board[source].piece_count ].type;
			board[destination].pieces[board[destination].piece_count].number = 1;
			board[destination].resident = 1;
			board[destination].piece_count++;
		}
		else if (board[destination].piece_count == 1)
		{
			char t = board[destination].pieces[0].type;
			board[destination].pieces[0].type = board[source].pieces[board[source].piece_count].type;
			board[destination].pieces[0].number = 1;
			board[destination].resident = 1;
			if (p2.coins == 0)
			{
				p2.prisoner_pieces[p2.prisoner_count].type = t;
				p2.prisoner_pieces[p2.prisoner_count].number = 2;
				p2.prisoner_count++;
			}
			else
			{
				int i;
				for (i = 23; i >= 18; i--) // find the first empty room in the opponent's castle
					if (board[i].piece_count == 0)
						break;
				if (i >= 18) // if the opponent's castle has an empty room
				{
					char answer;
					printf("Player2, do you want to use your coins? ( number of your coins: %d )\n", p2.coins);
					printf("1. No 2. Yes\n");
					while ((answer = getch()) != '1' && answer != '2')
						printf("Wrong answer,try again!\n");
					if (answer == '2')
					{
						p2.coins--;
						board[i].pieces[0].type = t;
						board[i].pieces[0].number = 2;
						board[i].resident = 2;
						board[i].piece_count++;
					}
				}
			}
		}
		else
		{
			return 0;
		}
		board[source].pieces[board[source].piece_count].type = 0;
		board[source].pieces[board[source].piece_count].number = 0;
		if (destination != 24)
		{
			if (board[destination].status == 1)
				p1.hole_turns = 2;
			else if (board[destination].status == 2)
			{
				p1.coins++;
				board[destination].status = 0;
			}
		}
	}
	else
	{

		if (dice.previous_move != 1 && destination == source - dice.first)
		{
			dice.previous_move = 1;
		}
		else if (dice.previous_move != 2 && destination == source - dice.second)
		{
			dice.previous_move = 2;
		}
		else if (can_bear_off(source))
		{
			if (destination != -1)
				return 0;
		}
		else if (destination != source - dice.first && destination != source - dice.second)
		{
			return 0;
		}

		board[source].piece_count--;
		if (board[source].piece_count == 0)
			board[source].resident = 0;
		if (destination == -1)
		{
			board[source].pieces[board[source].piece_count ].type = 0;
			board[source].pieces[board[source].piece_count ].number = 0;
			p2.remaining_pieces--;
			if (p2.remaining_pieces == 0)
				return -1;
		}
		if (board[destination].resident != 1)
		{
			board[destination].pieces[board[destination].piece_count].type = board[source].pieces[board[source].piece_count].type;
			board[destination].pieces[board[destination].piece_count].number = 2;
			board[destination].resident = 2;
			board[destination].piece_count++;
		}
		else if (board[destination].piece_count == 1)
		{
			char t = board[destination].pieces[0].type;
			board[destination].pieces[0].type = board[source].pieces[board[source].piece_count].type;
			board[destination].pieces[0].number = 2;
			board[destination].resident = 2;
			if (p1.coins == 0)
			{
				p1.prisoner_pieces[p1.prisoner_count].type = t;
				p1.prisoner_pieces[p1.prisoner_count].number = 1;
				p1.prisoner_count++;
			}
			else
			{
				int i;
				for (i = 0; i <=5 ; i++) // find the first empty room in the opponent's castle
					if (board[i].piece_count == 0)
						break;
				if (i <= 5) // if the opponent's castle has an empty room
				{
					char answer;
					printf("Player1, do you want to use your coins? ( number of your coins: %d )\n", p1.coins);
					printf("1. No 2. Yes\n");
					while ((answer = getch()) != '1' && answer != '2')
						printf("Wrong answer,try again!\n");
					if (answer == '2')
					{
						p1.coins--;
						board[i].pieces[0].type = t;
						board[i].pieces[0].number = 1;
						board[i].resident = 1;
						board[i].piece_count++;
					}
				}
			}
		}
		else
			return 0;

		board[source].pieces[board[source].piece_count].type = 0;
		board[source].pieces[board[source].piece_count].number = 0;
		if (destination != -1)
		{
			if (board[destination].status == 1)
			{
				p2.hole_turns = 2;
				printf("Player%d, Hole Detected Here\n", turn);
			}
			else if (board[destination].status == 2)
			{
				p2.coins++;
				board[destination].status = 0;
				printf("Player%d,1 Coin Collected\n", turn);
			}
		}
	}
	return 1;
}

int has_available_move()
{
	int i;
	for (i = 0; i <= 23; i++)
	{
		if (turn == 1)
		{
			if (p1.prisoner_count > 0)
			{
				if (board[dice.first - 1].piece_count == 5 && board[dice.second - 1].piece_count == 5)
					return 0;
				if (board[dice.first - 1].piece_count <= 1)
					return 1;
				if (board[dice.second - 1].piece_count <= 1)
					return 1;
			}
			if (board[i].resident == 1)
			{
				if (i + dice.first <= 23 && board[i + dice.first].piece_count == 5 && board[i + dice.second].piece_count == 5)
					return 0;
				if (i + dice.first == 24 || can_bear_off(i) || (i + dice.first <= 23 && (board[i + dice.first].piece_count <= 1 || board[i + dice.first].resident == 1)))
					return 1;
				if (i + dice.second == 24 || can_bear_off(i) || (i + dice.second <= 23 && ( board[i + dice.second].piece_count <= 1 || board[i + dice.second].resident == 1)))
					return 1;
			}
		}
		else
		{
			if (p2.prisoner_count > 0)
			{
				if (board[24 - dice.first].piece_count == 5 && board[24 - dice.first].piece_count == 5)
					return 0;
				if (board[24 - dice.first].piece_count <= 1)
					return 1;
				if (board[24 - dice.first].piece_count <= 1)
					return 1;
			}
			if (board[i].resident == 1)
			{
				if (i - dice.first >= 0 && board[i - dice.first].piece_count == 5 && board[i - dice.second].piece_count == 5)
					return 0;
				if (i - dice.first == -1 || can_bear_off(i) || (i - dice.first >= 0 && (board[i - dice.first].piece_count <= 1 || board[i - dice.first].resident == 2)))
					return 1;
				if (i - dice.second == -1 || can_bear_off(i) || (i - dice.second >= 0 && (board[i - dice.second].piece_count <= 1 || board[i - dice.second].resident == 2)))
					return 1;
			}
		}
	}
	return 0;
}

int main()
{
	int i,finished = 0, index_source, index_destination, coin_count, hole_count, move_result;
	char source[10], destination[10];
	srand(time(NULL));
	hole_count = rand() % 3 + 1;
	coin_count = rand() % 3 + 1;
	for (i = 0; i < hole_count; i++)
	{
		int x = rand() % 24;
		while ( board[x].type != 'R' || board[x].piece_count > 0 || board[x].status > 0 )
			x = rand() % 24;
		board[x].status = 1;
	}
	for (i = 0; i < coin_count; i++)
	{
		int x = rand() % 24;
		while (board[x].piece_count > 0 || board[x].status > 0)
			x = rand() % 24;
		board[x].status = 2;
	}

	while (!finished)
	{
		system(CLEAR_SCREEN);
		display();
		printf("Player %d, please press 'd' to throw dices...",turn);
		if (toupper( getch() ) == 'D')
		{ 
			int move_number = 1;
			dice.first = rand() % 6 + 1;
			dice.second = rand() % 6 + 1; 
			dice.previous_move = 0;
			if (dice.first == dice.second)
				dice.is_double = 1;
			else
				dice.is_double = 0;
			while ((!dice.is_double && move_number <= 2) || (dice.is_double && move_number <= 4))
			{
				move_result = 0;
				printf("Dices: %d,%d\n", dice.first, dice.second);
				if (!has_available_move())
				{
					printf("No move can be made!\n");
					system("pause");
					break;
				}
				printf("Player%d, please enter your ",turn);
				if (move_number == 1)
					printf("first");
				else if (move_number == 2)
					printf("second");
				else if (move_number == 3)
					printf("third");
				else
					printf("forth");
				printf(" source and destination:\n");
				while (!move_result)
				{
					scanf("%s%s", source, destination);
					if ((turn == 1 && p1.prisoner_count > 0) || (turn == 2 && p2.prisoner_count > 0))
					{
						if (source[0] != 'O' || source[1] != '_' || !is_valid_piece(source[2]))
							index_source = -2;
						else
							index_source = 24 + source[2];// 24 + source[2]: marks a move out of prison and also encodes the selected piece
					}										// selected_piece = index_source - 24;
					else
						index_source = string_to_room(source);
					index_destination = string_to_room(destination);
					while (index_source == -2 || index_destination == -2)
					{
						printf("Wrong mover Player%d, please enter again:\n", turn);
						scanf("%s%s", source, destination);
						index_source = string_to_room(source);
						index_destination = string_to_room(destination);
					} // repeat until the user enters a valid input
					move_result = validate_move(index_source, index_destination);
					if (!move_result)
						printf("Wrong mover Player%d, please enter again:\n", turn);

				}
				if (move_result == 1)
				{
					move_number++;
					printf("Nice move Player%d",turn);
					if ((!dice.is_double && move_number > 2) || (dice.is_double && move_number > 4))
						printf(",end of your turn.\n", turn);
					getch();
					system(CLEAR_SCREEN);
					display();
				}
				else if (move_result == -1)
				{
					printf("Player%d Won.", turn);
					finished = 1;
					break;
				}
			}
			if (turn == 1)
			{
				if (p2.hole_turns > 0)
				{
					p2.hole_turns--;
					printf("Player2 is stucked in the hole. Player1 should move again...\n");
					system("pause");
				}
				else
					turn = 2;
			}
			else
			{
				if (p1.hole_turns > 0)
				{
					p1.hole_turns--;
					printf("Player1 is stucked in the hole. Player2 should move again...\n");
					system("pause");
				}
				else
					turn = 1;
			}
		}
	}
	system("pause");
}
