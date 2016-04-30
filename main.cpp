#include <iostream>
#include "raw2any.h"

int main(int argc, char **argv)
{
    if (argc<3) {
        std::cout<<"Usage: raw2any infile outfile\n";
        exit(1);
    }

    QList< QList<double> > sim_points;
    QStringList VarList;
    bool isComplex;

    raw2any::parseNgSpiceSimOutput(argv[1],sim_points,VarList,isComplex);
    raw2any::writeCSV(argv[2],sim_points,VarList,isComplex);

    exit(0);
}
