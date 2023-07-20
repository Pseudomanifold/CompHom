#!/usr/bin/perl

use strict;
use warnings;

my $input 	= "graph.dot";
my $output 	= "graph.ct";

open(IN,  "<", $input)  or die "Error: Could not open \"$input\" for reading";

my %id = ();
my $last_id = 0;

my @output = ();
foreach my $line (<IN>)
{
	if($line =~ m/\s*(.+)s*->\s*(.+)\s*\[/)
	{
		$id{$1} = ++$last_id if(!exists($id{$1}));
		$id{$2} = ++$last_id if(!exists($id{$2}));

		# Entries that refer to themselves are removed from the list
		# because they would not change the result.
		push(@output, $id{$1} . "," . $id{$2} . "\n") if($1 ne $2);
	}
}

open(OUT, ">", $output) or die "Error: Could not open \"$output\" for writing";

foreach my $line (sort(@output))
{
	print OUT $line;
}

print OUT "EOT\n";

close(OUT);
close(IN);
