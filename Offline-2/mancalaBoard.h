#pragma once

#include<bits/stdc++.h>
const int mod = 2 ;

const int NORMAL = 1 ;
const int BONUS = 2 ;
const int EXTRA = 3 ;


struct lastState{
    int row ;
    int col ; ;
    int state ;
    int captured ;

    lastState() {
        row = col = 0  ;
    }

    lastState(int state,int row, int col) {
        this->state = state ;
        this->row = row ;
        this->col = col ;
    }
};

class mancalaBoard {
private:
    int target_pot[2] ; //two final pot to save/put the stones, 1%2 for player1 , 2%2 for player2
    int game_board[2][6] ; //2 rows and 6 column in each ,1%2 for player1 , 2%2 for player2

    lastState ls ;


    int counnt(int row) { //counting the column of each row seperately
        int cnt = 0 ;
        for(int j = 0 ; j < 6 ; j++) cnt+=game_board[row%mod][j] ;

        return cnt ;
    }

    bool chk(int col) {
        return col > 5 or col < 0 ;
    }

    void move(int& stones , int& row, int& col , int& direction, int& prev_stones) {
        for(int j = 0 ; j < stones ;) {
            if(chk(col+direction)) {
                if((row+1)%mod != cur_player %mod){
                    target_pot[cur_player%mod]++ ;  //putting a stone in targeting pot
                    j++ ;
                }
                row = (row + 1)%mod ;    //going to another player's territory
                col+=direction ;         //increasing/decreasing column
                direction = -direction ; //direction reversing
            }
            else {
                prev_stones = game_board[row%mod][col+direction] ;
                game_board[row%mod][col+direction]++ ;
                col+=direction ;
                j++ ;
            }
        }
        stones = 0 ;
    }

public:

    int cur_player ;
    /**
     * @brief Construct a new mancala Board object
     * initializing a mancala game board
     * @param stones
     */
    mancalaBoard() {
        for(int i = 0 ; i < 2 ; i++) {
            for(int j = 0 ; j < 6 ; j++) {
                game_board[i][j] = 4 ;
            }
        }

        cur_player = 1 ;
        target_pot[0] = 0 ;
        target_pot[1] = 0 ;
    }

    //Copy constructor
    mancalaBoard(const mancalaBoard& mancala) {
        for(int i = 0 ; i < 2 ; i++) {
            for(int j = 0 ; j < 6 ; j++) {
                game_board[i][j] = mancala.game_board[i][j] ;
            }
        }

        cur_player = mancala.cur_player ;
        //ls = mancala.ls ;

        target_pot[0] = mancala.target_pot[0] ;
        target_pot[1] = mancala.target_pot[1] ;
    }

    //mancalaBoard(const mancalaBoard& game_board)


    bool gameEnded() { //if any of the row become 0 the game ends!
        //if(target_pot[1] >= 25 or target_pot[0] >= 25) return true ;
        return (counnt(1) == 0 ? true : (counnt(2) == 0 ? true : false)) ;
    }


    bool canMove(int col) {
        col-- ;
        return game_board[cur_player%mod][col] > 0 ? true : false  ;
    }
    std::pair<int,int> getScore() {
        std::pair<int,int>scores ;
        scores.first = counnt(1) + target_pot[1] ;
        scores.second = counnt(2) + target_pot[2%mod] ;

        return scores ;
    }

    int getWinner() {
        std::pair<int,int> scores = getScore() ;

        target_pot[1]+=counnt(1) ;
        target_pot[2%mod]+=counnt(2) ;

        for(int i = 0 ; i < 2 ; i++) {
            for(int j = 0 ; j < 6 ; j++) game_board[i][j] = 0 ;
        }

        return ((scores.first > scores.second) ? 1 : ((scores.first < scores.second) ? 2 : 0)) ;
    }

    int ownSideStones(int player) {
        int cnt = 0 ;
        for(int col = 0; col < 6 ;col++) {
            cnt+=game_board[player%mod][col] ;
        }

        return cnt ;
    }

    /**
     * @brief get the number of stones in a particular cell in board
     *
     * @return int
     */
    int getStoneCount(int row ,int col) {
        col-- ;
        return game_board[row%mod][col] ;
    }

    /**
     * @brief get the total count of stones in targetPot
     *
     * @param player
     * @return int
     */
    int getTotStoneCount(int player) {
        return target_pot[player%mod] ;
    }

    /**
     * @brief
     * Chooses a block/column and move stones to anticlockwise direction
     * Need to remove all the stones from choosen cell at starting of move
     * @param col because the move we can make it through column
     * @return tupple
     */
    lastState& changeState(int col) {
        ls.captured = 0 ;
        col-- ;
        int row = cur_player ;
        int stones = game_board[row%mod][col] ;
        game_board[row%mod][col] = 0 ;


        int direction = (cur_player == 1 ? 1 : -1) ;
        int prev_stones = -1 ;
        int init_dir = direction ;
        int captured = target_pot[0] ;


        move(stones, row, col, direction, prev_stones) ;

        if(chk(col)) { // the last move ,Getting a bonus, because otherwise col will surely decrease and will be bounded to(0 to 5)
            //RULE 1 in visual gameplay,another move , so row ,column remain unchanged
            ls.row = -1 ; ls.col = -1 ; ls.state = BONUS ;
        }
        // (row == (cur_player == 1) ? 1 : 0)
        else if(row%mod == cur_player%mod && game_board[row%mod][col] == 1 && game_board[(row+1)%mod][col] > 0) { //Stealing the opponets stones!!RULE 2
            ls.captured = (row == 1 ? game_board[2%mod][col] : 0) ;
            target_pot[row%mod]+=game_board[row%mod][col] + game_board[(row+1)%mod][col] ;
            //now collecting all the stones of those two cell
            game_board[row%mod][col] = game_board[(row+1)%mod][col] = 0 ;

            cur_player = (cur_player == 1) ? 2 : 1 ;//Switching player
            ls.row = row%mod; ls.col = col  ; ls.state = EXTRA ;
        }

        else {
            cur_player = (cur_player == 1) ? 2 : 1 ;//Switching player
            ls.row = row%mod ; ls.col = col ; ls.state = NORMAL ;
        }
        return ls ;


    }

    void print_board() {
        for(int i = 0; i < 2 ; i++) {
            for(int j = 0 ; j < 6 ; j++) {
                std::cout << game_board[i][j] ;
                std::cout <<"    " ;
            }
            std::cout <<'\n' ;
        }
        std::cout << target_pot[1] <<"\n" << target_pot[0] << '\n' ;
    }

    int getCaptured(){
        return ls.captured ;
    }
} ;
