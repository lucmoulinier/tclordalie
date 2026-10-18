


# works with Tcl 8.6 and Tcl 9 (the library name differs, pkgIndex.tcl picks it)
set dir [file dirname [file normalize [info script]]]
source [file join $dir pkgIndex.tcl]
package require tclordalie
set Ls {}
lappend Ls "ACDEFG...KLMNPQ"
lappend Ls "ACDDDFG..KLMNPQ"
lappend Ls "ACDDDFG.NNNNNPQ"

set Lw {0.6 0.3 0.1}

set sum [Ccode_SumOfPairs blosum62 $Ls $Lw]
puts "sum: $sum"




