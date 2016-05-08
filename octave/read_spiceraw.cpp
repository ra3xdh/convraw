#include <octave/oct.h>

#include "../convraw.cpp"

namespace convraw {
    void fillSimResMatrix(Matrix &res,std::vector< std::vector<double> > &sim_points);
    void fillSimResCMatrix(ComplexMatrix &res,std::vector< std::vector<double> > &sim_points,int NumVars);
}

DEFUN_DLD (read_spiceraw, args, nargout,
	"-- Read SPICE-3f5 raw dataset:  DATA = read_spiceraw(FILE)")
{
    octave_value_list retval;

    int nargin =args.length();
    if (nargin!=1) {
	print_usage();
    } else {
	bool isComplex = false;
	std::vector< std::vector <double> > sim_points;
	std::vector< std::string > VarList;

	std::string infile = args(0).string_value();

	convraw::extractNumDataFromSPICE(infile.c_str(),sim_points,VarList,isComplex);
	int NumVars = VarList.size();

        int r_sz = sim_points.size();
        dim_vector dv(r_sz,NumVars);
	Matrix SimRes(dv);
	ComplexMatrix CSimRes(dv);
    

	if (isComplex) convraw::fillSimResCMatrix(CSimRes,sim_points,NumVars);
	else convraw::fillSimResMatrix(SimRes,sim_points);

	if (!error_state) {
	    if (isComplex) retval(0) = octave_value(CSimRes);
	    else retval(0) = octave_value(SimRes);
	}
    }
    return retval;
}



void convraw::fillSimResMatrix(Matrix &res,std::vector< std::vector<double> > &sim_points)
{
    int c_sz = 0;
    int r_sz = sim_points.size();
    if (!sim_points.empty()) c_sz=sim_points[0].size();
    for (int i=0;i<r_sz;i++) {
	for (int j=0;j<c_sz;j++) 
	    res(i,j) = sim_points[i][j];
    }
}

void convraw::fillSimResCMatrix(ComplexMatrix &res,std::vector< std::vector<double> > &sim_points, int NumVars) 
{
    int r_sz = sim_points.size();
    for (int i=0;i<r_sz;i++) {
	for (int j=0;j<NumVars;j++) {
	    double re = sim_points[i][2*j];
	    double im = sim_points[i][2*j+1];
	    res(i,j)=Complex(re,im);
	}
    }
}

