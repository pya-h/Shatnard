#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
// #include <conio.h>
#include "include/console.h";
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

struct TAS
{
	int aval;
	int dovom;
	int joft; // moshakhas mikonad aya a'daade tas ha mosavi hastand ya na
	int harekat_ghabli; // moshakhas mikonad harekat ghabli bazikon che bode, 0 bodan An be ma'niye in ast ke bazikon hanooz harakati nakarde
} tas ;

const struct MOHRE
{
	char noe;
	int shomare;
} khali = { 0, 0 };

struct BAZIKON
{
	int mohreha;// tedad mohrehayi ke hanooz be O naraftand
	int sekke; // tedad sekkehaye bazikon
	int chah; // tedad nobathaye asiri dar chah
	int tedad_zendani; // tedade mohre haye zendani
	struct MOHRE mohre_zendani[15];
}p1 = { 15, 0, 0 , 0, { 0 } }, p2 = { 15, 0, 0 , 0, { 0 } };

struct KHANE
{
	char noe; // Road ya Castle bodan		
	int vaziyat;// 2: Khaneye sekke daar	1:chah daar		0:khali
	int tedad_mohre;// tedade mohrehaye haazer dar in khane
	int mohre_saken; //moshakhas mikonad mohrehaye kodam bazikon sakene in khane hastand
	struct MOHRE mohre[5];//mohrehaye mojod dar har khane
} khane[24] = {
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
}; // tayine makan avaliye ye mohreha		0 ta 5: player2's Castle		6 ta 11: Road Number 2		12 ta 17: Road Number 1		18 ta 23: Player1's Castle

int nobat = 1;

int ham_arz(struct MOHRE a1, struct MOHRE a2)// barrassi ham arziye 2 mohre mokhtalef
{
	if (a1.noe == a2.noe && a2.shomare == a2.shomare)
		return 1;
	return 0;
}

void mohre2string(struct MOHRE m, char *str_mohre)
{
 
	str_mohre[0] = m.noe;
	str_mohre[1] = '_';
	str_mohre[2] = (char)m.shomare + '0';// "shomare" yek raghame sahih mibashad ke be char tabdil mishavad
	if (ham_arz(m, khali))
		str_mohre[0] = str_mohre[1] = str_mohre[2] = ' ';
}

struct MOHRE string2mohre(char s[])
{
	struct MOHRE m = { toupper(s[0]), s[2] - '0' };
	return m;
}

void khane2string(int index,char *str_khane)
{
	if (index >= 6 && index <= 17)//baa tavajjoh be tartib chineshe khane haa dar araye
		str_khane[0] = 'R';
	else
		str_khane[0] = 'C';

	if (index <= 11)
	{
		str_khane[1] = '1';
		str_khane[3] = (index % 6) + 1 + '0'; //baa tavajjoh be tartib chineshe khane haa dar araye
	}
	else
	{
		int t = ( 23 - index ) % 6 + 1;
		str_khane[1] = '2';
		str_khane[3] = t + '0'; // tabdile raghame sahih be raghame char
	}
	str_khane[2] = '_';
	str_khane[4] = '\0';
}

int string2khane(char *str_khane)
{
	char bakhsh = toupper( str_khane[0] ) ;
	int shomare_zamin = str_khane[1] - '0', shomare_khane = str_khane[3] - '0';
	str_khane[4] = '\0';
	if (str_khane[2] != '_'){
		return -2; //barrasi inke ayya formate vorodi dorost boode ya na
	}
	if (bakhsh == 'O')
	{
		if (nobat == 1)
			return 24;
		else
			return -1;
	}
	if (bakhsh == 'C')
	{
		if (shomare_khane >= 1 && shomare_khane <= 6)
		{
			if (shomare_zamin == 2)
				return shomare_khane - 1;
			if (shomare_zamin == 1)
				return 24 - shomare_khane;
		}
	}

	if (bakhsh == 'R')
	{
		if (shomare_khane >= 1 && shomare_khane <= 6)
		{
			if (shomare_zamin == 2)
				return 5 + shomare_khane;
			if (shomare_zamin == 1)
				return 12 + (6 - shomare_khane);
		}
	}
	return -2;
}

