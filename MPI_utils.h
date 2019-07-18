//
// Created by shreyas on 17/3/19.
//

#ifndef SW_MPI2_MPI_UTILS_H
#define SW_MPI2_MPI_UTILS_H

#include "mpi.h"
//#include "io_utils.h"
#include <iostream>
#include <fstream>
#include "SW_util.h"

class MPI_utils {
public:
    string fname1,fname2;
    string s1,s2;
    string ans1,ans2;
    char seq1[20],seq2[20];
    MPI_Status stat, stat2;
    int n_unfinished_jobs,n_workers;
    int n_proc,n_ad,n_r,n_c;
    SW_util *ob_ptr;
    int *shared_UP1,*shared_UP2,*shared_curr,*bt_shared_curr;
    vector<int> sc_inc, displs,displs2,sc_var,sc2_inc,sc2_var,r_displs,r_displs2;
    int rbuff_UP1[2],rbuff_UP2[1];
    int cont1,cont2,cont3;
    int ind_s1,ind_s2;
    int m_lrgst,m_lrgst_i, m_lrgst_j;
    int local_max[2],global_max[2];
    int i,j,x,y;
    int match_s= +3;
    int mismatch_s= -3;
    int gap_s= 2;

    void start(int argc, char** argv);
    void  test(int argc, char** argv);
    void initMeta(int n_proc);
    void findInd(int ant_diag_no,int ele_no,int *x,int *y,bool isString);
    int match_func(char a,char b);
    void printVec(SW_util *sw);
    void readSeq(string *fname,string *seq);
};


#endif //SW_MPI2_MPI_UTILS_H
