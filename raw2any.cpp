#include <iostream>
#include <fstream>

#include "raw2any.h"


namespace raw2any {

    void extractBinSamples(QDataStream &dbl, QList<QList<double> > &sim_points,
                                int NumPoints, int NumVars, bool isComplex);
    bool extractASCIISamples(QString &lin, QTextStream &ngsp_data,
                                  QList<QList<double> > &sim_points, int NumVars, bool isComplex);
}


void raw2any::parseNgSpiceSimOutput(QString ngspice_file,
                                    QList< QList<double> > &sim_points,
                                    QStringList &var_list, bool &isComplex)
{
    isComplex = false;
    bool isBinary = false;
    int NumPoints = 0;
    int bin_offset = 0;
    QByteArray content;

    QFile ofile(ngspice_file);
    if (ofile.open(QFile::ReadOnly)) {
        //QTextStream ts(&ofile);
        content = ofile.readAll();
        ofile.close();
    }

    QTextStream ngsp_data(&content);
    sim_points.clear();
    bool start_values_sec = false;
    int NumVars=0; // Number of dep. and indep.variables
    while (!ngsp_data.atEnd()) { // Parse header;
        QRegExp sep("[ \t,]");
        QString lin = ngsp_data.readLine();
        if (lin.isEmpty()) continue;
        if (lin.contains("Flags")&&lin.contains("complex")) { // output consists of
            isComplex = true; // complex numbers
            continue;         // maybe ac_analysis
        }
        if (lin.contains("No. Variables")) {  // get number of variables
            NumVars=lin.section(sep,2,2,QString::SectionSkipEmpty).toInt();
            continue;
        }
        if (lin.contains("No. Points:")) {  // get number of variables
            NumPoints=lin.section(sep,2,2,QString::SectionSkipEmpty).toInt();
            continue;
        }
        if (lin=="Variables:") {
            var_list.clear();
            QString indep_var = ngsp_data.readLine().section(sep,1,1,QString::SectionSkipEmpty);
            var_list.append(indep_var);

            for (int i=1;i<NumVars;i++) {
                lin = ngsp_data.readLine();
                QString dep_var = lin.section(sep,1,1,QString::SectionSkipEmpty);
                var_list.append(dep_var);
            }
            continue;
        }
        if (lin=="Values:") {
            start_values_sec = true;
            continue;
        }
        if (lin=="Binary:") {
            isBinary = true;
            bin_offset = ngsp_data.pos();
        }

        if (isBinary) {
            QDataStream dbl(content);
            dbl.setByteOrder(QDataStream::LittleEndian);
            dbl.device()->seek(bin_offset);
            extractBinSamples(dbl, sim_points, NumPoints, NumVars, isComplex);
            break;
        }

        if (start_values_sec) {
            extractASCIISamples(lin,ngsp_data,sim_points,NumVars,isComplex);
        }
    }
}

void raw2any::parseSTEPOutput(QString ngspice_file,
                     QList< QList<double> > &sim_points,
                     QStringList &var_list, bool &isComplex)
{
    isComplex = false;
    bool isBinary = false;
    int bin_offset = 0;
    QByteArray content;

    QFile ofile(ngspice_file);
    if (ofile.open(QFile::ReadOnly)) {
        content = ofile.readAll();
        ofile.close();
    }

    QTextStream ngsp_data(&content);
    sim_points.clear();
    bool start_values_sec = false;
    bool header_parsed = false;
    int NumVars=0; // Number of dep. and indep.variables
    int NumPoints=0; // Number of simulation points
    while (!ngsp_data.atEnd()) {
        QRegExp sep("[ \t,]");
        QString lin = ngsp_data.readLine();
        if (lin.isEmpty()) continue;
        if (lin.contains("Plotname:")&&  // skip operating point
            (lin.contains("DC operating point"))) {
            for(bool t = false; !t; t = (ngsp_data.readLine().startsWith("Plotname:")));
        }
        if (!header_parsed) {
            if (lin.contains("Flags")&&lin.contains("complex")) { // output consists of
                isComplex = true; // complex numbers
                continue;         // maybe ac_analysis
            }
            if (lin.contains("No. Variables")) {  // get number of variables
                NumVars=lin.section(sep,2,2,QString::SectionSkipEmpty).toInt();
                continue;
            }
            if (lin.contains("No. Points:")) {  // get number of variables
                NumPoints=lin.section(sep,2,2,QString::SectionSkipEmpty).toInt();
                continue;
            }
            if (lin=="Variables:") {
                var_list.clear();
                QString indep_var = ngsp_data.readLine().section(sep,1,1,QString::SectionSkipEmpty);
                var_list.append(indep_var);

                for (int i=1;i<NumVars;i++) {
                    lin = ngsp_data.readLine();
                    QString dep_var = lin.section(sep,1,1,QString::SectionSkipEmpty);
                    var_list.append(dep_var);
                }
                header_parsed = true;
                continue;
            }
        }

        if (lin=="Values:") {
            start_values_sec = true;
            continue;
        }
        if (lin=="Binary:") {
            isBinary = true;
            bin_offset = ngsp_data.pos();
        }

        if (isBinary) {
            QDataStream dbl(content);
            dbl.setByteOrder(QDataStream::LittleEndian);
            dbl.device()->seek(bin_offset);
            extractBinSamples(dbl,sim_points,NumPoints,NumVars,isComplex);
            int pos = dbl.device()->pos();
            ngsp_data.seek(pos);
            isBinary = false;
            continue;
        }


        if (start_values_sec) {
            if (!extractASCIISamples(lin,ngsp_data,sim_points,NumVars,isComplex)) continue;
        }

    }
}


