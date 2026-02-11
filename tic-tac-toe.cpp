#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
using namespace std;
char board[3][3];
void initialize() {
    char count='1';
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            board[i][j]=count++;
        }
    }
}

void display(){
    cout << endl;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            cout << " " << board[i][j] << " ";
            if (j < 2) cout << "|";
        }
        cout << endl;
        if (i<2) cout << "-----------" << endl;
    }
    cout << endl;
}

bool checkWin(char player){
    for (int i=0;i<3;i++) {
        if ((board[i][0]==player && board[i][1]==player && board[i][2]==player) ||
            (board[0][i] == player && board[1][i]==player && board[2][i]==player))
            return true;
    }
    if ((board[0][0]==player && board[1][1]==player && board[2][2]==player) ||
        (board[0][2]==player && board[1][1]==player && board[2][0]==player))
        return true;

    return false;
}

bool isDraw(){
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            if (board[i][j]!='X' && board[i][j]!='O')
                return false;
        }
    }
    return true;
}

bool makeMove(char player, int move){
    if (move<1 || move>9) return false;

    int row = (move-1)/3;
    int col = (move-1)%3;

    if (board[row][col]=='X' || board[row][col]=='O')
        return false;

    board[row][col]=player;
    return true;
}

// AI move (medium difficulty)
int aiMove(char aiPlayer, char humanPlayer){
    // 1. Check if AI can win
    for (int i=1;i<=9;i++){
        int row=(i-1)/3;
        int col = (i-1)%3;
        if (board[row][col]!='X' && board[row][col]!='O'){
            board[row][col]=aiPlayer;
            if (checkWin(aiPlayer)) {
                return i;
            }
            board[row][col]='0'+i; // undo
        }
    }
    // 2. Block human player from winning
    for (int i=1;i<=9;i++){
        int row=(i-1)/3;
        int col = (i-1)%3;
        if (board[row][col]!='X' && board[row][col]!='O'){
            board[row][col] = humanPlayer;
            if (checkWin(humanPlayer)) {
                board[row][col] = aiPlayer; // block
                return i;
            }
            board[row][col]='0'+i; // undo
        }
    }
    // 3. Otherwise pick random empty cell
    int move;
    do{
        move=rand()%9+1;
        int row=(move-1)/3;
        int col=(move-1)%3;
        if (board[row][col]!='X' && board[row][col]!='O')
            break;
    } while(true);

    return move;
}

int main() {
    srand(time(0));
    string playerName;
    char humanPlayer, aiPlayer;
    cout << "Do you want to be X or O? ";
    cin >> humanPlayer;
    humanPlayer = toupper(humanPlayer);
    aiPlayer = (humanPlayer == 'X')?'O':'X';
    cout << "Enter your name: ";
    cin >> playerName;
    char starter = (rand()%2==0)?humanPlayer:aiPlayer;
    char playAgain;
    do {
        initialize();
        display();

        char currentPlayer = starter;
        bool gameWon = false;

        if (starter == humanPlayer)
            cout << playerName << " starts first!\n";
        else
            cout << "Computer starts first!\n";

        while (true) {
            int move;

            if (currentPlayer == humanPlayer){
                cout << playerName << "'s turn (" << humanPlayer << "). Enter move (1-9): ";
                cin >> move;
                if (!makeMove(humanPlayer, move)){
                    cout << "Invalid move! Try again.\n";
                    continue;
                }
            } else {
                move = aiMove(aiPlayer, humanPlayer);
                cout << "Computer chooses: " << move << endl;
                makeMove(aiPlayer, move);
            }
            display();
            if (checkWin(currentPlayer)){
                if (currentPlayer == humanPlayer)
                    cout << "Congratulations " << playerName << "! You win 🎉\n";
                else
                    cout << "Computer wins! Better luck next time 😅\n";
                starter = currentPlayer;   // winner starts next game
                gameWon = true;
                break;
            }
            if (isDraw()){
                cout << "It's a draw!\n";
                starter = (starter == humanPlayer) ? aiPlayer : humanPlayer;
                break;
            }

            currentPlayer = (currentPlayer == humanPlayer) ? aiPlayer : humanPlayer;
        }
        cout << "Play again? (y/n): ";
        cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');
    cout << "Thanks for playing!\n";
    return 0;
}
