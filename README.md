## GeneratePrimes-cpp

A straight port of just part of planetclegg/GeneratePrimes-dotnetcore

I just wanted to port enough to C/C++ to get a performance baseline of
compiled C/C++ to native ARM vs JITed C# running under rosetta.
No unit tests, but main() checks the counts..  This isn't meant to be used
for anything beyond a rough performance comparison.

I originally used vector<> for the sieve, but changed that to an array hoping
for some improvement.  Made very little difference.

Unless I'm doing something wrong, the C# implementations are looking pretty good, 
all things considered. Only seeing about a ~20-25% improvement in runtime with the C++ version.

#### Mac Mini M1 (Macmini9,1)
```bash
$ clang++ -std=c++14 -Ofast  Optimized64PrimeSieve.cpp
$ file a.out
a.out: Mach-O 64-bit executable arm64
$ ./a.out 
Testing with 7 tests.
 - Calc primes up to      10000, (      1229 expected)... took     0.1 ms.
 - Calc primes up to     100000, (      9592 expected)... took     0.5 ms.
 - Calc primes up to    1000000, (     78498 expected)... took     3.4 ms.
 - Calc primes up to   10000000, (    664579 expected)... took    34.2 ms.
 - Calc primes up to  100000000, (   5761455 expected)... took   167.8 ms.
 - Calc primes up to 1000000000, (  50847534 expected)... took  2457.9 ms.
 - Calc primes up to 2147483647, ( 105097565 expected)... took  5690.6 ms.
```