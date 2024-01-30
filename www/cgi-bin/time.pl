#!/usr/bin/perl

use strict;
use warnings;

my ($sec, $min, $heure, $jour, $mois, $annee) = localtime();

$annee += 1900;

$mois += 1;

print "Hour : $heure:$min:$sec\n";
print "Date : $jour/$mois/$annee\n";