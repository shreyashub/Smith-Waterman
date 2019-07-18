//
// Created by shreyas on 17/3/19.
//

#ifndef SW_MPI2_SW_UTIL_H
#define SW_MPI2_SW_UTIL_H
#include <vector>
#include <string>
#include <iostream>
using namespace std;

//#include "io_utils.h"
class SW_util {
public:

    int len_seq1,len_seq2;
    string seq1,seq2;
//    int **H, **bt;
    vector< vector<int> > H,bt;


    int tot_r,tot_c,tot_ant_diag;
    int n,m;
    int x,y;
    int m_lrgst_x,m_lrgst_y;
    int m_lrgst=0;

    SW_util(string s1, string s2);
//    ~SW_util();
    void getNextInd(int ant_diag_no, int ele_no, int *x, int *y);
    void alloc2D(vector<vector<int> > &vec, int r);
    int no_ele(int ant_diag_no);
    int no_preInit_ele(int ant_diag_no);

};


#endif //SW_MPI2_SW_UTIL_H
