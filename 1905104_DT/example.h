#pragma once
#include<bits/stdc++.h>
typedef long long ll ;
struct example{
    std::vector<std::vector<std::string>> table_data ;
    std::vector<std::string> attr_names ; // first row of the input file
};

void get_attr_val_list(example& e,std::vector<std::vector<std::string>>&atrr_val_list) {
	std::map<std::string, ll> val ;
	for(ll j = 0; j < e.table_data.size(); j++) {
    		//identical atrribute value will be inserted only once
		val[e.table_data[j][e.attr_names.size()-1]] = 1;
	}

	for(auto it : val) {
        atrr_val_list[e.attr_names.size()-1].push_back(it.first) ;
	}
}
