#include <iostream>
#include <string>
#include <vector>
#include <time.h> 
using namespace std;
void playGame();
class card{
	public:
	card();
	//card(int number, char color);//a constructor to make a specific card, we should never need this
	card(int handSize, card top);//a constructor that takes the hand size and the card needed to match: used to bias the randomizer against players that are closer to winning (if we want to be evil)
	friend ostream& operator<<(ostream& out, const card& right);
	int getNumber();
	char getColor();
	void setColor(char color);//for choosing the color of a wild
	private:
	int number;//10 = draw two, 11 = skip, 12 = reverse, 13 = wild (ignore color), 14 = wild draw 4 (ignore color)
	char color;//r, g, b, y, or n for a colorless card
};
char wildChoice();
int main(){
	srand(time(0));//seed the randomizer with the time
	playGame();
	return 0;
}
ostream& operator<<(ostream& out, const card& right){
	if(right.color == 'y')
		out << "yellow ";
	else if(right.color == 'b')
		out << "blue ";
	else if(right.color == 'r')
		out << "red ";
	else if(right.color == 'g')
		out << "green ";
	if(right.number < 10)
		out << right.number;
	else if(right.number == 10)
		out << "draw two";
	else if(right.number == 11)
		out << "skip";
	else if(right.number == 12)
		out << "reverse";
	else if(right.number == 13)
		out << "wild";
	else if(right.number == 14)
		out << "wild draw four";
	return out;
}
card::card(){
	number = (rand() % 15);//between 0 and 14, for 0-9 plus the special cards
	color = (rand() % 4);//between 0 and 3, we pretend this is an int for now
	if(color == 0)
		color = 'y';
	else if(color == 1)
		color = 'b';
	else if(color == 2)
		color = 'r';
	else if(color == 3)
		color = 'g';
	if(number >= 13)//if it's a wild, don't give it a color
		color = 'n';
}
char card::getColor(){
	return color;
}
int card::getNumber(){
	return number;
}
void card::setColor(char colorIn){
	color = colorIn;
}
char wildChoice(){
	char color;
	cout << "Wildcard color (r, g, b, y):  ";
	cin >> color;
	return color;
}
void playGame(){
	int i, j, playerCount, currentPlayer = 0, nextPlayerDraws = 0, cardToPlay;
	bool canPlay = false, playDirection = true, crazy = true;//for playDirection: true = clockwise (incrementing), false = counterclockwise (decrementing). crazy = whether or not the 0 and 7 rule is active
	card top, temp;
	top = card();
	while(top.getNumber() >= 10 || top.getNumber() == 7 || top.getNumber() == 0)//reinitialize the first card until it's normal
		top = card();
	cout << "how many players?: ";
	cin >> playerCount;
	vector<vector<card>> playerHands(playerCount);
	for(i = playerCount-1; i >= 0; i--){
		for(j = 0; j < 7; j++){
		playerHands.at(i).push_back(card());//put 7 cards in each player's hand to start out
		}
	}
	
	while(true){//breaks when someone wins
		cout << "top: " << top << endl;
		cout << "player " << currentPlayer+1 << "\'s turn!" << endl;
		while(nextPlayerDraws != 0){//if the previous player used a draw 2 or a draw 4, make the current player draw those cards
		temp = card();
		//if(playerHands.at(currentPlayer).size() == 1)
		//	temp = card(playerHands.at(currentPlayer).size(), top);//bias against winners
		playerHands.at(currentPlayer).push_back(temp);
		cout << "drew a " << temp << endl;
		nextPlayerDraws--;
		}
	
		//press button to continue here to avoid showing other players the cards
		cout << "your hand: ";
		for(i = 0; i < playerHands.at(currentPlayer).size(); i++){//output the hand and check if they can play at the same time for efficiency
		if(!canPlay)//only do this check if none of the previous cards were playable
			canPlay = (playerHands.at(currentPlayer).at(i).getNumber() == top.getNumber() || playerHands.at(currentPlayer).at(i).getColor() == top.getColor() || playerHands.at(currentPlayer).at(i).getNumber() >= 13);
		if(i != 0)//don't print an extra comma
			cout << ", " << i+1 << ": ";
		else
			cout << "1: ";
		cout << playerHands.at(currentPlayer).at(i);
		}
		cout << endl;
		if(!canPlay){
		cout << "no playable cards!" << endl;
		}
		while(!canPlay){
		//probably add a button to draw and a button to play when you get a good card
		temp = card();
		//if(playerHands.at(currentPlayer).size() == 1)
		//	temp = card(playerHands.at(currentPlayer).size(), top);//bias against winners
		playerHands.at(currentPlayer).push_back(temp);
		cout << playerHands.at(currentPlayer).size() << ": ";
		cout << "drew a " << temp << endl;
		canPlay = (temp.getNumber() == top.getNumber() || temp.getColor() == top.getColor() || temp.getNumber() >= 13);
		}
		canPlay = false;//reset the variable
		cout << "which card do you want to play? (1 - " << playerHands.at(currentPlayer).size() << "): ";
		cin >> cardToPlay;//get which card they want to use
		while(!(playerHands.at(currentPlayer).at(cardToPlay-1).getNumber() == top.getNumber() || playerHands.at(currentPlayer).at(cardToPlay-1).getColor() == top.getColor() || playerHands.at(currentPlayer).at(cardToPlay-1).getNumber() >= 13)){//while the chosen card can't be used...
		cout << "that card cannot be played." << endl;
		cout << "which card do you want to play? (1 - " << playerHands.at(currentPlayer).size() << "): ";//continue asking until they reply with a usable card
		cin >> cardToPlay;
		}
		top = playerHands.at(currentPlayer).at(cardToPlay-1);//put their played card on top of the deck
		playerHands.at(currentPlayer).erase(playerHands.at(currentPlayer).begin() + cardToPlay-1);//remove the played card from their hand INNEFICIENTLY, consider switching to a linked list or something
		if(playerHands.at(currentPlayer).size() == 0){//if they just won, don't bother making them set a wildcard or anything so we exit now
		cout << "Player " << currentPlayer+1 << " has won!" << endl;
		break;
		}
		//accounting for the special cards
		if(top.getNumber() == 10)//draw 2
			nextPlayerDraws = 2;
		else if(top.getNumber() == 14){//draw 4
			nextPlayerDraws = 4;
			top.setColor(wildChoice());
		}
		else if(top.getNumber() == 12)//reverse
			playDirection = !playDirection;
		else if(top.getNumber() == 13)//wild
			top.setColor(wildChoice());
		if(playDirection){
			currentPlayer++;
		if(top.getNumber() == 11)
			currentPlayer++;//skip a player
			currentPlayer %= playerCount;//move on to the next player, wrapping from the last to the first
		}
		else{
			currentPlayer--;
		if(top.getNumber() == 11)
			currentPlayer--;//skip a player
		if(currentPlayer < 0)
			currentPlayer += playerCount;//move on to the next player, wrapping from the first to the last
		}
	}
}
