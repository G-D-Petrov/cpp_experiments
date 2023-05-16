#include <iostream>
#include <vector>
#include <iomanip>
#include <memory>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <sstream>

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
    case Ranks::ACE: os << "A"; break;
    case Ranks::JACK: os << "J"; break;
    case Ranks::QUEEN: os << "Q"; break;
    case Ranks::KING: os << "K"; break;
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

    string ascii_card;
public:
    Card(Suits suit, Ranks rank) : suit(suit), rank(rank) 
    {
        // cout << "Constructing " << *this << endl;
        std::stringstream filenameStream;
        filenameStream << "cards/" << rank << "_" << suit << ".txt";
        std::string filename = filenameStream.str();
        std::ifstream file(filename);
        string line;
        while (getline(file, line)) 
        {
            ascii_card += (line); 
            ascii_card += "\n"; 
        }
    }

    Card(const Card& other) : suit(other.suit), rank(other.rank), ascii_card(other.ascii_card)
    {
        // cout << "Copy constructing " << *this << endl;
    }

    Card& operator= (const Card& other) 
    {
        // cout << "Copy assigning " << *this << endl;
        if (this == &other)
            return *this;

        this->suit = other.suit;
        this->rank = other.rank;
        this->ascii_card = string(other.ascii_card);
        
        return *this;
    }

    Card(Card&& other) noexcept : suit(std::exchange(other.suit, Suits::SUITS_COUNT))
                                , rank(std::exchange(other.rank, Ranks::RANKS_COUNT))
                                , ascii_card(std::move(other.ascii_card))
    {
        // cout << "Move constructing " << *this << endl;
    }
    
    Card& operator=(Card&& other) noexcept 
    {
        // cout << "Move assigning " << *this << endl;
        if (this == &other)
            return *this;

        this->suit = std::exchange(other.suit, Suits::SUITS_COUNT);
        this->rank = std::exchange(other.rank, Ranks::RANKS_COUNT);
        this->ascii_card = std::move(other.ascii_card);
        
        return *this;
    }
    
    Suits Suit() const { return suit; }
    Ranks Rank() const { return rank; }
    const string& AsciiCard() const { return ascii_card; }

    friend ostream& operator<<(ostream& os, const Card& card) 
    {
        os << "Card(" << card.Rank() << ", " << card.Suit() << ")" << endl;
        os << card.AsciiCard() << endl;
        return os;
    }
};
int main()
{
    for (int i = 0; i < 1000; ++i)
    {
        vector<Card> deck(52, Card(Suits::SUITS_COUNT, Ranks::RANKS_COUNT));
        auto card_count = 0;
        for (size_t rank = 0; rank < static_cast<size_t>(Ranks::RANKS_COUNT); ++rank)
        {
            for (size_t suit = 0; suit < static_cast<size_t>(Suits::SUITS_COUNT); ++suit)
            {
                // const auto before = std::chrono::system_clock::now();

                // cout << "Creating " << static_cast<Suits>(suit) << " " << static_cast<Ranks>(rank) << endl;
                deck[card_count++] = Card(static_cast<Suits>(suit), static_cast<Ranks>(rank));
                // auto duration = std::chrono::system_clock::now() - before;

                // cout << "It took " << duration.count() << "s" << endl << endl;
            }
        }
    }

    // for (const auto& card : deck)
    //     cout << card;

    return 0;
}
