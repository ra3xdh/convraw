#include <iostream>
#include "convraw.h"

int main(int argc, char **argv)
{
    if (argc<3) {
        std::cout<<"Usage: convraw infile outfile\n";
        exit(1);
    }

    std::vector< std::vector<double> > sim_points;
    std::vector<std::string> VarList;
    bool isComplex;

    char *infile = argv[1];
    char *ofile = argv[2];
    if (convraw::checkForSWP(infile)) {
        convraw::parseSTEPOutput(infile,sim_points,VarList,isComplex);
    } else {
        convraw::parseNgSpiceSimOutput(argv[1],sim_points,VarList,isComplex);
    }

    convraw::writeCSV(ofile,sim_points,VarList,isComplex);

    exit(0);
}
