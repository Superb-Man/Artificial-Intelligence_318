#pragma once

#include<bits/stdc++.h>
#include "playerInterface.h"

class human:public playerInterface {
public:
    human(std::string name , int gameplayer_id):playerInterface(name,gameplayer_id) {
        //
    }

    const std::string getHeuristicName() {
        return "This Human { "+ name+ " } doesnot uses heuristic" ;
    }

    std::pair<int,int> move(mancalaBoard& mancala) {
        int col ;
        std :: cout <<"Select a column [1->6]\n" ; //Assumimg user will put valid input !!
        std :: cin >> col ;
        //Invalid command not handled!!
        //mancala.changeState(col) ;

        return  std::make_pair(INT_MAX,col) ;
    }
};
