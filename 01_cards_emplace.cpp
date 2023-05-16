#include <iostream>
#include <vector>
#include <iomanip>
#include <memory>
#include <random>
#include <algorithm>
#include <stdexcept>

using namespace std;

enum class Suits { HEARTS, SPADES, CLUBS, DIAMONDS, SUITS_COUNT };
enum class Ranks { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, RANKS_COUNT };

ostream& operator<<(ostream& os, const Suits& suit)
{
    switch (suit) {
        case Suits::HEARTS: os << "Hearts"; break;
        case Suits::SPADES: os << "Spades"; break;
        case Suits::CLUBS: os << "Clubs"; break;
        case Suits::DIAMONDS: os << "Diamonds"; break;
        default: os << "Unknown suit";
    }
    return os;
}

ostream& operator<< (ostream& os, const Ranks& rank)
{
    switch (rank)
    {
    case Ranks::ACE: os << "Ace"; break;
    case Ranks::JACK: os << "Jack"; break;
    case Ranks::QUEEN: os << "Queen"; break;
    case Ranks::KING: os << "King"; break;
    default:
        if (rank >= Ranks::TWO && rank <= Ranks::TEN)
            os << static_cast<int>(rank) + 1;
        else
            os << "unknown rank";
    }
    return os;
}

class Card 
{
    Suits suit;
    Ranks rank;

public:
    Card(Suits suit, Ranks rank) : suit(suit), rank(rank) 
    {
        // cout << "Constructing " << *this << endl;
    }

    Card(const Card& other) : suit(other.suit), rank(other.rank)
    {
        // cout << "Copy constructing " << *this << endl;
    }
    
    Card(Card&& other) noexcept : suit(other.suit), rank(other.rank)
    {
        // cout << "Move constructing " << *this << endl;
    }
    
    Card& operator=(Card&& other) noexcept 
    {
        this->suit = other.suit;
        this->rank = other.rank;
        
        return *this;
    }
    
    Suits Suit() const { return suit; }
    Ranks Rank() const { return rank; }

    friend ostream& operator<<(ostream& os, const Card& card) 
    {
        os << "Card(" << card.Rank() << ", " << card.Suit() << ")";
        return os;
    }
};

int main()
{
    for (int i = 0; i < 100000; ++i)
    {
        vector<Card> deck;
        for (size_t rank = 0; rank < static_cast<size_t>(Ranks::RANKS_COUNT); ++rank)
        {
            for (size_t suit = 0; suit < static_cast<size_t>(Suits::SUITS_COUNT); ++suit)
            {
                deck.emplace_back(Card(static_cast<Suits>(suit), static_cast<Ranks>(rank)));
            }
        }
    }

    return 0;
}
