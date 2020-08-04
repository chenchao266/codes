//============================================================================
// Name        : sieve.cpp
// Author      : Bernd C. Kellner
// Version     : 1.0 (64-bit)
// Copyright   : (C) 2008 Bernd C. Kellner (bk at bernoulli dot org)
// Description : generates a prime table by a simple sieve modulo 30
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, see <http://www.gnu.org/licenses/>.
//============================================================================


#include "stdlib.h"
#include "memory.h"

#include "sieve.h"

enum
{
  BitsPerData = 8,
  NumPerData = 30,
  FirstPrime = 7
};

unsigned char bits[NumPerData] = { 0, 0x01, 0, 0, 0, 0, 0, 0x02, 0, 0, 0,
  0x04, 0, 0x08, 0, 0, 0, 0x10, 0, 0x20, 0, 0, 0, 0x40, 0, 0, 0, 0, 0, 0x80 };

PrimeTable::PrimeTable( long bound )
{
  maxNum = abs( bound );
  size = (maxNum - 1) / NumPerData + 1;

  sieve = new unsigned char[size];
  memset( sieve, 0, size );

  for ( long i = FirstPrime; i * i < maxNum; i += 2 )
    if ( IsPrime( i ) )
      for ( long j = 2 * i; j < maxNum; j += i )
        sieve[j / NumPerData] |= bits[j % NumPerData];
}

PrimeTable::~PrimeTable()
{
  delete[] sieve;
}

bool PrimeTable::IsPrime( long n ) const
{
  if ( n >= maxNum )
    return false;

  if ( n < FirstPrime )
    return n == 2 || n == 3 || n == 5;

  int data = sieve[n / NumPerData];
  int mask = bits[n % NumPerData];

  return mask == 0 ? false : (data & mask) == 0;
}

long PrimeTable::CountPrimes( long n, long m ) const
{
  int count = 0;

  if ( n < 0 || m < 0 || n > m || n >= maxNum || m >= maxNum )
    return -1;

  if ( (n & 1) == 0 )
  {
    if ( n == 2 || ( n == 0 && m >= 2 ) )
      count++;
    n++;
  }

  for ( ; n <= m; n += 2 )
    if ( IsPrime( n ) )
      count++;

  return count;
}

long PrimeTable::NextPrime( long n ) const
{
  if ( n < 0 || n >= maxNum )
    return -1;

  if ( n < 2 )
    return 2;

  if ( (++n & 1) == 0 )
    n++;

  while ( n < maxNum && !IsPrime( n ) )
    n += 2;

  return n < maxNum ? n : -1;
}

long PrimeTable::PrevPrime( long n ) const
{
  if ( n <= 2 || n >= maxNum )
    return -1;

  if ( n == 3 )
    return 2;

  if ( (--n & 1) == 0 )
    n--;

  while ( !IsPrime( n ) )
    n -= 2;

  return n;
}
