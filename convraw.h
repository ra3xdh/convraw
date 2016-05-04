#ifndef RAW2ANY_H

#include <string>
#include <vector>

namespace convraw {

void parseNgSpiceSimOutput(char *ngspice_file,
                           std::vector< std::vector<double> > &sim_points,
                           std::vector<std::string> &var_list, bool &isComplex);

void parseSTEPOutput(char *ngspice_file,
                     std::vector< std::vector<double> > &sim_points,
                     std::vector< std::string> &var_list, bool &isComplex);

bool checkForSWP(char *spice_file);

}



#define RAW2ANY_H

#endif // RAW2ANY_H
