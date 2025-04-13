#include <stdio.h>
#include "cards.h"

const char* suit_names[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
const char* rank_names[] = {
    "2", "3", "4", "5", "6", "7", "8", "9", "10",
    "Jack", "Queen", "King", "Ace"
};

void countRanks(struct Card hand[], int rankCounts[NUM_RANKS]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        rankCounts[hand[i].rank]++;
    }
}

void countSuits(struct Card hand[], int suitCounts[NUM_SUITS]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        suitCounts[hand[i].suit]++;
    }
}

bool isFlush(struct Card hand[]) {
    int suitCounts[NUM_SUITS] = {0};
    countSuits(hand, suitCounts);
    for (int i = 0; i < NUM_SUITS; i++) {
        if (suitCounts[i] == HAND_SIZE) {
            return true;
        }
    }
    return false;
}

bool isStraight(struct Card hand[]) {
    int rankCounts[NUM_RANKS] = {0};
    countRanks(hand, rankCounts);

    int consecutive = 0;
    for (int i = 0; i < NUM_RANKS; i++) {
        if (rankCounts[i] > 0) {
            consecutive++;
            if (consecutive == HAND_SIZE) {
                return true;
            }
        } else {
            consecutive = 0;
        }
    }
    return false;
}

bool isFullHouse(struct Card hand[]) {
    int rankCounts[NUM_RANKS] = {0};
    countRanks(hand, rankCounts);

    bool hasThree = false, hasTwo = false;
    for (int i = 0; i < NUM_RANKS; i++) {
        if (rankCounts[i] == 3) hasThree = true;
        if (rankCounts[i] == 2) hasTwo = true;
    }
    return hasThree && hasTwo;
}

bool isFourOfAKind(struct Card hand[]) {
    int rankCounts[NUM_RANKS] = {0};
    countRanks(hand, rankCounts);

    for (int i = 0; i < NUM_RANKS; i++) {
        if (rankCounts[i] == 4) {
            return true;
        }
    }
    return false;
}

bool isPair(struct Card hand[]) {
    int rankCounts[NUM_RANKS] = {0};
    countRanks(hand, rankCounts);

    for (int i = 0; i < NUM_RANKS; i++) {
        if (rankCounts[i] == 2) {
            return true;
        }
    }
    return false;
}

bool isThreeOfAKind(struct Card hand[]) {
    int rankCounts[NUM_RANKS] = {0};
    countRanks(hand, rankCounts);

    for (int i = 0; i < NUM_RANKS; i++) {
        if (rankCounts[i] == 3) {
            return true;
        }
    }
    return false;
}

bool isTwoPair(struct Card hand[]) {
    int rankCounts[NUM_RANKS] = {0};
    countRanks(hand, rankCounts);

    int pairCount = 0;
    for (int i = 0; i < NUM_RANKS; i++) {
        if (rankCounts[i] == 2) {
            pairCount++;
        }
    }
    return pairCount == 2;
}

const char* evaluateHand(struct Card hand[]) {
    if (isFlush(hand) && isStraight(hand)) {
        if (hand[0].rank == TEN) {
            return "Royal Flush";
        }
        return "Straight Flush";
    }
    if (isFourOfAKind(hand)) return "Four of a Kind";
    if (isFullHouse(hand)) return "Full House";
    if (isFlush(hand)) return "Flush";
    if (isStraight(hand)) return "Straight";
    if (isThreeOfAKind(hand)) return "Three of a Kind";
    if (isTwoPair(hand)) return "Two Pair";
    if (isPair(hand)) return "One Pair";
    return "High Card";
}

void printCard(struct Card c) {
    printf("%7s of %s", rank_names[c.rank], suit_names[c.suit]);
}

void printHand(struct Card hand[], int selectedCardIndex, bool held[], bool cursor) {
    int i, r, c;

    // Print the cards
    for (i = 0; i < HAND_SIZE; i++) {
        r = 5 + i;
        c = 10;

        // move cursor
        printf("\x1b[%d;%dH", r, c);

        if (cursor && i == selectedCardIndex) {
            printf("-> ");  // highlight current selection
        } else {
            printf("   ");  // spacing to line up arrows
        }

        printCard(hand[i]);

        if (held[i]) {
            printf(" \x1b[31m[HELD]\x1b[0m");
        }
    }
    
    // Print the hand type
    const char* handType = evaluateHand(hand);
    printf("\x1b[%d;%dHCurrent hand type: %s", r+2, c, handType);
}