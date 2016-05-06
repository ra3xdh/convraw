#ifndef RAW2ANY_H

#include <string>
#include <vector>

namespace convraw {

void parseNgSpiceSimOutput(const char *ngspice_file,
                           std::vector< std::vector<double> > &sim_points,
                           std::vector<std::string> &var_list, bool &isComplex);

void parseSTEPOutput(const char *ngspice_file,
                     std::vector< std::vector<double> > &sim_points,
                     std::vector< std::string> &var_list, bool &isComplex);

bool checkForSWP(const char *spice_file);

void extractNumDataFromSPICE(const char *infile,
                        std::vector< std::vector<double> > &sim_points,
                        std::vector<std::string> &var_list, bool &isComplex);

}



#define RAW2ANY_H

#endif // RAW2ANY_H
