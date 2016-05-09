###DESCRIPTION

`convraw` is converter of SPICE3f5 raw-Binary and raw-ASCII datasets to other dataset formats.
CSV only is supported. Use `cmake` to compile it.

`convraw` also provides an Octave extension to read SPICE-3f5 datasets. Octvae interface files 
are located in `octave/` subdirectory.  Use the following sequence to compile it:

~~~
cd octave
make
~~~ 

Then `read_spiceraw.oct` appears. Use `autoload()` function in Octave to install it. Use 
`read_spiceraw()` from Octave to read SPICE=3f5 files. 

###Usage

~~~
convraw input.dat output.csv
~~~

