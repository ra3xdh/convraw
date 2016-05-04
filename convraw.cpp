#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "convraw.h"


namespace convraw {

    void extractBinSamples(std::ifstream &dbl, std::vector< std::vector<double> > &sim_points,
                                int NumPoints, int NumVars, bool isComplex);
    bool extractASCIISamples(std::string &lin, std::ifstream &ngsp_data,
                             std::vector< std::vector<double> > &sim_points,
                             int NumVars, bool isComplex);
}

std::string section(std::string s, std::string sep, int num);


void convraw::parseNgSpiceSimOutput(char *ngspice_file,
                                    std::vector < std::vector <double> > &sim_points,
                                    std::vector < std::string > &var_list, bool &isComplex)
{
    isComplex = false;
    bool isBinary = false;
    int NumPoints = 0;
    int bin_offset = 0;

    std::ifstream ngsp_data;
    std::ifstream ngsp_data_bin;
    ngsp_data.open(ngspice_file);
    ngsp_data_bin.open(ngspice_file,std::ios::binary);
    if (!ngsp_data.is_open()) return;
    if (!ngsp_data_bin.is_open()) return;

    sim_points.clear();
    bool start_values_sec = false;
    int NumVars=0; // Number of dep. and indep.variables
    std::string lin;
    while (std::getline(ngsp_data,lin)) { // Parse header;
        if (lin.empty()) continue;
        if ((lin.find("Flags")!=std::string::npos)
             &&(lin.find("complex")!=std::string::npos)) { // output consists of
            isComplex = true; // complex numbers
            continue;         // maybe ac_analysis
        }
        if (lin.find("No. Variables:")!=std::string::npos) {  // get number of variables
            NumVars=std::stoi(section(lin," ",2));
            continue;
        }
        if (lin.find("No. Points:")!=std::string::npos) {  // get number of variables
            NumPoints=std::stoi(section(lin," ",2));
            continue;
        }
        if (lin=="Variables:") {
            var_list.clear();
            std::getline(ngsp_data,lin);
            std::string indep_var = section(lin,"\t",1);
            var_list.push_back(indep_var);

            for (int i=1;i<NumVars;i++) {
                std::getline(ngsp_data,lin);
                std::string dep_var = section(lin,"\t",1);
                var_list.push_back(dep_var);
            }
            continue;
        }
        if (lin=="Values:") {
            start_values_sec = true;
            continue;
        }
        if (lin=="Binary:") {
            isBinary = true;
            bin_offset = ngsp_data.tellg();
        }

        if (isBinary) {
            ngsp_data_bin.seekg(bin_offset);
            extractBinSamples(ngsp_data_bin, sim_points, NumPoints, NumVars, isComplex);
            break;
        }

        if (start_values_sec) {
            extractASCIISamples(lin,ngsp_data,sim_points,NumVars,isComplex);
        }
    }

    ngsp_data.close();
    ngsp_data_bin.close();
}

void convraw::parseSTEPOutput(char *ngspice_file,
                     std::vector< std::vector<double> > &sim_points,
                     std::vector<std::string> &var_list, bool &isComplex)
{
    isComplex = false;
    bool isBinary = false;
    int bin_offset = 0;

    std::ifstream ngsp_data;
    std::ifstream ngsp_data_bin;
    ngsp_data.open(ngspice_file);
    ngsp_data_bin.open(ngspice_file,std::ios::binary);
    if (!ngsp_data.is_open()) return;
    if (!ngsp_data_bin.is_open()) return;

    sim_points.clear();
    bool start_values_sec = false;
    bool header_parsed = false;
    int NumVars=0; // Number of dep. and indep.variables
    int NumPoints=0; // Number of simulation points
    std::string lin;
    while (std::getline(ngsp_data,lin)) {
        if (lin.empty()) continue;
        if ((lin.find("Plotname:")!=std::string::npos)&&  // skip operating point
            (lin.find("DC operating point")!=std::string::npos)) {
            while (lin.find("Plotname:")==std::string::npos) getline(ngsp_data,lin);
        }
        if (!header_parsed) {
            if ((lin.find("Flags")!=std::string::npos)
                 &&(lin.find("complex")!=std::string::npos)) { // output consists of
                isComplex = true; // complex numbers
                continue;         // maybe ac_analysis
            }
            if (lin.find("No. Variables:")!=std::string::npos) {  // get number of variables
                NumVars=std::stoi(section(lin," ",2));
                continue;
            }

            if (lin=="Variables:") {
                var_list.clear();
                std::getline(ngsp_data,lin);
                std::string indep_var = section(lin,"\t",1);
                var_list.push_back(indep_var);

                for (int i=1;i<NumVars;i++) {
                    std::getline(ngsp_data,lin);
                    std::string dep_var = section(lin,"\t",1);
                    var_list.push_back(dep_var);
                }
                header_parsed = true;
                continue;
            }
        }

        if (lin.find("No. Points:")!=std::string::npos) {  // get number of variables
            NumPoints=std::stoi(section(lin," ",2));
            continue;
        }

        if (lin=="Values:") {
            start_values_sec = true;
            continue;
        }
        if (lin=="Binary:") {
            isBinary = true;
            bin_offset = ngsp_data.tellg();
        }

        if (isBinary) {
            ngsp_data_bin.seekg(bin_offset);
            extractBinSamples(ngsp_data_bin, sim_points, NumPoints, NumVars, isComplex);
            int pos = ngsp_data_bin.tellg();
            ngsp_data.seekg(pos);
            isBinary = false;
            continue;
        }

        if (start_values_sec) {
            if (extractASCIISamples(lin,ngsp_data,sim_points,NumVars,isComplex)) {
                NumPoints--;
                if (NumPoints==0) start_values_sec = false;
            } else continue;
        }

    }
    ngsp_data.close();
    ngsp_data_bin.close();
}


