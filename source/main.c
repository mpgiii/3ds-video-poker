#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <3ds.h>
#include "cards.h"

// Global state variables
struct Card deck[DECK_SIZE];
int remainingCardsCount = DECK_SIZE;
struct Card hand[HAND_SIZE];
int selectedCardIndex = 0;
bool held[HAND_SIZE] = {false, false, false, false, false};
enum GameState { NEW_HAND, REDRAW, GAME_OVER } gameState = GAME_OVER;

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

void resetHolds() {
    int i;
    for (i = 0; i < HAND_SIZE; i++) {
        held[i] = false;
    }
}

void drawNewCards() {
    int i, cardIndex;
    if (remainingCardsCount <= 0) {
        printf("No more cards to draw!\n");
        return;
    }
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

int main(int argc, char* argv[]) {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    srand(time(NULL));
    
    printf("Welcome to 3DS Video Poker, by mpgiii.\n");

    while (aptMainLoop()) {
        gspWaitForVBlank();
        gfxSwapBuffers();
        hidScanInput();
        
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break;
        
        switch (gameState) {
            case NEW_HAND:
                consoleClear();
                printf("Generating hand...\n");
                initializeRemainingDeck();
                drawNewCards();
                resetHolds();
                selectedCardIndex = 0;
                gameState = REDRAW;
                printHand(hand, selectedCardIndex, held, true);
                break;
            case REDRAW:
                if (kDown & KEY_A) {
                    consoleClear();
                    printf("Redrawing cards...\n");
                    drawNewCards();
                    gameState = GAME_OVER;
                    printf("\x1b[1;1HGAME OVER\n");
                    printf("Press A to reset.");
                    printHand(hand, selectedCardIndex, held, false);
                }
                if (kDown & KEY_DUP) {
                    if (selectedCardIndex > 0) selectedCardIndex--;
                    consoleClear();
                    printHand(hand, selectedCardIndex, held, true);
                }
                if (kDown & KEY_DDOWN) {
                    if (selectedCardIndex < HAND_SIZE - 1) selectedCardIndex++;
                    consoleClear();
                    printHand(hand, selectedCardIndex, held, true);
                }
                if (kDown & KEY_X) {
                    held[selectedCardIndex] = !held[selectedCardIndex];
                    consoleClear();
                    printHand(hand, selectedCardIndex, held, true);
                }
                break;
            case GAME_OVER:
                if (kDown & KEY_A) {
                    gameState = NEW_HAND;
                }
                break;
        }
        
        // Static instructions
        printf("\x1b[27;1HPress A to deal a new hand/redraw");
        printf("\x1b[28;1HPress X to toggle hold");
        printf("\x1b[29;1HUse UP/DOWN to select card");
        printf("\x1b[30;1HPress START to exit");
    }

    gfxExit();
    return 0;
}