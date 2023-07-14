#pragma once

#include<bits/stdc++.h>
#include<string.h>
#include "playerInterface.h"
int W1,W2,W3,W4 ;

int heurisric1(mancalaBoard& mancala , int addtitional = 0 ) {
    W1 = 1 ;
    return W1*(mancala.getTotStoneCount(2) - mancala.getTotStoneCount(1)) ;
}

int heurisric2(mancalaBoard& mancala , int addtitional = 0 ) {
    int opponent = (mancala.cur_player == 2) ? 1 : 2 ;
    W1 = 10 ,W2 = 1  ;

    return W1 * (mancala.getTotStoneCount(2) - mancala.getTotStoneCount(1))
            + W2 * (mancala.ownSideStones(2) - mancala.ownSideStones(1))  ;
}

int heuristic3(mancalaBoard& mancala , int additional = 0 ) {
    int opponent = (mancala.cur_player == 2) ? 1 : 2 ;
    W1 = 10 ,W2 = 2 ,W3 = 1 ;

    return W1 * (mancala.getTotStoneCount(2) - mancala.getTotStoneCount(1))
            + W2 * (mancala.ownSideStones(2) - mancala.ownSideStones(1))
            + W3 * additional ;
}

static int (*heuristics[])(mancalaBoard& mancala, int additional) = {
heurisric1,
heurisric2,
heuristic3,
};

class computerBot : public playerInterface {
private:
    //private needed because the other player Interface is for user her/himself that might not be Computer Bot
    int max_depth ;
    int heuristic_id ;

    /**
     * @brief
     *
     * @param alpha ->MAX
     * @param beta --> min
     * @param depth
     * @param extra
     * @return std::pair<int,int> first one is to define alpha or beta, 2nd one is column for nextState of mancala to move!
     */
    std::pair<int,int> search(mancalaBoard& mancala,int alpha ,int beta , int depth, int additional) {

        if(mancala.gameEnded()) {
            //reached to the end of mancala,
            //just return the scores with final winning configuration
            std::pair<int,int> p = mancala.getScore() ;
            return std::make_pair((p.first - p.second) , -1) ;
        }
        if(depth == max_depth) {
            //reached to leaves states!
            //Now use the heuristic
            //But the column is not yet fixed

            return std::make_pair(heuristics[heuristic_id](mancala , additional) , -1) ;
        }

        //set the alpha beta to MIN_INT and MAX_INT at every depth while preorder traverse according to player move

        std::pair<int , int>parent_alphaBeta ;
        parent_alphaBeta.first = (mancala.cur_player == 1) ? INT_MIN : INT_MAX ;
        parent_alphaBeta.second = 1 ; //just for calculation,It will be changed in runtime

        //We need to consider the move of opponent
        //Thats why we will use the opposite

        int strt = (mancala.cur_player == 1) ? 6 : 1 ;
        int en = (mancala.cur_player == 1) ? 0 : 7 ;
        int dir = (mancala.cur_player == 1) ? -1 : 1 ;



        while(strt != en) {
            // std::cout << col <<"  COLUMN IS ......" ;
            // std::cout << "Player is ......." << mancala.cur_player << '\n' ;
            if(!mancala.canMove(strt)) {
                strt+=dir ;
                continue ;
            }

            //making a copy mancala
            mancalaBoard new_mc(mancala) ;
            new_mc.changeState(strt) ;

            //new_mc.print_board() ;
            int add = additional ;

            //if the current_player and the new_mancala player same then add
            if(new_mc.cur_player == mancala.cur_player) {
                add+=(mancala.cur_player == 1 ? +1 : -1) ;
            }

            //calling recursively
            std::pair<int,int> child = search(new_mc,alpha ,beta , depth+1 , add) ;

            if(mancala.cur_player == 1) {
                //max for player 1
                parent_alphaBeta = (parent_alphaBeta.first < child.first) ? std::make_pair(child.first,strt) : parent_alphaBeta ;
                alpha = (alpha > parent_alphaBeta.first ) ? alpha : parent_alphaBeta.first ;
            }

            else {
                parent_alphaBeta = (parent_alphaBeta.first > child.first) ? std::make_pair(child.first , strt) : parent_alphaBeta ;
                beta = (beta < parent_alphaBeta.first ) ? beta : parent_alphaBeta.first ;
            }

            if(alpha >= beta) break ;

            strt+=dir ;

        }

        return parent_alphaBeta ;


    }
public:
    computerBot(int max_depth , int heuristic_id,std::string name, int gameplayer_id ):playerInterface(name,gameplayer_id) {
        this->max_depth = max_depth ;
        this->heuristic_id = heuristic_id ;
    }

    const std::string getHeuristicName() {
        return "This AI bot uses heuristic " + std::to_string(heuristic_id)+" at max depth "+ std::to_string(max_depth) ;
    }

    //return the min value
    std::pair<int,int> move(mancalaBoard& mancala ) {
        return search(mancala,INT_MIN,INT_MAX,0,0) ;
    }

    ~computerBot() {

    }





} ;
