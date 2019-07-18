//
// Created by shreyas on 17/3/19.
//

#include <algorithm>
#include "MPI_utils.h"
void MPI_utils ::initMeta(int n_proc) {
    sc_inc.resize(n_proc);
    sc2_inc.resize(n_proc);
    displs.resize(n_proc); //0,1,2...
    displs2.resize(n_proc); //,1,2,3...
    sc_var.resize(n_proc);//0 | 1
    sc2_var.resize(n_proc);// 0 | 2
    r_displs.resize(n_proc);
    r_displs2.resize(n_proc);
    sc_inc[0]=1;
    sc2_inc[0]=2;
    for (int i = 0; i < n_proc; ++i) {
        displs[i]=i;
        displs2[i]=i+1;
        r_displs[i]=i+1;
        r_displs2[i]=i;
        sc_inc[i]=0;
        sc2_inc[i]=0;
    }
}
void MPI_utils :: start(int argc, char** argv) {
    MPI_Init ( &argc, &argv );
    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    MPI_Comm_size ( MPI_COMM_WORLD, &n_proc );
    int rank;
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    initMeta(n_proc);
    m_lrgst=-1;
    s1="";
    s2="";
    if(rank==0) {
//        string f1="in1.txt",f2="in2.txt";
//        readSeq(&f1,&s1);
//        readSeq(&f2,&s2);
        getline(cin,s1);
        getline(cin,s2);
//        ob_ptr = new SW_util(s1,s2);
        strcpy(seq1,s1.c_str());
        strcpy(seq2,s2.c_str());
    }
    SW_util sw(s1,s2);
    n_ad=sw.tot_ant_diag;
    n_r=sw.tot_r;
    n_c=sw.tot_c;
    MPI_Bcast(&n_ad,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&n_r,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&n_c,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(seq1,n_r-1,MPI_CHAR,0,MPI_COMM_WORLD);
    MPI_Bcast(seq2,n_c-1,MPI_CHAR,0,MPI_COMM_WORLD);
    for (int i = 2; i < n_ad; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank==0) {
            n_unfinished_jobs = sw.no_ele(i)-sw.no_preInit_ele(i);
        }
        MPI_Bcast(&n_unfinished_jobs,1,MPI_INT,0,MPI_COMM_WORLD);
        if(n_unfinished_jobs!=0){
            if(rank==0){
                shared_UP1=sw.H[i-1].data();
                shared_UP2=sw.H[i-2].data();
                shared_curr = sw.H[i].data();
                bt_shared_curr = sw.bt[i].data();

            }

            int lrgst=0,ptr=0,curr_max,ind=0,lrgst_rank;
            if(n_unfinished_jobs<n_proc){
                if(i<=max(n_r,n_c)){
                    sc_inc[n_unfinished_jobs-1]=1;
                    sc2_inc[n_unfinished_jobs-1]=2;
                }
                else{
                    sc_inc[n_unfinished_jobs]=0;
                    sc2_inc[n_unfinished_jobs]=0;
                }
                if(i<n_r+1){
                    MPI_Scatterv(shared_UP1,&sc2_inc[0],&displs[0],MPI_INT,&rbuff_UP1[0],sc2_inc[rank],MPI_INT,0,MPI_COMM_WORLD);
                    MPI_Scatterv(shared_UP2,&sc_inc[0],&displs[0],MPI_INT,&rbuff_UP2[0],sc_inc[rank],MPI_INT,0,MPI_COMM_WORLD);
                }
                else{
                    MPI_Scatterv(shared_UP1,&sc2_inc[0],&displs[0],MPI_INT,&rbuff_UP1[0],sc2_inc[rank],MPI_INT,0,MPI_COMM_WORLD);
                    MPI_Scatterv(shared_UP2,&sc_inc[0],&displs2[0],MPI_INT,&rbuff_UP2[0],sc_inc[rank],MPI_INT,0,MPI_COMM_WORLD);
                }
                MPI_Barrier(MPI_COMM_WORLD);
                if(rank<n_unfinished_jobs) {

                        if((i<n_r))
                            ind = rank+1;
                        else
                            ind = rank;
                        findInd(i,ind,&ind_s1,&ind_s2,true);
                        cont1=rbuff_UP2[0] + match_func(seq1[ind_s1],seq2[ind_s2]);
                        cont2=rbuff_UP1[1] - gap_s;
                        cont3=rbuff_UP1[0] - gap_s;
                        lrgst= max(max(0,cont3),max(cont1,cont2));

                        if(lrgst==cont1)
                            ptr=1;
                        else if(lrgst==cont2)
                            ptr=2;
                        else if(lrgst==cont3)
                            ptr=3;

                    }
                if(i<n_r){
                    MPI_Gatherv(&lrgst,sc_inc[rank],MPI_INT,&shared_curr[0],&sc_inc[0],&r_displs[0],MPI_INT,0,MPI_COMM_WORLD);
                    MPI_Gatherv(&ptr,sc_inc[rank],MPI_INT,&bt_shared_curr[0],&sc_inc[0],&r_displs[0],MPI_INT,0,MPI_COMM_WORLD);
                }
                else{
                    MPI_Gatherv(&lrgst,sc_inc[rank],MPI_INT,&shared_curr[0],&sc_inc[0],&r_displs2[0],MPI_INT,0,MPI_COMM_WORLD);
                    MPI_Gatherv(&ptr,sc_inc[rank],MPI_INT,&bt_shared_curr[0],&sc_inc[0],&r_displs2[0],MPI_INT,0,MPI_COMM_WORLD);
                }
            }
            else{
                if(rank==0)
                    cout<<"Insufficient number of processes, Partial Result"<<endl;
                break;
            }
            local_max[0]=lrgst;
            local_max[1]=rank;
            MPI_Reduce(local_max,global_max,1,MPI_2INT,MPI_MAXLOC,0,MPI_COMM_WORLD);
            if(rank==0){
                if(m_lrgst<global_max[0]){
                    m_lrgst=global_max[0];
                    if(i<n_r){
                        global_max[1]++;
                    }
                    m_lrgst_i=i;
                    m_lrgst_j=global_max[1];
                }
            }
        }
    }
    if(rank==0){
        i=m_lrgst_i;
        j=m_lrgst_j;
        while(1){
            if(sw.H[i][j]==0)
                break;
            switch (sw.bt[i][j]){
                case 1:
                    findInd(i,j,&x,&y, true);
                    ans1+=seq1[x];
                    ans2+=seq2[y];
                    if(i<n_r)
                        j=j-1;
                    else
                        j=j+1;
                    i=i-2;
                    break;
                case 2:
                    findInd(i,j,&x,&y, true);
                    ans1+=seq1[x];
                    ans2+='_';
                    if(i<n_r)
                        ;
                    else
                        j=j+1;
                    i=i-1;
                    break;
                case 3:
                    findInd(i,j,&x,&y, true);
                    ans1+='_';
                    ans2+=seq2[y];
                    if(i<n_r)
                        j=j-1;
                    else
                        ;
                    i=i-1;
                    break;
                default: {
                    printf("Error\n");
                    exit(0);
                }
            }
        }
        reverse(ans1.begin(), ans1.end());
        reverse(ans2.begin(), ans2.end());
        cout<<"Aligned Result"<<endl<<ans1<<endl<<ans2<<endl;
    }
    MPI_Finalize();
}

