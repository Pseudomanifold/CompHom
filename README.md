# CompHom

`comphom` is a simple C++ program for computing simplicial homology
groups of triangulated simplicial complexes.

## Dependencies

`comphom` requires a C++ compiler and [`CMake`](https://cmake.org) for
building. Alternatively, a simple `Makefile` is supplied.

## How do I install `comphom`?

If you have `CMake` installed:

    $ mkdir build
    $ cd build
    $ cmake ../
    $ make

If you want to try out the `Makefile` instead:

    $ make

The Makefile should work for any POSIX-compatible operating system.

## What does `comphom` do?

Given a triangulation (or a whole bunch of them), `comphom` computes
homology groups and writes the results to a file. One line is used per
triangulation and if read from left to right, they descend in order. In
other words: the highest dimension is the first group from the left, the
lowest dimension is on the very right side of the line.

First, the rank of the free part of the group is printed. Afterwards the
torsion parts are printed in the form `Z_n`. The torsion parts will be
concatenated via '+'. 

## How do I use `comphom`&hellip;

### &hellip;when using F. Lutz' triangulations?

F. Lutz has some nice triangulations on his website. These have been
used to test the program. If you want to use them, please visit the
Manifold Page at http://www.math.tu-berlin.de/diskregeom/stellar.

To feed them into `comphom`, follow these three steps:

1. Download all triangulations you are interested in. 
2. Use the Perl script `lex_convert.pl` to convert the files
   into the format comphom understands: `perl lex_convert.pl --input <file>`
3. Start `comphom`: `comphom --input <file>.ct [--output <file>.out]`

### &hellip;when using my own triangulations?

If you have your own triangulations, the process is even simpler:

1. Convert the triangulation so that `comphom` can process it. This is
   very easy: each simplex is entered in one line. After all simplices
   have been entered, add one line reading 'EOT' (end of triangulation).
2. Start `comphon`: `comphom --input <file> [--output <file>.out]`
