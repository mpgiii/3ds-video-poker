#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <3ds.h>

#define DECK_SIZE 52
#define HAND_SIZE 5

enum Suit {
	HEARTS,
	DIAMONDS,
	CLUBS,
	SPADES,
	NUM_SUITS
};

enum Rank {
	TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
	JACK, QUEEN, KING, ACE,
	NUM_RANKS
};

struct Card {
	enum Rank rank;
	enum Suit suit;
};

struct Card hand[HAND_SIZE];  // global variable for state management

const char* suit_names[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
const char* rank_names[] = {
	"2", "3", "4", "5", "6", "7", "8", "9", "10",
	"Jack", "Queen", "King", "Ace"
};

void initializeDeck(struct Card deck[]) {
	int i = 0;
	int s, r;
	for (s = 0; s < NUM_SUITS; s++) {
		for (r = 0; r < NUM_RANKS; r++) {
			deck[i].rank = (enum Rank) r;
			deck[i].suit = (enum Suit) s;
			i++;
		}
	}
}

void shuffleDeck(struct Card deck[]) {
	int i, j;
	struct Card temp;
	for (i = DECK_SIZE - 1; i > 0; i--) {
		j = rand() % (i + 1);
		temp = deck[i];
		deck[i] = deck[j];
		deck[j] = temp;
	}
}

void printCard(struct Card c) {
	printf("%s of %s", rank_names[c.rank], suit_names[c.suit]);
}

void printHand() {
	int i;
	for (i = 0; i < HAND_SIZE; i++) {
		printCard(hand[i]);
		printf("\n");
	}
}


void generatePokerHand()
{
	int i;

	struct Card deck[DECK_SIZE];
	srand(time(NULL));

	initializeDeck(deck);
	shuffleDeck(deck);

	// Draw a hand of cards and copy into the global hand array
	for (i = 0; i < HAND_SIZE; i++) {
		hand[i] = deck[i];
	}
}

int main(int argc, char* argv[])
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	printf("Welcome to video poker, by mpgiii.\n\n");
	printf("Press 'A' to generate a new hand.\n");
	printf("Press 'START' to exit.");

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// Your code goes here
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		else if (kDown & KEY_A)
		{
			consoleClear();
			printf("Generating hand...\n");
			generatePokerHand();
			printHand();
			printf("\n\n");
			printf("Press 'A' to generate a new hand.\n");
			printf("Press 'START' to exit.");

		}
	}

	gfxExit();
	return 0;
}
