//
// Created by shreyas on 17/3/19.
//

#ifndef SW_MPI2_IO_UTILS_H
#define SW_MPI2_IO_UTILS_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void readSeq(string *fname,string *seq){
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
void writeSeq(string *fname,string *seq){
    int size=(*seq).length();
    ofstream file(fname->c_str());
    if(!file.is_open()){
        cout<<"Cant Open OutFile"<<endl;
        return;
    }
    file.write ((*seq).c_str(), size);
    file.close();
}

#endif //SW_MPI2_IO_UTILS_H
