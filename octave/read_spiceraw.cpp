#include <octave/oct.h>


DEFUN_DLD (read_spiceraw, args, nargout,
	"Usage: read_spiceraw('spice_data.dat')")
{
    octave_value_list retval;

    int nargin =args.length();
    if (nargin!=1) {
	print_usage();
    } else {
	bool isComplex = false;
	std::vector< std::vector <double> > sim_points;
	Matrix SimRes;
	ComplexMatrix CSimRes;
	if (!error_state) {
	    if (isComplex) retval(0) = octave_value(CSimRes);
	    else retval(0) = octave_value(SimRes);
	}
    }
    return retval;
}