void namayesh()
{
	int i, j;
	char str_mohre[4] = { 0 };
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
			mohre2string(khane[i].mohre[j],str_mohre);
			printf("%s", str_mohre);
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
			mohre2string(khane[i].mohre[j], str_mohre);
			printf("%s", str_mohre);
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

int mohre_check(char m)
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

int vaziyate_payani(int source)
{
	int i;
	if (nobat == 1)//agar hameye mohre haye yek bazikon dar castle bashand , pas nabayad mohreyi az in bazikon dar jahaye digar yaft shavad!
	{
		if (source < 18)
			return 0;
		for (i = source - 1; i >= 0; i--)
			if (khane[i].mohre_saken == 1)
				return 0;
	}
	else
	{
		if (source > 5)
			return 0;
		for (i = source + 1; i <= 23; i++)
			if (khane[i].mohre_saken == 2)
				return 0;
	}
	if (tas.harekat_ghabli != 1 && tas.aval > source)
	{
		tas.harekat_ghabli = 1;
		return 1;
	}
	else if (tas.harekat_ghabli != 2 && tas.dovom > source)
	{
		tas.harekat_ghabli = 2;
		return 1;
	}
	return 0;
}

int barrasi_harekat(int source, int destination) // 0:harekate eshtebah		1:harekat dorost	-1:pirozi
{
	int i,j;
	//barrasiye halate azad kardane mohreye zendani
	if (source == 24)
	{

		if (khane[destination].tedad_mohre == 5)
		{
			return 0;
		}
		if (nobat == 1)
		{

			if (destination != tas.aval-1 && destination != tas.dovom-1)
				return 0;
			if (tas.harekat_ghabli != 1 && destination == tas.aval-1)
			{
				tas.harekat_ghabli = 1;
			}
			else if (tas.harekat_ghabli != 2 && destination == tas.dovom-1)
			{
				tas.harekat_ghabli = 2;
			}
			else
				return 0;
			if (khane[destination].mohre_saken != 2)
			{
				khane[destination].mohre[khane[destination].tedad_mohre].noe = (char) (source - 24); // be tozihe in formol dar main() deghat shavad
				khane[destination].mohre[khane[destination].tedad_mohre].shomare = 1;
				khane[destination].mohre_saken = 1;
				khane[destination].tedad_mohre++;
				for (i = 0; i < p1.tedad_zendani; i++)
				{
					if (p1.mohre_zendani[i].noe == (char)(source - 24))
					{
						for (j = 0; i + j < p1.tedad_zendani; j++)
						{
							p1.mohre_zendani[i + j].noe = p1.mohre_zendani[i + j + 1].noe;
							p1.mohre_zendani[i + j].shomare = p1.mohre_zendani[i + j + 1].shomare;
						}
					}
				}
			}
			else if (khane[destination].tedad_mohre == 1)
			{
				char t = khane[destination].mohre[0].noe;
				khane[destination].mohre[0].noe = (char)(source - 24);
				khane[destination].mohre[0].shomare = 1;
				khane[destination].mohre_saken = 1;
				for (i = 0; i < p1.tedad_zendani; i++)
				{
					if (p1.mohre_zendani[i].noe == (char)(source - 24))
					{
						for (j = 0; i + j < p1.tedad_zendani; j++)
						{
							p1.mohre_zendani[i + j].noe = p1.mohre_zendani[i + j + 1].noe;
							p1.mohre_zendani[i + j].shomare = p1.mohre_zendani[i + j + 1].shomare;
						}
					}
				}
				if (p2.sekke == 0)
				{
					p2.mohre_zendani[p2.tedad_zendani].noe = t;
					p2.mohre_zendani[p2.tedad_zendani].shomare = 2;
					p2.tedad_zendani++;
				}
				else
				{
					int i;
					for (i = 23; i >= 18; i--) //donbale avalin khaneye khali dar ghal'eye harif migardim
						if (khane[i].tedad_mohre == 0)
							break;
					if (i >= 18) //agar dar castle harif khaneye khali vojod dashte bashad
					{
						char javab;
						printf("\nPlayer2, do you want to use your coins? ( number of your coins: %d )\n", p2.sekke);
						printf("1. No 2. Yes\n");
						while ((javab = getch()) != '1' && javab != '2')
							printf("Wrong answer,try again!\n");
						if (javab == '2')
						{
							p2.sekke--;
							khane[i].mohre[0].noe = t;
							khane[i].mohre[0].shomare = 2;
							khane[i].mohre_saken = 2;
							khane[i].tedad_mohre++;
						}
					}
				}
			}
		}
		else
		{
			if (destination != tas.aval - 1 && 24 - destination != tas.dovom)
				return 0;
			if (tas.harekat_ghabli != 1 && 24 - destination == tas.aval - 1)
			{
				tas.harekat_ghabli = 1;
			}
			else if (tas.harekat_ghabli != 2 && 24 - destination == tas.dovom - 1)
			{
				tas.harekat_ghabli = 2;
			}
			else
				return 0;
			if (khane[destination].mohre_saken != 2)
			{
				khane[destination].mohre[khane[destination].tedad_mohre].noe = (char)(source - 24); // be tozihe in formol dar main() deghat shavad
				khane[destination].mohre[khane[destination].tedad_mohre].shomare = 1;
				khane[destination].mohre_saken = 1;
				khane[destination].tedad_mohre++;
				for (i = 0; i < p2.tedad_zendani; i++)
				{
					if (p2.mohre_zendani[i].noe == (char)(source - 24))
					{
						for (j = 0; i + j < p2.tedad_zendani; j++)
						{
							p2.mohre_zendani[i + j].noe = p2.mohre_zendani[i + j + 1].noe;
							p2.mohre_zendani[i + j].shomare = p2.mohre_zendani[i + j + 1].shomare;
						}
					}
				}
			}
			else if (khane[destination].tedad_mohre == 1)
			{
				char t = khane[destination].mohre[0].noe;
				khane[destination].mohre[0].noe = (char)(source - 24);;
				khane[destination].mohre[0].shomare = 1;
				khane[destination].mohre_saken = 1;
				for (i = 0; i < p2.tedad_zendani; i++)
				{
					if (p2.mohre_zendani[i].noe == (char)(source - 24))
					{
						for (j = 0; i + j < p2.tedad_zendani; j++)
						{
							p2.mohre_zendani[i + j].noe = p2.mohre_zendani[i + j + 1].noe;
							p2.mohre_zendani[i + j].shomare = p2.mohre_zendani[i + j + 1].shomare;
						}
					}
				}
				if (p2.sekke == 0)
				{
					p2.mohre_zendani[p2.tedad_zendani].noe = t;
					p2.mohre_zendani[p2.tedad_zendani].shomare = 2;
					p2.tedad_zendani++;
				}
				else
				{
					int i;
					for (i = 23; i >= 18; i--) //donbale avalin khaneye khali dar ghal'eye harif migardim
						if (khane[i].tedad_mohre == 0)
							break;
					if (i >= 18) //agar dar castle harif khaneye khali vojod dashte bashad
					{
						char javab;
						printf("\nPlayer2, do you want to use your coins? ( number of your coins: %d )\n", p2.sekke);
						printf("1. No 2. Yes\n");
						while ((javab = getch()) != '1' && javab != '2')
							printf("Wrong answer,try again!\n");
						if (javab == '2')
						{
							p2.sekke--;
							khane[i].mohre[0].noe = t;
							khane[i].mohre[0].shomare = 2;
							khane[i].mohre_saken = 2;
							khane[i].tedad_mohre++;
						}
					}
				}
			}
		}
	}


	//barrasi harekat haye ma'moli
	if (khane[source].tedad_mohre == 0 || (destination != 24 && khane[destination].tedad_mohre == 5))
		return 0;
	if (khane[source].mohre_saken != nobat)
		return 0;


	if (nobat == 1)
	{

		if (tas.harekat_ghabli != 1 && destination == source + tas.aval)
		{
			tas.harekat_ghabli = 1;
		}
		else if (tas.harekat_ghabli != 2 && destination == source + tas.dovom)
		{
			tas.harekat_ghabli = 2;
		}
		else if (vaziyate_payani(source))
		{
			if (destination != 24)
				return 0;

		}
		else if (destination != source + tas.aval && destination != source + tas.dovom)
		{
			return 0;
		}

		khane[source].tedad_mohre--;
		if (khane[source].tedad_mohre == 0)
			khane[source].mohre_saken = 0;
		if (destination == 24)
		{
			p1.mohreha--;
			if (p1.mohreha == 0)
				return -1;
		}
		else if (khane[destination].mohre_saken != 2)
		{
			khane[destination].mohre[ khane[destination].tedad_mohre ].noe = khane[source].mohre[ khane[source].tedad_mohre ].noe;
			khane[destination].mohre[khane[destination].tedad_mohre].shomare = 1;
			khane[destination].mohre_saken = 1;
			khane[destination].tedad_mohre++;
		}
		else if (khane[destination].tedad_mohre == 1)
		{
			char t = khane[destination].mohre[0].noe;
			khane[destination].mohre[0].noe = khane[source].mohre[khane[source].tedad_mohre].noe;
			khane[destination].mohre[0].shomare = 1;
			khane[destination].mohre_saken = 1;
			if (p2.sekke == 0)
			{
				p2.mohre_zendani[p2.tedad_zendani].noe = t;
				p2.mohre_zendani[p2.tedad_zendani].shomare = 2;
				p2.tedad_zendani++;
			}
			else
			{
				int i;
				for (i = 23; i >= 18; i--) //donbale avalin khaneye khali dar ghal'eye harif migardim
					if (khane[i].tedad_mohre == 0)
						break;
				if (i >= 18) //agar dar castle harif khaneye khali vojod dashte bashad
				{
					char javab;
					printf("Player2, do you want to use your coins? ( number of your coins: %d )\n", p2.sekke);
					printf("1. No 2. Yes\n");
					while ((javab = getch()) != '1' && javab != '2')
						printf("Wrong answer,try again!\n");
					if (javab == '2')
					{
						p2.sekke--;
						khane[i].mohre[0].noe = t;
						khane[i].mohre[0].shomare = 2;
						khane[i].mohre_saken = 2;
						khane[i].tedad_mohre++;
					}
				}
			}
		}
		else
		{
			return 0;
		}
		khane[source].mohre[khane[source].tedad_mohre].noe = 0;
		khane[source].mohre[khane[source].tedad_mohre].shomare = 0;
		if (destination != 24)
		{
			if (khane[destination].vaziyat == 1)
				p1.chah = 2;
			else if (khane[destination].vaziyat == 2)
			{
				p1.sekke++;
				khane[destination].vaziyat = 0;
			}
		}
	}
	else
	{

		if (tas.harekat_ghabli != 1 && destination == source - tas.aval)
		{
			tas.harekat_ghabli = 1;
		}
		else if (tas.harekat_ghabli != 2 && destination == source - tas.dovom)
		{
			tas.harekat_ghabli = 2;
		}
		else if (vaziyate_payani(source))
		{
			if (destination != -1)
				return 0;
		}
		else if (destination != source - tas.aval && destination != source - tas.dovom)
		{
			return 0;
		}

		khane[source].tedad_mohre--;
		if (khane[source].tedad_mohre == 0)
			khane[source].mohre_saken = 0;
		if (destination == -1)
		{
			khane[source].mohre[khane[source].tedad_mohre ].noe = 0;
			khane[source].mohre[khane[source].tedad_mohre ].shomare = 0;
			p2.mohreha--;
			if (p2.mohreha == 0)
				return -1;
		}
		if (khane[destination].mohre_saken != 1)
		{
			khane[destination].mohre[khane[destination].tedad_mohre].noe = khane[source].mohre[khane[source].tedad_mohre].noe;
			khane[destination].mohre[khane[destination].tedad_mohre].shomare = 2;
			khane[destination].mohre_saken = 2;
			khane[destination].tedad_mohre++;
		}
		else if (khane[destination].tedad_mohre == 1)
		{
			char t = khane[destination].mohre[0].noe;
			khane[destination].mohre[0].noe = khane[source].mohre[khane[source].tedad_mohre].noe;
			khane[destination].mohre[0].shomare = 2;
			khane[destination].mohre_saken = 2;
			if (p1.sekke == 0)
			{
				p1.mohre_zendani[p1.tedad_zendani].noe = t;
				p1.mohre_zendani[p1.tedad_zendani].shomare = 1;
				p1.tedad_zendani++;
			}
			else
			{
				int i;
				for (i = 0; i <=5 ; i++) //donbale avalin khaneye khali dar ghal'eye harif migardim
					if (khane[i].tedad_mohre == 0)
						break;
				if (i <= 5) //agar dar castle harif khaneye khali vojod dashte bashad
				{
					char javab;
					printf("Player1, do you want to use your coins? ( number of your coins: %d )\n", p1.sekke);
					printf("1. No 2. Yes\n");
					while ((javab = getch()) != '1' && javab != '2')
						printf("Wrong answer,try again!\n");
					if (javab == '2')
					{
						p1.sekke--;
						khane[i].mohre[0].noe = t;
						khane[i].mohre[0].shomare = 1;
						khane[i].mohre_saken = 1;
						khane[i].tedad_mohre++;
					}
				}
			}
		}
		else
			return 0;

		khane[source].mohre[khane[source].tedad_mohre].noe = 0;
		khane[source].mohre[khane[source].tedad_mohre].shomare = 0;
		if (destination != -1)
		{
			if (khane[destination].vaziyat == 1)
			{
				p2.chah = 2;
				printf("Player%d, Hole Detected Here\n", nobat);
			}
			else if (khane[destination].vaziyat == 2)
			{
				p2.sekke++;
				khane[destination].vaziyat = 0;
				printf("Player%d,1 Coin Collected\n", nobat);
			}
		}
	}
	return 1;
}

int tas_ghabel_harekat()
{
	int i;
	for (i = 0; i <= 23; i++)
	{
		if (nobat == 1)
		{
			if (p1.tedad_zendani > 0)
			{
				if (khane[tas.aval - 1].tedad_mohre == 5 && khane[tas.dovom - 1].tedad_mohre == 5)
					return 0;
				if (khane[tas.aval - 1].tedad_mohre <= 1)
					return 1;
				if (khane[tas.dovom - 1].tedad_mohre <= 1)
					return 1;
			}
			if (khane[i].mohre_saken == 1)
			{
				if (i + tas.aval <= 23 && khane[i + tas.aval].tedad_mohre == 5 && khane[i + tas.dovom].tedad_mohre == 5)
					return 0;
				if (i + tas.aval == 24 || vaziyate_payani(i) || (i + tas.aval <= 23 && (khane[i + tas.aval].tedad_mohre <= 1 || khane[i + tas.aval].mohre_saken == 1)))
					return 1;
				if (i + tas.dovom == 24 || vaziyate_payani(i) || (i + tas.dovom <= 23 && ( khane[i + tas.dovom].tedad_mohre <= 1 || khane[i + tas.dovom].mohre_saken == 1)))
					return 1;
			}
		}
		else
		{
			if (p2.tedad_zendani > 0)
			{
				if (khane[24 - tas.aval].tedad_mohre == 5 && khane[24 - tas.aval].tedad_mohre == 5)
					return 0;
				if (khane[24 - tas.aval].tedad_mohre <= 1)
					return 1;
				if (khane[24 - tas.aval].tedad_mohre <= 1)
					return 1;
			}
			if (khane[i].mohre_saken == 1)
			{
				if (i - tas.aval >= 0 && khane[i - tas.aval].tedad_mohre == 5 && khane[i - tas.dovom].tedad_mohre == 5)
					return 0;
				if (i - tas.aval == -1 || vaziyate_payani(i) || (i - tas.aval >= 0 && (khane[i - tas.aval].tedad_mohre <= 1 || khane[i - tas.aval].mohre_saken == 2)))
					return 1;
				if (i - tas.dovom == -1 || vaziyate_payani(i) || (i - tas.dovom >= 0 && (khane[i - tas.dovom].tedad_mohre <= 1 || khane[i - tas.dovom].mohre_saken == 2)))
					return 1;
			}
		}
	}
	return 0;
}

int main()
{
	int i,payan = 0, index_source, index_destination, tedad_sekke, tedad_chah, barrasi;
	char source[10], destination[10];
	srand(time(NULL));
	tedad_chah = rand() % 3 + 1;
	tedad_sekke = rand() % 3 + 1;
	for (i = 0; i < tedad_chah; i++)
	{
		int x = rand() % 24;
		while ( khane[x].noe != 'R' || khane[x].tedad_mohre > 0 || khane[x].vaziyat > 0 )
			x = rand() % 24;
		khane[x].vaziyat = 1;
	}
	for (i = 0; i < tedad_sekke; i++)
	{
		int x = rand() % 24;
		while (khane[x].tedad_mohre > 0 || khane[x].vaziyat > 0)
			x = rand() % 24;
		khane[x].vaziyat = 2;
	}

	while (!payan)
	{
		system(CLEAR_SCREEN);
		namayesh();
		printf("Player %d, please press 'd' to throw dices...",nobat);
		if (toupper( getch() ) == 'D')
		{ 
			int harekat = 1;
			tas.aval = rand() % 6 + 1;
			tas.dovom = rand() % 6 + 1; 
			tas.harekat_ghabli = 0;
			if (tas.aval == tas.dovom)
				tas.joft = 1;
			else
				tas.joft = 0;
			while ((!tas.joft && harekat <= 2) || (tas.joft && harekat <= 4))
			{
				barrasi = 0;
				printf("Dices: %d,%d\n", tas.aval, tas.dovom);
				if (!tas_ghabel_harekat())
				{
					printf("No move can be made!\n");
					system("pause");
					break;
				}
				printf("Player%d, please enter your ",nobat);
				if (harekat == 1)
					printf("first");
				else if (harekat == 2)
					printf("second");
				else if (harekat == 3)
					printf("third");
				else
					printf("forth");
				printf(" source and destination:\n");
				while (!barrasi)
				{
					scanf("%s%s", source, destination);
					if ((nobat == 1 && p1.tedad_zendani > 0) || (nobat == 2 && p2.tedad_zendani > 0))
					{
						if (source[0] != 'O' || source[1] != '_' || !mohre_check(source[2]))
							index_source = -2;
						else
							index_source = 24 + source[2];//24+source[2] alave bar inke moshakhas mikonad harekat az zendan shoro mishavad , neshandahandeye mohreye entekhabi karbar niz hast
					}										// mohreye_entekhabi = index_source - 24;
					else
						index_source = string2khane(source);
					index_destination = string2khane(destination);
					while (index_source == -2 || index_destination == -2)
					{
						printf("Wrong mover Player%d, please enter again:\n", nobat);
						scanf("%s%s", source, destination);
						index_source = string2khane(source);
						index_destination = string2khane(destination);
					} //ta zamani ke karbar vorodiye sahih vared nakonad tekrar mishavad
					barrasi = barrasi_harekat(index_source, index_destination);
					if (!barrasi)
						printf("Wrong mover Player%d, please enter again:\n", nobat);

				}
				if (barrasi == 1)
				{
					harekat++;
					printf("Nice move Player%d",nobat);
					if ((!tas.joft && harekat > 2) || (tas.joft && harekat > 4))
						printf(",end of your turn.\n", nobat);
					getch();
					system(CLEAR_SCREEN);
					namayesh();
				}
				else if (barrasi == -1)
				{
					printf("Player%d Won.", nobat);
					payan = 1;
					break;
				}
			}
			if (nobat == 1)
			{
				if (p2.chah > 0)
				{
					p2.chah--;
					printf("Player2 is stucked in the hole. Player1 should move again...\n");
					system("pause");
				}
				else
					nobat = 2;
			}
			else
			{
				if (p1.chah > 0)
				{
					p1.chah--;
					printf("Player1 is stucked in the hole. Player2 should move again...\n");
					system("pause");
				}
				else
					nobat = 1;
			}
		}
	}
	system("pause");
}
// narrasi harekat haye baghi
//paak kardane debug
