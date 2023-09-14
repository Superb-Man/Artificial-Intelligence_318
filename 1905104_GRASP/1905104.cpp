#include<bits/stdc++.h>
#include<random>
#define SET_X 1
#define SET_Y 2
#define UNASSIGNED 0
#define ll long long
#define vertex ll
#define MAX_RANGE 10005
#define MAX_ITERATION 50

//variables
ll w_min , w_max ;
ll nodes ;

double sigma_x[MAX_RANGE] ;
double sigma_y[MAX_RANGE] ;

std::set<vertex>set_x ;
std::set<vertex>set_y ;

std::vector<std::pair<ll,ll>> edgelist ;
double g[MAX_RANGE][MAX_RANGE] ;


void clear() {
    if(set_x.begin() != set_x.end())    set_x.clear() ;
    if(set_y.begin() != set_y.end())    set_y.clear() ;
    for(ll i = 0 ; i < nodes ; i++) {
        sigma_x[i] = 0 ;
        sigma_y[i] = 0 ;
    }
}

ll cost() {
    //std::cout << "X = "<<set_x.size() <<" " << "Y = " << set_y.size() <<'\n' ;
    double c = 0 ;
    for(auto x : set_x) {
        for(auto y : set_y) {
            if(g[x][y] != INT_MAX) {
                c+=g[x][y] ;
            }
            // if(g[y][x] != INT_MAX) {
            //     c+=g[y][x] ;
            // }
        }
    }

    return (ll) c ;
}

double min_max(std::vector<std::pair<ll,ll>>& edgelist , bool mini) {
    double w = (!mini ? -1e5 : 1e5) ;
    for(auto e : edgelist) { 
        w = (!mini ? std::max(g[e.first][e.second],w) : std::min(g[e.first][e.second],w)) ;
    }
    return w ;
}

// std::vector<vertex> get_vertex_list(std::set<vertex>& set_x , std::set<vertex>& set_y) {
//     std::vector<vertex> vec ;
//     for(ll i = 0 ; i < nodes ; i++) {
//         if(set_x.find(i) == set_x.end() || set_y.find(i) == set_y.end()) {
//             vec.push_back(i) ;
//         }
//     }

//     return vec ;
// }

double calculate(double* sigma,std::set<vertex>&myset,vertex vert,vertex u, double add = 1) {
    double sum = 0 ;

    if(g[vert][u] != INT_MAX) {
        sigma[vert]+=(add*g[vert][u]) ;
    }
    // else if(g[u][vert] != ll_MAX) {
    //     sigma[vert]+=(add*g[u][vert]) ;
    // }

    return sum ;
}

ll semi_greedy_max_cut(ll type) {
    //std::cout <<"Semi greedy heuristic\n" ;
    
    double alpha = (type == 1 ? 1 : (double) rand()/(RAND_MAX)) ; 

    //std::cout << alpha <<'\n' ;

    double w_min = min_max(edgelist, true) ;
    double w_max = min_max(edgelist, false) ;

    double meu = w_min + alpha * (w_max - w_min) ;
    std::vector<std::pair<ll,ll>> rcl_edges ;

    for(auto e = edgelist.begin(); e != edgelist.end() ; ++e){
        if(g[e->first][e->second] >= meu) {
            rcl_edges.push_back({e->first,e->second}) ;
        }
    }

    std::set<vertex> V ;
    for(vertex v = 0 ; v < nodes ; v++) {
        V.insert(v) ;
    }

    ll cnt = 0 ;

    ll r = rand() % rcl_edges.size()  ;

    set_x.insert(rcl_edges[r].first) ;
    set_y.insert(rcl_edges[r].second) ;

    V.erase(rcl_edges[r].first) ;
    V.erase(rcl_edges[r].second) ;

    for(vertex v : V) {
        //std::cout << v <<'\n' ;
        for(auto u : set_x) calculate(sigma_x,set_y,v,u) ;
        for(auto u : set_y) calculate(sigma_y,set_x,v,u) ;
    }
    //std::cout << V.size() <<'\n' ;

    while(set_x.size() + set_y.size() != nodes) {
        
        w_min = INT_MAX ;
        w_max = INT_MIN ;

        for(auto it = V.begin() ; it != V.end(); ++it) {
            w_min = std::min(w_min, sigma_x[*it]) ;
            w_min = std::min(w_min,sigma_y[*it]) ;
            
            w_max = std::max(w_max, sigma_x[*it]) ;
            w_max = std::max(w_max,sigma_y[*it]) ;

        }

        meu = w_min + alpha * (w_max - w_min) ;

        std::vector<vertex> rcl_v ;

        //std::cout << meu <<" meu\n" ;

        for(auto it = V.begin() ; it != V.end() ; ++it) {
            if(std::max(sigma_x[*it],sigma_y[*it]) >= meu) {
                rcl_v.push_back(*it) ;
            }
        }

        ll mod = rand()% rcl_v.size() ;
        vertex v_star = rcl_v[mod] ;

        if(sigma_x[v_star] > sigma_y[v_star] ) {
            set_x.insert(v_star) ;
            V.erase(v_star) ;
            for(vertex v : V)   calculate(sigma_y,set_x,v,v_star) ;
        }
        else {
            set_y.insert(v_star) ;
            V.erase(v_star) ;
            for(vertex v : V)   calculate(sigma_x,set_y,v,v_star) ;
        }

        sigma_x[v_star] = sigma_y[v_star] = 0 ;

        // for(vertex v : V) {
        //     (!set_one ? sigma_x[v] = accumulate(set_y,v) : sigma_y[v] = accumulate(set_x,v)) ;
        // }

    }

    for(vertex v = 0 ; v < nodes ; v++) {
        //std::cout << v <<'\n' ;
        sigma_y[v] = 0 ;
        for(auto u : set_x) calculate(sigma_y,set_x,v,u) ;

        sigma_x[v] = 0 ;
        for(auto u : set_y) calculate(sigma_x,set_y,v,u) ;
    }



    return cost() ;
}


