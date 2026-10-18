# tclordalie

C helper commands for Ordali, a multiple sequence alignment editor.
Package `tclordalie` 0.1. It builds against Tcl 8.6 and Tcl 9 from the same sources.

Commands: `Ccode_SumOfPairs`, `Ccode_Pairwise`, `Ccode_Pairwise2`, `Ccode_DoBootstrap`, `Ccode_DonnePosSG`.

```tcl
package require tclordalie
Ccode_SumOfPairs blosum62 {ACDEFG...KLMNPQ ACDDDFG..KLMNPQ} {0.6 0.4}
```

## Build

TEA (autoconf) build, out of the source tree:

```sh
mkdir build-tcl8 && cd build-tcl8
../configure --with-tcl=/path/to/tcl8.6/lib && make     # -> libtclordalie0.1.so
mkdir build-tcl9 && cd build-tcl9
../configure --with-tcl=/path/to/tcl9/lib && make       # -> libtcl9tclordalie0.1.so
make install
```

Both libraries can be installed side by side; `pkgIndex.tcl` loads the one that matches
the running interpreter. On Windows, build with MinGW/MSYS2 the same way.
`configure` is already generated; run `autoconf` only after editing `configure.ac`.

## Test

```sh
cd build-tcl9 && cp ../tst.tcl . && tclsh9.0 tst.tcl
```

## License

GPL-3.0-or-later, Copyright (c) Luc Moulinier. See [LICENSE](LICENSE).
