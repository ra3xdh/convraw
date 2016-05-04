#include <iostream>
#include <fstream>
#include <string>

#include "fileformats.h"

void convraw::writeCSV(char *csv_file, std::vector< std::vector<double> > &sim_points,
                       std::vector<std::string> &var_list, bool isComplex)
{
    std::ofstream csv;
    csv.open(csv_file);
    std::string s;
    int NVars = var_list.size();
    int NPoints = sim_points.size();
    for(std::vector<std::string>::iterator it = var_list.begin();it != var_list.end(); it++) {
        s += (*it) + ";";
    }
    s.erase(s.end()-1);

    csv<<s<<std::endl;
    for(int i=0;i<NPoints;i++) {
        std::string ss;
        ss = std::to_string(sim_points[i][0]) + ";"; // indep.var
        for (int j=0;j<NVars-1;j++) {
            if (isComplex) {
                std::string re = std::to_string(sim_points[i][2*j+1]);
                double im_val = sim_points[i][2*j+2];
                std::string im = std::to_string(im_val);
                ss += re;
                if (im_val>=0) ss += "+";
                ss += im + "i" + ";";
            } else {
                std::string re = std::to_string(sim_points[i][j+1]);
                ss += re + ";";
            }
        }
        ss.erase(ss.end()-1);
        csv<<ss<<std::endl;
    }

    csv.close();
}
