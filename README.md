### DESCRIPTION

`convraw` is converter of SPICE3f5 raw-Binary and raw-ASCII datasets to other dataset formats.
CSV only is supported. Use `cmake` to compile it.

### Octave package

`convraw` also provides an Octave extension to read SPICE-3f5 datasets. Octave interface files 
are located in `octave/` subdirectory. 

Execute `octave_makepkg.sh` script to create an Octave package. It will create `spiceraw-0.0.1.tar.gz` 
package. Then run `octave`  and execute:
~~~
pkg install spiceraw-0.0.1.tar.gz
~~~
to install it into Octave. Use `pkg load spiceraw` to use `read_spiceraw()` function that reads 
SPICE datasets.


You may also compile and run an Octave package without installation. Use the following sequence 
to compile it:

~~~
cd octave
make
~~~ 

Then `read_spiceraw.oct` appears. Use `autoload()` function in Octave to install it. Use 
`read_spiceraw()` from Octave to read SPICE=3f5 files. 

### Usage

~~~
convraw input.dat output.csv
~~~

