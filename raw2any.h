#ifndef RAW2ANY_H

#include <QtCore>

namespace raw2any {

void parseNgSpiceSimOutput(char *ngspice_file,
                           std::vector< std::vector<double> > &sim_points,
                           std::vector<std::string> &var_list, bool &isComplex);

void parseSTEPOutput(char *ngspice_file,
                     std::vector< std::vector<double> > &sim_points,
                     std::vector< std::string> &var_list, bool &isComplex);

void writeCSV(char *csv_file,  std::vector< std::vector<double> > &sim_points,
              std::vector<std::string> &var_list, bool isComplex);

bool checkForSWP(char *spice_file);

}



#define RAW2ANY_H

#endif // RAW2ANY_H
