// sieve.h

#ifndef SIEVE_H_
#define SIEVE_H_

class PrimeTable
{
private:
  long maxNum;
  long size;
  unsigned char *sieve;

public:
  // creates a sieve for primes < bound
  PrimeTable( long bound );
  ~PrimeTable();

  // returns true if n is prime in interval [1,bound-1]
  bool IsPrime( long n ) const;

  // returns number of primes in interval [n,m], -1 if out of range
  long CountPrimes( long n, long m ) const;

  // returns next prime > n, -1 if out of range
  long NextPrime( long n ) const;

  // returns previous prime < n, -1 if out of range
  long PrevPrime( long n ) const;
};

#endif

