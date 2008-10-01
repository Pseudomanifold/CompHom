#!/usr/bin/perl
#
# This script expects triangulations of topological objects in F. Lutz'
# lexicographic format (now slightly outdated) and converts the file so that it
# can be used by the comphom program.
#
# See the manifold page (http://www.math.tu-berlin.de/diskregeom/stellar) for
# more details about triangulations.

use strict;
use warnings;

use Getopt::Long;

my $input;
my $output;

GetOptions(	"input=s"	=>	\$input,
		"i=s"		=>	\$input);

$output = $input . ".ct";
open(IN, "<", $input) or die "Error: Could not open input file: $_";
open(OUT, ">", $output) or die "Error: Could not open output file: $_";

my $partial;
my @fields;
while(my $data = <IN>)
{
	if($data =~ m/\]/)
	{
		# Cat the string and check if it's time
		# to process it.

		$partial .= $data;
		chomp($partial);
		if($data =~ m/\]\]/)
		{
			$partial =~ m/.+\=\[(.+)\]$/;
			@fields = split(/],/, $partial);

			my $vertex; 
			foreach my $field (@fields)
			{
				$field =~ m/\s*(=\[)?\[(.+)(\]\])?$/;
				$vertex = $2;
				$vertex =~ s/\]\]//;

				print OUT "$vertex\n";
			}

			$partial = "";
			print OUT "EOT\n";
		}
	}
}

close(OUT);
close(IN);
