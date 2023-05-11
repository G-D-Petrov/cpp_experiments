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

class Card {
    Suits suit;
    Ranks rank;

public:
    Card(Suits suit, Ranks rank) : suit(suit), rank(rank) 
    {
        cout << "Constructing " << *this << endl;
    }
    Suits Suit() const { return suit; }
    Ranks Rank() const { return rank; }

    Card(const Card& other) : suit(other.suit), rank(other.rank)
    {
        cout << "Copy constructing " << *this << endl;
    }

    Card(Card&& other) : suit(std::exchange(other.suit, Suits::SUITS_COUNT)), rank(std::exchange(other.rank, Ranks::RANKS_COUNT))
    {
        cout << "Move constructing " << *this << endl;
    }

    friend ostream& operator<<(ostream& os, const Card& card) {
        os << "Card(" << card.Rank() << ", " << card.Suit() << ")";
        return os;
    }
};

template <template<typename...> class container=std::vector>
class Deck {
    container<unique_ptr<Card>> cards;

public:
    Deck() {
        for (int suit = 0; suit < static_cast<int>(Suits::SUITS_COUNT); ++suit) {
            for (int rank = 0; rank < static_cast<int>(Ranks::RANKS_COUNT); ++rank) {
                cards.push_back(make_unique<Card>(static_cast<Suits>(suit), static_cast<Ranks>(rank)));
            }
        }
    }

    auto& Cards() const { return cards; }

    void Shuffle() {
        random_device rd;
        mt19937 g(rd());
        shuffle(cards.begin(), cards.end(), g);
    }

    unique_ptr<Card> Draw() {
        if (cards.empty()) {
            return nullptr;
        }

        unique_ptr<Card> card = move(cards.back());
        cards.pop_back();
        return card;
    }

    Card& operator[](int index) {
        if (index < 0 || index >= cards.size()) {
            throw out_of_range("Index out of range");
        }
        return *cards[index];
    }

    auto begin() const { return cards.begin(); }
    auto end() const { return cards.end(); }
};

ostream& operator<<(ostream& os, const Deck<>& deck) {
    os << "Deck(" << deck.Cards().size() << " cards)";
    for (const auto& card : deck) {
        os << "\n\t" << *card;
    }
    return os;
}

class Player {
    string name;
    vector<unique_ptr<Card>> hand;

public:
    Player(const string& name) : name(name) {}

    const string& Name() const { return name; }

    const vector<unique_ptr<Card>>& Hand() const { return hand; }

    void AddCard(unique_ptr<Card> card) {
        hand.push_back(std::move(card));
    }

};

ostream& operator<<(ostream& os, const Player& player) {
    os << "Player(" << player.Name() << ")";
    for (const auto& card : player.Hand()) {
        os << "\n\t" << *card;
    }
    return os;
}

int main() {
    // unique_ptr<Card> card = make_unique<Card>(Suits::CLUBS, Ranks::ACE);

    unique_ptr<Deck<>> deck = make_unique<Deck<>>();

    unique_ptr<Player> player = make_unique<Player>("John");
    unique_ptr<Player> dealer = make_unique<Player>("Dealer");

    cout << "Access with *" << endl;
    cout << (*deck)[0] << endl;

    cout << endl;

    cout << *deck << endl;

    cout << endl;

    deck->Shuffle();

    cout << *deck << endl;

    cout << endl;

    dealer->AddCard(deck->Draw());
    dealer->AddCard(deck->Draw());

    player->AddCard(deck->Draw());
    player->AddCard(deck->Draw());

    cout << *deck << endl;

    cout << endl;

    cout << *dealer << endl;

    cout << endl;

    cout << *player << endl;

    for (const auto& card : *deck) {
        cout << *card << endl;
    }

    return 0;
}
