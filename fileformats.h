#ifndef FILEFORMATS_H
#define FILEFORMATS_H

#include <vector>

namespace convraw {

void writeCSV(char *csv_file,  std::vector< std::vector<double> > &sim_points,
              std::vector<std::string> &var_list, bool isComplex);
}


#endif // FILEFORMATS_H
