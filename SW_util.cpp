//
// Created by shreyas on 17/3/19.
//

#include "SW_util.h"

SW_util :: SW_util(string s1,string s2){
    seq1=s1;
    seq2=s2;
    len_seq1 = seq1.length();
    len_seq2 = seq2.length();
    if(!seq1.empty() && !seq2.empty()){
        tot_r = len_seq1 + 1;
        tot_c = len_seq2 + 1;
        //Mem Alloc
        m = min(tot_r, tot_c);
        n = max(tot_r, tot_c);
        tot_ant_diag = m + n - 1;
        alloc2D(H, tot_ant_diag);
        alloc2D(bt, tot_ant_diag);
        //Initialization
        for (int i = 0; i < tot_r; ++i) {
            H[i][0] = 0;
            bt[i][0] = 0;
        }
        for (int i = 0; i < tot_c; ++i) {
            int last_ind = H[i].size() - 1;
            H[i][last_ind] = 0;
            bt[i][last_ind] = 0;
        }
    }

}
void SW_util :: getNextInd(int ant_diag_no,int ele_no,int *x,int *y){
    *x = (ant_diag_no < tot_r) ? ant_diag_no : tot_r-1;
    *y = (ant_diag_no < tot_r) ? 0 : (ant_diag_no - tot_r + 1);
    *x = *x - ele_no;
    *y = *y + ele_no;
}

int SW_util :: no_ele(int ant_diag_no){

    if(ant_diag_no < (m-1)) {
        return ant_diag_no+1;
    }
    else if(ant_diag_no < n){
        return  m;
    }
    else if(ant_diag_no < (m+n-1)){
        return m - (ant_diag_no+1-n);
    }
}
void SW_util :: alloc2D(vector<vector<int> > &vec, int r) {
    vec.resize(r);
    for (int i = 0; i < r; ++i) {
        vec[i] = vector<int>(no_ele(i));
    }
}
int SW_util ::no_preInit_ele(int ant_diag_no) {
    if(ant_diag_no<m)
        return 2;
    else if(ant_diag_no<n)
        return 1;
    else
        return 0;
}