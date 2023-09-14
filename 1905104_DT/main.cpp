#include<bits/stdc++.h>
#include "decision_tree.h"
#include "reader.h"
#include "example.h"
#include <random>

double x ;
void split(example& org,example& train, example& test) {
    ll split = (ll) (x * org.table_data.size()) ;
    for(ll i = 0; i < split ; i++) {
        train.table_data.push_back(org.table_data[i]) ;
    }
    for(ll j = split ; j < org.table_data.size() ; j++) {
        test.table_data.push_back(org.table_data[j]) ;
    }
}

int main() {
    std::string name = "cars.txt" ;
    std::string att_names = "attributes.txt" ;
    std::random_device rd;
    std::mt19937 g(rd());
    srand(unsigned(time(0)));
    std::cout << "Enter dataset range % for train : " ;
    std::cin >> x ;

    ll iteration = 50 ;
    std::cout <<"Total Iteration : " ;
    std::cin >> iteration ;

    example table ;
    readFile(name,table) ;
    std::vector<std::vector<std::string>> attr_val_list ;
    attr_val_list.resize(table.table_data[0].size()) ;
    readAttributes(att_names,attr_val_list) ;
    get_attr_val_list(table,attr_val_list) ;

    for(ll k = 0 ; k < attr_val_list.size() ; k++) {
        for(ll l = 0 ; l < attr_val_list[k].size() ; l++) {
            std::cout << attr_val_list[k][l] <<" " ;
            //break ;
        }
        std::cout << '\n' ;
    }
    //for(ll k = 0 ; k < attr_val_list.size() ; k++)
    
    double avg = 0.0 ;
        // std::vector<std::vector<std::string>> attr_val_list ;
        // attr_val_list.push_back({"vhigh", "high", "med", "low"}) ;
        // attr_val_list.push_back({"vhigh", "high", "med", "low"}) ;
        // attr_val_list.push_back({"2","3","5more","4"}) ;
        // attr_val_list.push_back({"2","4","more"}) ;
        // attr_val_list.push_back({"small","med","big"}) ;
        // attr_val_list.push_back({"low", "med" , "high"}) ;
        // attr_val_list.push_back({"unacc","good","vgood","acc"}) ;
        // for(ll q = 0 ; q < attr_val_list.size() ; q++) {
        //     for(ll w = 0 ; w < attr_val_list[q].size() ; w++) {
        //         std::cout << attr_val_list[q][w] <<" " ;
        //     }
        //     std::cout <<'\n' ;
        // }

    std::vector<double>std_dev;
    for(ll i = 0 ; i < iteration ; i++) {
        std::shuffle(table.table_data.begin(), table.table_data.end(),g) ;
        example train , test ;
        split(table , train ,test)  ;
        train.attr_names = test.attr_names = table.attr_names ;
        //std::cout << "train_dataset row : " << train.table_data.size() <<'\n' ;
        decision_tree cdi(train) ;
        cdi.initial_example = train ;
        cdi.execute(cdi.initial_example , 0, attr_val_list,0) ;
        //cdi.print_tree(0,"") ;
        ll hit = 0 ;
        for(ll j = 0 ; j < test.table_data.size() ; j++) {
            std::vector<std::string> tst ;
            for(ll k = 0 ; k < test.table_data[j].size() - 1 ; k++) {
                tst.push_back(test.table_data[j][k]) ;
                //std::cout << tst[k] << ' ' ;
            }
            //std::cout << '\n' ;

            std::string label = cdi.think(tst) ;
            //std::cout << label <<'\n' ;
            if(test.table_data[j][test.table_data[j].size() - 1] == label) {
                hit++ ;
                //std::cout << test.table_data[j][test.table_data[j].size() - 1] <<" " << label <<'\n' ;
            }
            //std::cout << hit << '\n' ;
        }
        std::cout <<"test set " << i+1 <<" : " << hit << "/" << test.table_data.size() <<'\n' ;
        std_dev.push_back((double)hit/(double) test.table_data.size()) ;

        double accuracy = (double) hit / test.table_data.size() ;
        avg+=accuracy ;
    }
    avg/=iteration ;

    double std_deviation = 0.0 ;
    for(ll i = 0 ; i < std_dev.size() ; i++) {
        std_deviation+=(std_dev[i] - avg) * (std_dev[i] - avg) ;
    }

    std_deviation/=(double) iteration ;
    std::cout << "Mean Accuracy : " << avg * 100 <<'\n' ;
    std::cout << "Standard Deviation : " << sqrt(std_deviation) <<'\n' ;
}