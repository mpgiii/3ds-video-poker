#include <stdbool.h>
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

// global variables for state management
struct Card deck[DECK_SIZE];
int remainingCardsCount = DECK_SIZE;
struct Card hand[HAND_SIZE];
int selectedCardIndex = 0;
bool held[HAND_SIZE] = {false, false, false, false, false};
enum GameState { NEW_HAND, REDRAW, GAME_OVER } gameState = NEW_HAND;
// end state management variables

const char* suit_names[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
const char* rank_names[] = {
	"2", "3", "4", "5", "6", "7", "8", "9", "10",
	"Jack", "Queen", "King", "Ace"
};

void shuffleDeck() {
	int i, j;
	struct Card temp;
	for (i = DECK_SIZE - 1; i > 0; i--) {
		j = rand() % (i + 1);
		temp = deck[i];
		deck[i] = deck[j];
		deck[j] = temp;
	}
}

void initializeRemainingDeck() {
	int i = 0;
	int s, r;
    for (s = 0; s < NUM_SUITS; s++) {
        for (r = 0; r < NUM_RANKS; r++) {
            deck[i].rank = (enum Rank) r;
            deck[i].suit = (enum Suit) s;
            i++;
        }
    }
    shuffleDeck(deck); // Shuffle deck to start fresh
    remainingCardsCount = DECK_SIZE; // Reset remaining card count
}

void printCard(struct Card c) {
	printf("%s of %s", rank_names[c.rank], suit_names[c.suit]);
}

void printHand() {
	int i;
	int r, c;
	for (i = 0; i < HAND_SIZE; i++) {
		r = 5 + i;
		c = 10;
		
		// move cursor
		printf("\x1b[%d;%dH", r, c);
		
		if (i == selectedCardIndex) {
			printf("-> ");  // highlight current selection
		} else {
			printf("   ");  // spacing to line up arrows
		}
		
		printCard(hand[i]);
		
		if (held[i]) {
			printf(" \x1b[31m[HELD]\x1b[0m");
		}
	}
}

void drawNewCards() {
	int i, cardIndex;
	for (i = 0; i < HAND_SIZE; i++) {
        if (!held[i]) {
            if (remainingCardsCount > 0) {
                // Draw a card from the remaining deck
                cardIndex = rand() % remainingCardsCount;
                hand[i] = deck[cardIndex];
                // Move the last card to the drawn spot to avoid empty spots
                deck[cardIndex] = deck[remainingCardsCount - 1];
                remainingCardsCount--;
            }
        }
    }
}

void resetHolds() {
	int i;
	for (i = 0; i < HAND_SIZE; i++) {
		held[i] = false;
	}
}

int main(int argc, char* argv[]) {
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	printf("Welcome to video poker, by mpgiii.\n\n");

	// Main loop
	while (aptMainLoop()) {
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// retrieve user input
		u32 kDown = hidKeysDown();
		
	
		// handle user input
		if (kDown & KEY_START) {
			// exit the app
			break;
		}
		if (kDown & KEY_A) {
			consoleClear();
			
			if (gameState == NEW_HAND) {
				// generate a new hand
				printf("Generating hand...\n");
                initializeRemainingDeck(deck);
                drawNewCards(deck);
                resetHolds();
                selectedCardIndex = 0;
                gameState = REDRAW;
			} else if (gameState == REDRAW) {
				// redraw cards that were not held in round one
				printf("Redrawing cards...\n");
				drawNewCards(deck);
				gameState = GAME_OVER;
			} else if (gameState == GAME_OVER) {
				// reset for a new game
				printf("GAME OVER\n");
				printf("Press A to start a new game.");
				gameState = NEW_HAND;
			}

			printHand();
		}
		if (kDown & KEY_DUP) {
			if (selectedCardIndex > 0) selectedCardIndex--;
			consoleClear();
			printHand();
		}

		if (kDown & KEY_DDOWN) {
			if (selectedCardIndex < HAND_SIZE - 1) selectedCardIndex++;
			consoleClear();
			printHand();
		}

		if (kDown & KEY_X) {
			held[selectedCardIndex] = !held[selectedCardIndex];
			consoleClear();
			printHand();
		}
		
		if (kDown & KEY_B) {
			drawNewCards();
			consoleClear();
			printHand();
		}

		// print static instructions at the bottom of the screen
		printf("\x1b[27;1H");
		printf("Press A to deal a new hand");
		printf("\x1b[28;1H");
		printf("Press X to toggle hold");
		printf("\x1b[29;1H");
		printf("Use UP/DOWN to select card");
		printf("\x1b[30;1H");
		printf("Press START to exit");
	}

	gfxExit();
	return 0;
}
