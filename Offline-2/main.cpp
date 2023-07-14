#include<bits/stdc++.h>
#include "playerInterface.h"
#include "computerBot.h"
#include "human.h"
#include "print_board.h"
#include<windows.h>

int main() {
    playerInterface* player1 ;

    std :: cout << "Choose AI vs AI -------->[ 1 ] "<< std:: endl;
    std:: cout << "Choose AI vs Human--------->[ 2 ]" << std :: endl;

    int op ;
    std :: cin >> op ;
    if(op == 2) {
        std::string name ;
        int player_id ;
        std::cin >> name ; ; //Give 1 doesn't matter
        player1 = new human(name, 1) ;
    }
    else {
        player1 = new computerBot(12,1,"AI_BOLOD",1) ;
    }
    playerInterface* player2 = new computerBot(12,2,"AI_CHAGOL",2) ;

    std::cout << "Starting game"<<std::endl;
    std::cout << "First player is " << player1->getName() << std::endl;
    std::cout << "Second player is " << player2->getName() << std::endl;


    //mancala running

    mancalaBoard mancala ;
    //printBoard(mancala) ;

    int player = mancala.cur_player ;
    print(mancala) ;

    int cc= 0 ;

    while(!mancala.gameEnded()) {

        std::cout <<"Current Player" << "Player" << player << '\n' ;

        std::pair<int,int> move = (player == 1) ? player1->move(mancala) : player2->move(mancala) ;
        // std::cout << move.second <<"   MOVE COLUMN" <<'\n';
        // std::cout <<player << "        Player"<<'\n' ;
        // std::cout <<mancala.cur_player <<"        Mancala_player" << '\n' ;
        std::cout << "Selected column :  " << move.second << '\n' ;
        lastState ls = mancala.changeState(move.second) ;

        //now need to check if it is normal, bonus or extra

        //printBoard(mancala) ;
        if(ls.state == NORMAL) {
            std :: cout << "[ Most recent --->Last stone moved at row : " << ls.row + 1<< " and Column : " << ls.col + 1 << " ]\n" ;
        }
        else if(ls.state == BONUS) {
            std::cout <<"{ Bonus move awarded! }" << '\n' ;
        }

        else if(ls.state = EXTRA) {
            std::cout << "[ Most recent --->Last stone moved at row : " << ls.row + 1 << " and Column : " << ls.col + 1 << " ]\n" ;
            std::cout << "[ Stealing the opponents stones ] \n" ;
        }

        //std::cout << "Player1 score : " << mancala.getTotStoneCount(1) << '\n' ;

        //std::cout << "Player2 score : " << mancala.getTotStoneCount(2) <<"\n" ;

        std:: cout << "\nCurrent Board\n" ;
        print(mancala) ;
        mancala.print_board() ;
        player = mancala.cur_player ;

        cc++ ;
        if(mancala.getTotStoneCount(1) >=25 or mancala.getTotStoneCount(2) >=25) {
            break ;
        }
       // if(cc == 20 ) break ;


    }

    player = mancala.getWinner() ;
    print(mancala) ;

    std::cout << "Player1 score : " << mancala.getTotStoneCount(1) << '\n' ;

    std::cout << "Player2 score : " << mancala.getTotStoneCount(2) <<"\n" ;


}
