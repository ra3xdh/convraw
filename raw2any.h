#ifndef RAW2ANY_H

#include <QtCore>

namespace raw2any {

void parseNgSpiceSimOutput(QString ngspice_file,
                           QList< QList<double> > &sim_points,
                           QStringList &var_list, bool &isComplex);

void parseSTEPOutput(QString ngspice_file,
                     QList< QList<double> > &sim_points,
                     QStringList &var_list, bool &isComplex);

void writeCSV(char *csv_file,  QList< QList<double> > &sim_points,
              QStringList &var_list, bool isComplex);

}



#define RAW2ANY_H

#endif // RAW2ANY_H