void convraw::extractBinSamples(std::ifstream &dbl, std::vector< std::vector<double> > &sim_points,
                                int NumPoints, int NumVars, bool isComplex)
{
    int cnt = NumPoints;
    while (cnt>0) {
        std::vector<double> sim_point;
        double re,im;
        dbl.read((char *)&re,sizeof(double)); // Indep. variable
        sim_point.push_back(re);
        if (isComplex) dbl.read((char *)&im,sizeof(double)); // drop Im part of indep.var
        for (int i=1;i<NumVars;i++) { // first variable is independent
            if (isComplex) {
                dbl.read((char *)&re,sizeof(double)); // Re
                dbl.read((char *)&im,sizeof(double)); // Im
                sim_point.push_back(re);
                sim_point.push_back(im);
            } else {
                dbl.read((char *)&re,sizeof(double));
                sim_point.push_back(re); // Re
            }
        }
        sim_points.push_back(sim_point);
        cnt--;
    }
}

bool convraw::extractASCIISamples(std::string &lin, std::ifstream &ngsp_data,
                                  std::vector< std::vector<double> > &sim_points,
                                  int NumVars, bool isComplex)
{
    std::vector<double> sim_point;
    /*bool ok = false;
    QRegExp dataline_patter("^ *[0-9]+[ \t]+.*"); // ???
    if (!dataline_patter.exactMatch(lin)) return false;*/
    if (lin.empty()) return false;
    double indep_val = std::stod(section(lin,"\t",1)); // only real
    sim_point.push_back(indep_val);
    for (int i=1;i<NumVars;i++) {
        std::getline(ngsp_data,lin);
        if (isComplex) {
            double re_dep_val = std::stod(section(lin,",",0));  // for complex sim results
            double im_dep_val = std::stod(section(lin,",",1));  // imaginary part follows
            sim_point.push_back(re_dep_val);              // real part
            sim_point.push_back(im_dep_val);
        } else {
            double dep_val = std::stod(lin);
            sim_point.push_back(dep_val);
        }
    }
    sim_points.push_back(sim_point);
    return true;
}




bool convraw::checkForSWP(char *spice_file)
{
    std::ifstream sp_file;
    std::string line;
    int cnt=0;

    sp_file.open(spice_file);
    if (!sp_file.is_open()) return false;
    while(getline(sp_file,line)) {
        if (line.find("Plotname:")!=std::string::npos) cnt++;
    }
    if (cnt>1) return true;
    else return false;
}

std::string section(std::string s, std::string sep, int num)
{
    size_t pos = 0;
    int cnt = 0;
    std::string token;
    std::string ss = s;
    std::string res;
    res.clear();
    pos = ss.find_first_not_of(sep);
    ss.erase(0,pos);
    while (!ss.empty()) {
        pos = ss.find(sep);
        token = ss.substr(0, pos);
        if (cnt==num) {
            res = token;
            break;
        }
        ss.erase(0, pos + sep.length());
        cnt++;
    }
    return res;
}
