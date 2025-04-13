#ifndef POKER_CARDS_H_
#define POKER_CARDS_H_

#include <stdbool.h>

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

extern const char* suit_names[];
extern const char* rank_names[];

const char* evaluateHand(struct Card hand[]);
void printCard(struct Card c);
void printHand(struct Card hand[], int selectedCardIndex, bool held[], bool cursor);

#endif // POKER_CARDS_H_