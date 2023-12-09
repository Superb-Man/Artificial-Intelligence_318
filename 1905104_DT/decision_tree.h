#pragma once
#include<bits/stdc++.h>
#include "example.h"
typedef long long ll ;

struct node{
    ll c_index ; //for attr_criteria_index
    ll tree_indx ; 
    std::string attr_val ; //for branching
    std::string result ; //for labeling
    std::vector<ll>children ; //for childlist
    bool leaf ;

    node() {
        this->leaf = false ;
    }
    node(ll tree_index) {
        this->tree_indx = tree_index ;
        this->leaf = false ;
    }
    
} ;

class decision_tree{
public:
    //variables
    example initial_example ;
    std::vector<node> tree ;// for each detpth nodes

    //constructor
    decision_tree(example) ;
    
    //methods
    std::string think(std::vector<std::string>) ;
    ll search(std::vector<std::string>& , ll) ;
    bool is_leaf(example) ;
    double entropy_parent(example) ;
    double entropy_attr(example , ll,ll) ;
    ll selected_attr(example, std::vector<std::vector<std::string>>) ;
    std::pair<std::string ,ll>majority_label(example);
    void print_tree(ll , std::string) ;

    void execute(example, ll,std::vector<std::vector<std::string>>,ll) ;

};

decision_tree::decision_tree(example initial_example) {
    this->initial_example = initial_example ;
    node root ;
    root.tree_indx = 0 ;
    tree.push_back(root) ;
    ////std::cout << "Seg fault not here\n" ;
}

/**
 * @param line_input is the every data line sparsed
 * from the input_table
 * @return the label of input line
*/
std::string decision_tree::think(std::vector<std::string>line_input) {
    std::string label = "" ;
    ll x = search(line_input , 0) ;
    if(x == -1) {
        ////std::cout <<"I cannot guess anything!Bad AI\n" ;
        return "lol Bad AI" ;
    }
    ////std::cout << tree[x].result << '\n' ;
    return tree[x].result ;
}

/**
 * @param line for each line from input
 * @param depth -> at which depth the attribute_name exist
 * therefore do branching according to attribute value
 * @return the index of the desired node
*/
ll decision_tree::search(std::vector<std::string>&line_input , ll tree_idx) {
    if(tree[tree_idx].leaf) return tree_idx ;

    for(ll i = 0 ; i < tree[tree_idx].children.size() ; i++) {
        if(line_input[tree[tree_idx].c_index] == tree[tree[tree_idx].children[i]].attr_val)
            return search(line_input , tree[tree_idx].children[i]) ;
    } 

    return -1 ;
}

/**
 * checks if an intermideate generated table is leaf
 * Thus checks the Output or validation
 * EX-> YES,NO,1,0... desired output type
*/

bool decision_tree::is_leaf(example e) {
    ////std::cout << "Debug\n" ;
    for(ll i =  1 ; i < e.table_data.size() ; i++) {
        if(e.table_data[0][e.table_data[0].size() - 1] != e.table_data[i][e.table_data[i].size() - 1])
            return false ;
    }

    ////std::cout << "Debug--->true\n" ;

    return true ;
}


double decision_tree::entropy_parent(example e) {
    double res = 0.0 ;
    std::map<std::string , ll> label_cnt ;

    for(ll i = 0 ; i < e.table_data.size() ; i++) {
        //couting the classifications of output
        label_cnt[e.table_data[i][e.table_data[i].size()-1]]++ ;
    }

    double p ;
    for(auto it : label_cnt) {
        p = (double) it.second/(double)e.table_data.size() ;
        //res+= 1.0 * p * log2(1/p))
        res+= (-1.0 * p * log2(p)) ; // doing this to recover from fractional loss
    }

    return res ;
}

double decision_tree::entropy_attr(example e , ll attr_index , ll gr = 0) {
    double res = 0.0 ;
    std::map<std::string , std::vector<ll>> attr_val_map ;

    for(ll i = 0 ; i < e.table_data.size() ; i++) {
        attr_val_map[e.table_data[i][attr_index]].push_back(i) ;
    }

    for(auto it : attr_val_map) {
        example nxt ;
        for(ll i = 0 ; i < it.second.size() ; i++)
            nxt.table_data.push_back(e.table_data[it.second[i]]) ;
        if(!gr) res+= (double) entropy_parent(nxt) * ((double)nxt.table_data.size()/(double)e.table_data.size()) ;
        else {
            double d ;
            d = (double) nxt.table_data.size() / (double)e.table_data.size() ;
            res+= -1.0 * d* log2(d) ; 
        }
    }

    return res ;

}

/**
 * @param table
 * checks according to the gain ratio or gain (that we can differ and change using a paramter)
 * Select the attributename with more gain_ratio / gain
 * @return the index of the attribute in table
*/

ll decision_tree::selected_attr(example e, std::vector<std::vector<std::string>>attr_val_list) {
    ll mx_idx = -1 ;
    double mx_val = -1 ;

    for(ll i = 0 ; i < attr_val_list.size() - 1 ; i++) {
        double gain = entropy_parent(e) - entropy_attr(e,i,0) ;
        double gain_ratio = gain / entropy_attr(e,i,1) ;
        //std::cout << "gain :" <<gain_ratio << '\n' ;
        if(mx_val < gain_ratio) {
            mx_val = gain_ratio ;
            mx_idx = i ;
        }
    }
    //std::cout << mx_idx << '\n' ;

    return mx_idx ;
}

