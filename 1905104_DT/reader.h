#pragma once 

#include<bits/stdc++.h>
#include"example.h"


void readFile(std::string file_name, example& read_table) {
    //example read_table ;
    std::ifstream fin ;
    fin.open(file_name) ;

    if(!fin) {
        std::cout << "File not found\n" ;
        exit(0) ;
    }

    //first line is attribute name
    bool data = false ;
    std::string attr = "" ;
    while(!getline(fin,attr).eof()) {
        std::vector<std::string> line ;
        //read a line
        //Now parse the line 
        ll start = 0 ;
        for(ll i = 0 ; i < attr.size() ; i++) {
            if(attr[i] == '\t' || attr[i] == ',') {
                std::string column = attr.substr(start,i-start) ;
                start = i + 1 ;
                line.push_back(column) ;
            }
        }
        // for parsing the last space/tab separated string
        std::string column = attr.substr(start , attr.size() - start - 1) ;
        line.push_back(column) ;
        if(!data) {
            //storing the attributes name
            data = true ;
            read_table.attr_names = line ;
        }
        else {
            //storing the datas
            read_table.table_data.push_back(line) ;
        }
    }

    fin.close() ;

    //return read_table ;
}

void readAttributes(std::string file_name,std::vector<std::vector<std::string>>& attr_val_list) {
    std::ifstream fin ;
    fin.open(file_name) ;

    if(!fin) {
        std::cout << "File not found\n" ;
        exit(0) ;
    }

    ll cnt =  0 ;
    std::string attr = "" ;
    while(!getline(fin,attr).eof()) {
        //std::vector<std::string> line ;
        ll c = 0 ;
        //read a line
        //Now parse the line 
        ll start = 0 ;
        for(ll i = 0 ; i < attr.size() ; i++) {
            if(!i) continue ;
            if(attr[i] == '\t' || attr[i] == ',' || attr[i] == ':' || attr[i] == ' ') {
                if(attr[i-1] != '\t' && attr[i-1] != ',' && attr[i-1] != ':' && attr[i-1] != ' ') {
                    c++ ;
                    std::string column = attr.substr(start,i-start) ;
                    start = i + 1 ;
                    if(c != 1)  attr_val_list[cnt].push_back(column) ;
                }
                else {
                    start = i + 1 ;
                }
            }
        }
        //std::cout << "here\n" ;
        // for parsing the last space/tab separated string
        std::string column = attr.substr(start , attr.size() - start - 1) ;
        //line.push_back(column) ;
        attr_val_list[cnt++].push_back(column) ;
    }

    fin.close() ;
    std::cout << "done" <<'\n' ;
}