void MPI_utils ::test(int argc, char** argv) {
    MPI_Init ( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &n_proc );
//    SW_util *ob;
    int rank;
    vector <int> *pt;
    int *pt2;
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    vector<int > data(20),recvb(10),sc(5),disp(5);
    int recc[10];
    int val;
    if(rank==0) {
        for (int i = 0; i <3; ++i) {
            data[i] = i;
        }
        pt = &data;
        pt2= &data[0];
    }
    for (int i = 0; i < 5; ++i) {
//        sc[i]=i+1;
        if(i<3)
            sc[i]=1;
        else
            sc[i]=0;
        disp[i]=i;
        if(rank==0){
            val=10+i;
        }
        MPI_Bcast(&val,1,MPI_INT,0,MPI_COMM_WORLD);
//        cout<<val<<"@"<<rank<<endl;
        MPI_Barrier(MPI_COMM_WORLD);

    }
    MPI_Scatterv(pt2,&sc[0],&disp[0],MPI_INT,&recvb[0],sc[rank],MPI_INT,0,MPI_COMM_WORLD);
    printf("%d recvd in rank %d\n",recvb[0],rank);
    MPI_Finalize();
}


void MPI_utils::findInd(int ant_diag_no,int ele_no,int *x,int *y, bool isString) {
//    int tot_r = seq1_len + 1;
    int tot_r = n_r;
    *x = (ant_diag_no < tot_r) ? ant_diag_no : tot_r-1;
    *y = (ant_diag_no < tot_r) ? 0 : (ant_diag_no - tot_r + 1);
    if(isString== true) {
        *x = (*x - ele_no) - 1;
        *y = (*y + ele_no) - 1;
    }
    else {
        *x = (*x - ele_no) ;
        *y = (*y + ele_no) ;
    }
}

int MPI_utils::match_func(char a,char b){
    if(a==b)
        return match_s;
    else
        return mismatch_s;
}

void MPI_utils::printVec(SW_util *sw) {
    for (int i = 0; i < sw->H.size(); ++i) {
        for (int j = 0; j < sw->H[i].size(); ++j) {
            cout<<sw->H[i][j]<<" "<<endl;
        }
        cout<<endl;
    }

}

void MPI_utils::readSeq(string *fname, string *seq) {
    streampos size;
    char * buf;
    ifstream file(fname->c_str(), ios::in|ios::binary|ios::ate);
    if(!file.is_open()){
        cout<<"Cant Open InFile"<<endl;
        return;
    }
    size=file.tellg();
    buf = new char [size];
    file.seekg (0, ios::beg);
    file.read (buf, size);
    file.close();
    *seq= *seq+buf;
    delete[](buf);
}
