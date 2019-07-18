//#include <iostream>
#include <stdio.h>
//#include "MPI_utils.h"
#include <vector>
using namespace std;
#include "MPI_utils.h"
int main(int argc, char** argv) {
    MPI_utils ob;
    ob.start(argc,argv);
    return 0;
}