void raw2any::extractBinSamples(QDataStream &dbl, QList<QList<double> > &sim_points,
                                int NumPoints, int NumVars, bool isComplex)
{
    int cnt = NumPoints;
    while (cnt>0) {
        QList<double> sim_point;
        double re,im;
        dbl>>re; // Indep. variable
        sim_point.append(re);
        if (isComplex) dbl>>im; // drop Im part of indep.var
        for (int i=1;i<NumVars;i++) { // first variable is independent
            if (isComplex) {
                dbl>>re; // Re
                dbl>>im; // Im
                sim_point.append(re);
                sim_point.append(im);
            } else {
                dbl>>re;
                sim_point.append(re); // Re
            }
        }
        sim_points.append(sim_point);
        cnt--;
    }
}

bool raw2any::extractASCIISamples(QString &lin, QTextStream &ngsp_data,
                                  QList<QList<double> > &sim_points, int NumVars, bool isComplex)
{
    QRegExp sep("[ \t,]");
    QList<double> sim_point;
    bool ok = false;
    QRegExp dataline_patter("^ *[0-9]+[ \t]+.*");
    if (!dataline_patter.exactMatch(lin)) return false;
    double indep_val = lin.section(sep,1,1,QString::SectionSkipEmpty).toDouble(&ok);
    //double indep_val = lin.split(sep,QString::SkipEmptyParts).at(1).toDouble(&ok); // only real indep vars
    if (!ok) return false;
    sim_point.append(indep_val);
    for (int i=0;i<NumVars;i++) {
        if (isComplex) {
            QStringList lst = ngsp_data.readLine().split(sep,QString::SkipEmptyParts);
            if (lst.count()==2) {
                double re_dep_val = lst.at(0).toDouble();  // for complex sim results
                double im_dep_val = lst.at(1).toDouble();  // imaginary part follows
                sim_point.append(re_dep_val);              // real part
                sim_point.append(im_dep_val);
            }
        } else {
            double dep_val = ngsp_data.readLine().remove(sep).toDouble();
            sim_point.append(dep_val);
        }
    }
    sim_points.append(sim_point);
    return true;
}


void raw2any::writeCSV(char *csv_file, QList<QList<double> > &sim_points,
                       QStringList &var_list, bool isComplex)
{
    std::ofstream csv;
    csv.open(csv_file);
    std::string s;
    int NVars = var_list.count();
    int NPoints = sim_points.count();
    for(QStringList::iterator it = var_list.begin();it != var_list.end(); it++) {
        s += (*it).toStdString() + ";";
    }
    s.erase(s.end()-1);

    csv<<s<<std::endl;
    for(int i=0;i<NPoints;i++) {
        std::string ss;
        ss = std::to_string(sim_points[i][0]) + ";"; // indep.var
        for (int j=0;j<NVars-1;j++) {
            if (isComplex) {
                std::string re = std::to_string(sim_points[i][2*j+1]);
                std::string im = std::to_string(sim_points[i][2*j+2]);
                ss += re + ";" + im + ";";
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
