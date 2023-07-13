#pragma once 

#include<bits/stdc++.h>
#include "mancalaBoard.h"

class playerInterface {
protected:
    std::string name ;
    int gameplayer_id ;
    mancalaBoard mancala ;
public:
    playerInterface() = default ;
    
    playerInterface(std::string name , int gameplayer_id) {
        this->name = name ;
        this->gameplayer_id = gameplayer_id ;
    }

    std::string getName() {
        return name ;
    }


    int getGameplayerID() {
        return gameplayer_id ;
    }

    virtual const std::string getHeuristicName() = 0 ;
    //Using purpose of Human and Computer
    virtual std::pair<int,int> move(mancalaBoard& mancala) = 0 ;
    virtual ~playerInterface() = default ;
};