std::pair<ll,ll> local_search() {
    bool change  = true ;
    ll iterations = 0 ;
    ll mx = INT_MIN ;
    ll s = 100 ;
    while(change) {
        change = false ;
        ll set_one = 0 ;
        vertex ch ;
        for(vertex v = 0 ; v < nodes && !change ; v++) {
            if(set_x.count(v) && sigma_y[v] > sigma_x[v]) {
                set_x.erase(v) ;
                set_y.insert(v) ;
                for(ll vert = 0 ; vert < nodes ; vert++) {
                    calculate(sigma_x,set_y,vert,v) ;
                    calculate(sigma_y,set_x,vert,v,-1.0) ;
                }

                change = true ;
            }
            else if(set_y.count(v) && sigma_x[v] > sigma_y[v]) {
                set_y.erase(v) ;
                set_x.insert(v) ;
                for(ll vert = 0 ; vert < nodes ; vert++) {
                    calculate(sigma_y,set_x,vert,v) ;
                    calculate(sigma_x,set_y,vert,v,-1.0) ;
                }


                change = true ;
            }
            //
        }
        //std::cout << change <<'\n' ;
        mx = std::max(cost(),mx) ;
        ++iterations ;

    }
    //std::cout << iterations <<'\n' ;
    return std::make_pair(iterations , mx) ;
}

ll randomized() {
    for(ll i = 0 ; i < nodes ; i++) {
        ll r = rand() ;
        r&1 ? set_x.insert(i) : set_y.insert(i) ;
    }

    return cost() ;
}

void GRASP() {

    ll randomized_cost = randomized() ;
    //std::cout << randomized_cost <<" Random\n" ;
    clear() ;
    ll greedy_cost = semi_greedy_max_cut(1) ;
    //std::cout << greedy_cost <<'\n' ;
    clear() ;

    ll semi_greedy_cost = semi_greedy_max_cut(0) ;
    //std::cout << semi_greedy_cost <<'\n' ;

    ll mx = 0 ;
    ll itr = 0 ;
    std::pair<ll,ll>local_srch_cost ;
    for(ll i = 0 ; i < 50; i++) {
        local_srch_cost = local_search() ;
        mx=std::max(mx,local_srch_cost.second) ;
        itr+=local_srch_cost.first ;

    }
    //std::cout << mx <<"\n" ;
    clear() ;

    ll best = INT_MIN ;
    
    for(ll i = 0 ; i < MAX_ITERATION ; i++) {
        semi_greedy_max_cut(0) ;
        local_srch_cost = local_search() ;
        clear() ;

        best = std::max(local_srch_cost.second , best) ;
    }
    std::cout << randomized_cost <<","<< greedy_cost << "," <<
        semi_greedy_cost << ","<< itr/50 <<"," << mx <<"," << MAX_ITERATION <<"," << best<< std::endl ;


}


int main() {
    srand(time(0));
    ll edges ;
    freopen("output.csv", "a", stdout) ;
    // std::cout << ",Problem,,,Constructive algorithm,,Local Search,,GRASP," << std::endl ;
    // std::cout << "Name,V,E,Simple Randomized,Simple Greedy,Semi Greedy,,Simple local,,GRASP" << std::endl ;
    // std::cout << ",,,,,,No. of iterations,Best value,No. of iterations,Best value" << std::endl ;
    for(ll z = 32 ; z <= 39 ;z++) {
        std::string name = "g"+std::to_string(z)+".rud" ;
        freopen(name.c_str(),"r",stdin) ;

        std::cin >> nodes >> edges ;

        for(ll i = 0 ; i < nodes ; i++) {
            for(ll j = 0 ; j < nodes ;j++) g[i][j] = INT_MAX ;
        }
        for(ll i = 0 ; i < edges ; i++) {
            ll u , v ;
            double c ;
            std::cin >> u >> v >> c ;
            u--;v--;
            g[u][v] = c ;
            g[v][u] = c ;
            edgelist.push_back(std::make_pair(u,v)) ;
            //std::cout <<"here\n" ;
        } 
        //std::cout <<"break" <<'\n' ;
        std::cout << "G"<< z <<"," << nodes <<"," << edges<<"," ;

        GRASP() ;

        clear() ;
    }
}
