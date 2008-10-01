COMPHOM USER MANUAL

This is small README for the comphom program. Since the program cannot
do very much, there is almost nothing that can go wrong, so don't be
afraid.

1. How do I install comphom?

Extract the archive into a folder of your choice and run "make". The
Makefile should work for any POSIX-compatible operating system.

2. What does comphom do?

Given a triangulation (or a whole bunch of them), comphom computes
homology groups and writes the results to a file. One line is used per
triangulation and if read from left to right, the descend in order,
i.e.: The highest dimension is the first group from the left, the lowest
dimension is on the very right side of the line.

Firts, the rank of the free part of the group is printed. Afterwards
the torsion parts are printed in the form Z_n. The torsion parts will be
concatenated via "+". 

3. How do I use comphom...
3.1. ...when using F. Lutz' triangulations?

F. Lutz has some nice triangulations on his website. These have been
used to test the program. If you want to use them, please visit the
Manifold Page at http://www.math.tu-berlin.de/diskregeom/stellar.

To feed them into comphom, follow these three steps:

	1. Download all triangulations you are interested in. 
	2. Use the perl script lex_convert.pl to convert the files
	   into the format comphom understands:
	   	perl lex_convert.pl --input <file>
	3. Start comphom:
		comphom --input <file>.ct [--output <file>.out]

3.2. ...when using my own triangulations?

If you have your own triangulations, the process is even simpler:

	1. Convert the triangulation so that comphom can process it.
	   This is very easy: Each simplex is entered in one line. After
	   all simplices have been entered, add one line reading "EOT"
	   (end of triangulation).
	2. Start comphon:
		comphom --input <file> [--output <file>.out]

4. It does not work!

If you have found a bug, please inform me about it
(bastian[AT]annwfn[DOT]net).		
