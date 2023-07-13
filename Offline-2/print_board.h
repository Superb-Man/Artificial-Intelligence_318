#pragma once

#include<bits/stdc++.h>
#include "mancalaBoard.h"
  // int a[2] ;
  // int arr[2][6];

std::pair<int,int> calc(int n) {
  std::pair<int,int> p;
  p.second = n%10 ;
  n/=10 ;
  p.first = n%10 ;
  return p ;
}


void print(mancalaBoard& mancala) {
  //top wall
  for(int z = 0; z < 3 ; z++) {
    std::cout << " ";
    if(z == 0) {
      for(int i = 0 ; i < 160 ; i++){
        std::cout << "-" ;
      }
    }
    std::cout << std::endl;
    for(int j = 0 ; j < 6 ; j++) {
      for(int i = 0 ; i <= 160 ; i++) {
        char space = ' ' ;
        if(j == 3) {
          if((z == 0 or z == 2) and i/20 > 0 and i/20 < 7) {
            int row = (z==0) ? 0 : 1 ;
            int col = i/20  ;
            space = (i%20 != 10) ? ((i%20==11) ? calc(mancala.getStoneCount(row,col)).second +'0' : space): calc(mancala.getStoneCount(row,col)).first +'0' ;
          }
          else if(z == 1 and(i/20 == 0 or i/20 == 7)) {
            int pl = i/20 ;
            space = (i%20 != 10) ? ((i%20==11) ? calc(mancala.getTotStoneCount(pl%2)).second +'0' : space): calc(mancala.getTotStoneCount(pl%2)).first +'0' ;
          }
        }
        (i%20 == 0) ? ((i <=140) ? ((z==1 && i/20 > 1 && i/20 < 7) ? std::cout <<space : std::cout << "|" ) : std::cout <<" |") : std::cout <<space ;
      }
      std::cout << std::endl;
    }


    for(int i = 0 ; i < 160 ; i++){
      (z <2) ? ((i/20 >= 1 && i/20 < 7) ? std::cout << "-" : std::cout << " ") :std::cout << "-" ;
    }
    std::cout<< std::endl;
  }
}
