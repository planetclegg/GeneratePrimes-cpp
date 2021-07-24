#include <chrono>
#include <list>
#include <vector>
#include <cmath>
#include <cstdio>
#include <sys/types.h>
using namespace std;

namespace PlanetCleggUtilsPrimes
{
    class Optimized64PrimeSieve 
    {
        public:
        vector<int> GeneratePrimesUpTo(int limit)
        {
            if (limit < 2)
                return vector<int>(0);

            uint ulimit = (uint)limit;
            int ulimitDiv2 = (int)limit / 2;

            //playing around with inlining and alternatives to BitArray
            // var bits = new BitArray(1+(limit/2), true);
            int sizeBits = ((limit / 2) + 64) / 64;
            //vector<uint64_t> bits; 
            // using array to try to get slightly better performance, but doesn't
            // make much difference vs using vector<>
            uint64_t * bits = new uint64_t[sizeBits];
            
            // we're just going to repeat this 60 bit pattern over and over
            // to mark the multiples of 3s and 5s in a more efficient way
            uint64_t wheel = 0b011010010010110;  // 15 bit seed, odd numbes up to 30
            // repeat 15 bit seed to make 60 bits
            wheel = wheel | wheel << 15 | wheel << 30 | wheel << 45;

            for (int i =0; i < sizeBits; i++)
            {
                // set first 60 bits, then repeat cycle for remaining 4
                bits[i] = (wheel << 60 | wheel);
                // rotate 60-bit wheel right by 4 bits. top 4 out of 64 bits stay 0
                wheel = (wheel >> 4) | ((wheel & 0b1111) << 56);
            }
            bits[0] &= ~0b110UL; // manually mark 3,5, because the wheel doesnt

            uint sqrt1 = (uint) sqrt(ulimit);
            int sqrtDiv2 = (int)sqrt1 / 2;

            // for factorDiv2 1->3, 2->5, 3->7, etc since even numbers are not tracked
            for (int factorDiv2 = 3; factorDiv2 <= sqrtDiv2; factorDiv2 += 1)
            {
                if ((bits[factorDiv2 >> 6] & (1UL << factorDiv2)) == 0)
                {
                    int factor = 1 + (factorDiv2 * 2);
                    for (int i = (factor * factor)/2; i <= ulimitDiv2; i += factor)
                    {
                        bits[(i >> 6)] |= (1UL << i);
                    }
                }
            }

            int capacity = EstimatePrimeCountUpTo(limit);
            vector<int> result;
            result.reserve(capacity);
            result.push_back(2);

            for (uint i = 3; i <= ulimit; i += 2)
            {
                int idx = (int)(i / 2);
                if ((bits[idx >> 6] & (1UL << idx)) == 0)
                {
                    result.push_back((int)i);
                }
            }
            delete[] bits;
            return result;
        }

        // used for List capacity estimation.  should be same or slightly
        // above the actual # primes up to n
        int EstimatePrimeCountUpTo(int n)
        {
            // note: the x/ln(x) estimate isn't great for small numbers,
            // so we'll fudge the smallest numbers, and estimates don't
            // really start to converge well until n > 200, where the
            // estimate is less than 10% off and gets smaller as n increases.
            if (n < 2)  return 0;
            if (n < 7)  return (n + 1) / 2;
            if (n < 11) return 4;

            // see: https://primes.utm.edu/howmany.html , but watch out for typos.
            double b = 1.04423;
            return  (int)(b * ((double)n / (log(n) - 1 ) ));
        }
    };
}


int main() 
{

    using namespace std::chrono;
 
    std::vector<int> vvv ={1};

    vector< vector<int> > runData {
            //(1000, 168),
            {10000, 1229},
            //(65536, 6542),
            {100000,        9592},
            {1000000,       78498},
            {10000000,      664579},
            {100000000,     5761455},
            {1000000000,    50847534},
            {2147483647,    105097565}
    };



    printf("Testing with %zd tests.\n", runData.size());
        
    for (auto &run : runData)     
    {
        int limit = run[0];
        int expected = run[1];
        
        printf(" - Calc primes up to %10d, (%10d expected)...",limit,expected);

        high_resolution_clock::time_point startTime = high_resolution_clock::now();
 
        PlanetCleggUtilsPrimes::Optimized64PrimeSieve sieve;
        vector<int> result = sieve.GeneratePrimesUpTo(limit);
        if (result.size() != expected)
        {
            printf("Failed to verify prime count for %d, %d != %zd", limit, expected, result.size());
            throw "bug-in-code";
        }
        
        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        duration<double, std::milli> time_span = endTime - startTime;
                 
        printf(" took %7.1f ms.\n",time_span.count());

    }
    printf("\nFinished.");
}