/**
 * just as theory..checks the last column counter
 * based on labeling 
 * and sets majority
 * for counter , we used map
*/
std::pair<std::string,ll> decision_tree::majority_label(example e) {
    std::map<std::string,ll> label_cnt ;
    std::pair<std::string, ll> majority_pair ;
    majority_pair.first ="" ;
    majority_pair.second = 0 ;

    for(ll i = 0 ; i < e.table_data.size() ; i++) {
        ll  col = e.table_data[i].size()-1 ;
        label_cnt[e.table_data[i][col]]++ ;
        ll val = label_cnt[e.table_data[i][col]] ;
        majority_pair = (val > majority_pair.second ? 
                            std::make_pair(e.table_data[i][col],val) : majority_pair) ;

    }

    return majority_pair ;

}

void decision_tree::print_tree(ll node_idx , std::string branching) {
    //std::cout <<"called\n" ;
    if(tree[node_idx].leaf) {
        std::cout << branching << "Label: " << tree[node_idx].result <<'\n' ;
    }

    for(ll i = 0 ; i < tree[node_idx].children.size(); i++) {
        std::string str = branching + initial_example.attr_names[tree[node_idx].c_index] + " = " + tree[tree[node_idx].children[i]].attr_val+ ", " ; 
        print_tree(tree[node_idx].children[i],  str) ;
    }
}

/**
 * @param table
 * @param node_index for detecting which node(as we didnot use pointers!!)
 * @param attr_val_list for branching 
*/
void decision_tree::execute(example e , ll node_idx, std::vector<std::vector<std::string>>attr_val_list,ll depth) {
    //case : if it's a leaf..Only yes or no type
    if(is_leaf(e)) {
        tree[node_idx].leaf = true ;
        ll z = e.table_data.size() ;
        ll k = e.table_data[z-1].size() ;

        tree[node_idx].result = e.table_data[z-1][k-1] ;
        return ;
    }

    if(attr_val_list.size() -1 == 0) {
        tree[node_idx].leaf = true ;
        tree[node_idx].result = majority_label(e).first ;

        return ;
    }

    // if(depth > e.attr_names.size() - 1) {
    //     //std::cout << "lol" <<'\n' ;
    //     tree[node_idx].leaf = true ;
    //     tree[node_idx].result = majority_label(e).first ;
    //     return ;
    // }

    ////std::cout << "Debug\n" ;
    std::map<std::string , std::vector<ll>> attr_val_mp ;
    ll selected_idx = selected_attr(e,attr_val_list) ;
    for(ll i = 0 ; i < e.table_data.size() ;i++) {
        attr_val_mp[e.table_data[i][selected_idx]].push_back(i) ;
    }

    tree[node_idx].c_index = selected_idx ;
    std::pair<std::string,ll>majority_pair = majority_label(e) ;



    //case : if ratio > 0.8 then ofcourse we are going clkose to have one type of output
    //So we can return 
    //checking this to make it more pure
    //On averge more than 80% example results are same .So making it child


    for(ll i = 0 ; i < attr_val_list[selected_idx].size() ; i++) {
        //making a new example_table based on this attr_bute index ;
        example e_new ;
        node nxt_node ;
        std::string attr_val = attr_val_list[selected_idx][i] ; 
        std::vector<ll>candidates = attr_val_mp[attr_val] ;
        


        std::vector<std::vector<std::string>>new_attr_val_list ;
        // ////std::cout << "selected _idx : " <<selected_idx << '\n' ;
        //new_attr_val_list.erase(attr_val_list.begin() + selected_idx) ;
        for(ll k = 0 ; k < attr_val_list.size() - 1 ; k++) {
            if (k == selected_idx) continue ;
            new_attr_val_list.push_back(attr_val_list[k]) ;
        }
        ////std::cout << "size : " <<attr_val_list.size() << " " <<new_attr_val_list.size() <<'\n' ;
        // //std::cout << "size : " <<attr_val_list.size() << " " <<new_attr_val_list.size() <<'\n' ;
        // //std::cout << "deleted\n" ;
        // //std::cout << "candidates size : " << candidates.size() <<'\n' ;

        for(ll j = 0 ; j < candidates.size() ; j++) {
            ////std::cout << candidates[j] <<"\n" ;
            ////std::cout << e.table_data.size() <<'\n';
            e_new.table_data.push_back(e.table_data[candidates[j]]) ;
        }
        ////std::cout << "reached here\n" ;

        nxt_node.attr_val = attr_val ;
        nxt_node.tree_indx = (ll) tree.size() ;
        tree[node_idx].children.push_back((ll)tree.size()) ;
        tree.push_back(nxt_node) ;
        ////std::cout <<"Debug\n" ;

        ///case : all examples are over
        //make this node as leaf
        //set the label from parent
        if(e_new.table_data.size() == 0) {
            nxt_node.leaf = true ;
            //take from parent majority label
            nxt_node.result = majority_label(e).first ;
            tree[nxt_node.tree_indx] = nxt_node ;
            //return ;
        }

        else {
            //case 4 : continue this process
            ////std::cout << "recursive call\n" ;
            execute(e_new , nxt_node.tree_indx , attr_val_list,depth+1) ;
        }

    }



}
