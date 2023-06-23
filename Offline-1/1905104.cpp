//Bismillahir Rahmanir Rahim

#include<bits/stdc++.h>

int K ,zero_pos ;
int expanded = 0 ,explored = 0 ;
bool check(int r, int c) {

    if(r < 0 || c < 0 || r >= K || c >= K) return false ;
    return true ;
}

int inv_count(std::vector<int>&board) {
    int inv_cnt = 0 ;

    for (int i = 0; i < board.size() ; i++){
        for (int j = i + 1; j < board.size() ; j++){
            if (board[i] > board[j]) {
                inv_cnt++;
            }
        }
    }
    //std::cout << inv_cnt << '\n' ;
    return inv_cnt;

}

bool is_solvable(std::vector<int>&board) {
    int inv_cnt  = inv_count(board) ;

    if(K%2 && inv_cnt%2) {
        return false ;
    }

    if(!(K%2) && ((!(K - zero_pos)%2 && !(inv_cnt%2)) || ((K - zero_pos)%2 && inv_cnt%2))) {
        return false ;
    }

    return true ;
}

struct node {
    std::vector<std::vector<int>> board ;
    node* parent ;
    int move ;
    int priority ;

    //constructor
    node(std::vector<std::vector<int>> board , node* parent , int move , int priority) {
        this->board = board ;
        this->parent  = parent ;
        this->move = move ;
        this->priority = priority ;
    }
} ;

class Compare {
public:
    bool operator()(node*& node1, node*& node2) {
        return node1->priority > node2->priority ;
    }
};

    // bool operator < (node& node1 ,node& node2) {
    //     if (node1.priority == node2.priority) {
    //         return node1.move < node2.move ;
    //     }
    //     return node1.priority < node2.priority ;
    // }

    // //Overload the > operator.
    // bool operator > (const node& node1 ,const node& node2) {
    //     if (node1.priority == node2.priority) {
    //         return node1.move > node2.move ;
    //     }
    //     return node1.priority > node2.priority ;
    // }

int dx[4] = {-1,0,1,0} ;
int dy[4] = {0,1,0,-1} ;

std::vector<std::vector<int>>inp_matrix ;



int hamming_distance(std::vector<std::vector<int>>board) {
    int cnt = 0 ;

    for(int i = 0 ; i < K ; i++) {
        for(int j = 0 ; j < K ; j++) {
            if(i*K+j+1 == K*K) continue ;
            cnt+=(board[i][j] == i*K + j + 1) ? 0 : 1 ;
        }
    }
    //cout << cnt << endl ;
    return cnt ;
}


int manhattan_distance(std::vector<std::vector<int>>board) {
    int manhattan = 0 ;

    //int x  , y ;
    for(int i = 0 ; i < K ;i++) {
        for(int j = 0 ; j < K ; j++) {
            int val = board[i][j] ;
            if(val != 0) {
                manhattan += abs(((val-1)/K) - i) + abs(((val-1)%K) - j) ;
            }
        }
    }

    return manhattan ;
}

void print_matix(node* cur) {
    if(cur->parent) {
        print_matix(cur->parent) ;
    }
    for(int i = 0 ; i < K ; i++) {
        for(int j = 0 ; j < K ; j++) {
            std::cout << cur->board[i][j] <<" " ;
        }
        std:: cout << '\n' ;
    }
    std:: cout <<'\n' ;
}

void print(std::vector<std::vector<int>> board) {
    for(int i = 0 ; i < K ; i++) {
        for(int j = 0 ; j <K ; j++) {
            std::cout << board[i][j] <<" " ;
        }
        std::cout << std::endl;
    }
}

void A_Star(int (*heuristic[])(std::vector<std::vector<int>>),int h) {
    //cout <<"A_star" << endl;
    std::priority_queue<node*,std::vector<node*>,Compare > pq;
    std::map<std::vector<std::vector<int>>,bool>vis ;
    node *root =  new node(inp_matrix , nullptr , 0 , 0) ;
    std::queue<node*>delNode ;
    pq.push(root) ;
    explored++ ;
    while(!pq.empty()) {
        node* cur = pq.top() ;
        pq.pop() ;
        delNode.push(cur) ;

        //if already visited then don't go to further
        if(vis.find(cur->board) != vis.end()){
            continue ;
        }

        vis[cur->board] = true ;
        expanded++ ;

        if(!heuristic[h](cur->board)) {
            std::cout << "Number of moves : " << cur->move << std::endl;
            std::cout << "All moves are as follows : " << '\n' ;

            print_matix(cur) ;

            while(!pq.empty()) {
                node* temp = pq.top() ;
                pq.pop() ;
                delete temp ;
                //explored-- ;
            }

            while(!delNode.empty()){
                node* temp = delNode.front() ;
                delNode.pop() ;
                delete temp ;
                //explored-- ;
            }


            break;
        }

        for(int r = 0 ; r < K ; r++) {
            for(int c = 0 ; c < K ; c++) {
                if(cur->board[r][c] == 0) {
                    for(int i = 0 ; i < 4 ; i++) {
                        int n_r = r + dx[i] ;
                        int n_c = c + dy[i] ;

                        if(check(n_r,n_c)) {
                            std::vector<std::vector<int>> childboard = cur->board ;
                            std::swap(childboard[r][c] , childboard[n_r][n_c]) ;

                            if(vis.find(childboard) != vis.end()) {
                                continue ;
                            }
                            //std::cout << manhattan_distance(child->board) << endl;



                            pq.push(new node(childboard , cur ,cur->move+1,cur->move + heuristic[h](childboard))) ;
                            explored++ ;
                        }

                    }
                }
            }
        }

    }
}


int cast_to_num(std::string s) {
    std::stringstream ss(s) ;
    int x ; ss >> x ;
    return x ;
}

int main() {
    //freopen("output.txt", "w", stdout);
    std :: cin >> K ;
    inp_matrix= std::vector<std::vector<int>>(K,std::vector<int>(K,0)) ;
    for(int i = 0 ; i < K ; i++) {
        for(int j = 0 ; j < K ; j++) {
            std::string s ;
            std::cin >> s ;
            if(s == "*") {
                inp_matrix[i][j] = 0 ;
                zero_pos = i ;
                //std::cout << zero_pos << '\n' ;
            }

            else inp_matrix[i][j] = cast_to_num(s) ;
        }
    }

    std::vector<int> board1D ;

    for(int i = 0 ; i < K ; i++) {
        for(int j = 0 ; j < K ; j++) {
            if(inp_matrix[i][j])
                board1D.push_back(inp_matrix[i][j]) ;
        }
    }

    if(!is_solvable(board1D)) {
        std:: cout << "Unsolvable Puzzle" <<'\n' ;
        return 0 ;
    }
    //std::cout <<"LOL"<<'\n' ;

    int (*heuristic[])(std::vector<std::vector<int>>)={manhattan_distance,hamming_distance} ;

    A_Star(heuristic,0) ;

    std::cout << "expanded nodes : " << expanded << std::endl;
    std::cout << "explored nodes : " << explored << std::endl;


}
