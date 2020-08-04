//============================================================================
// Name        : calcbn.cpp
// Author      : Bernd C. Kellner
// Version     : 2.0 (64-bit / GMP / single & multi threaded)
// Copyright   : (C) 2002-2008 Bernd C. Kellner (bk at bernoulli dot org)
// Description : computes a Bernoulli number via Riemann zeta function
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


#include <strstream>
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <math.h>

#include <gmp.h>
#include <gmpxx.h>

#include "thread.h"
#include "sieve.h"
#include "gmp-chudnovsky.h"

//---------------------------------------------------------------------------
// defs
//---------------------------------------------------------------------------

const int MAX_NUM = 10000000;
const int MAX_THREADS = 32;
const int MAX_CHECK = 200;
const double MM_LN2PI = 1.837877066409345484;
const double MM_1_LN2 = 1.442695040888963387;

//---------------------------------------------------------------------------
// power and mod
//---------------------------------------------------------------------------

// returns number of bits of n
int CountBits( int n )
{
  int m = 0;

  while ( n > 0 )
  {
    if ( n & 1 )
      m++;
    n >>= 1;
  }

  return m;
}

// returns Nn / ( 2^n - 1 )
inline mpz_class PowerDiv2( mpz_class &Nn, int n )
{
  mpz_class pw = 1;

  pw <<= n;
  return Nn / --pw;
}

// returns Nn / ( b^n - 1 ) for odd b
inline mpz_class PowerDivP( mpz_class &Nn, int base, int n2, int b2 )
{
  mpz_class pw;

  mpz_ui_pow_ui( pw.get_mpz_t(), base, n2 );
  for ( int i=0; i < b2; i++ )
    mpz_mul( pw.get_mpz_t(), pw.get_mpz_t(), pw.get_mpz_t() );
  mpz_clrbit( pw.get_mpz_t(), 0 );
  return Nn / pw;
}

// returns pos. remainder mod m
inline mpz_class Mod( mpz_class a, mpz_class &m )
{
  return (a %= m) >= 0 ? a : a+m;
}

// returns pos. remainder mod m
inline mpz_class Mod( mpz_class a, int m )
{
  return (a %= m) >= 0 ? a : a+m;
}

// returns b^n mod m
inline mpz_class PowerMod( mpz_class base, mpz_class &m, int n )
{
  mpz_class pw;

  mpz_powm_ui( pw.get_mpz_t(), base.get_mpz_t(), n, m.get_mpz_t() );
  return pw;
}

//---------------------------------------------------------------------------
// calculation
//---------------------------------------------------------------------------

// n even, 2 <= n <= MAX_N
void CalcDenom( const int n, mpz_class &Dn, double &dlog, mpz_class &Cn,
  mpz_class &Tn, PrimeTable *pt )
{
  int m, ndiv, p, pa[2];

  Dn = 1;
  dlog = 0.0;
  Tn = n;
  Cn = 0;

  // two iterations for denominator and correction terms
  for ( int iter = 0; iter < 2; iter++ )
    for ( int div = 1; div * div <= n; div++ )
      if ( n % div == 0 )
      {
        ndiv = n / div;
        m = 0;
        if ( pt->IsPrime( div + 1 ) )
          pa[m++] = div + 1;
        if ( pt->IsPrime( ndiv + 1 ) && div != ndiv )
          pa[m++] = ndiv + 1;

        for ( int i = 0; i < m; i++ )
        {
          p = pa[i];
          if ( iter == 0 )
          {
            Dn *= p;
            dlog += log( p );
            while ( Tn >= p && Tn % p == 0 )
              Tn /= p;
          }
          else
            Cn += Dn / p;
        }
      }
}

// n even, 2 <= n <= MAX_N
void CalcApprNumer( const int n, mpz_class &Nn, mpz_class &Dn, int prec )
{
  int nbits = CountBits( n );

  // factorial
  mpz_class fac;
  mpz_fac_ui( fac.get_mpz_t(), n );
  fac >>= n - nbits - 1;
  fac *= Dn;

  // pi
  mpf_class pp;
  mpf_pi( pp.get_mpf_t(), prec );

  // approximation of numerator
  mpf_ui_div( pp.get_mpf_t(), 1, pp.get_mpf_t() );
  mpf_pow_ui( pp.get_mpf_t(), pp.get_mpf_t(), n );
  pp >>= nbits;
  pp *= fac;

  Nn = floor( pp );
}

// n even, 2 <= n <= MAX_N
void CalcCorrNumer( const int n, mpz_class &Nn, mpz_class &Dn, mpz_class &Cn,
  mpz_class &Tn, int d )
{
  mpz_class m, m2, c, a;

  if ( Dn > d && Dn > Tn )
  {
    m = Dn;
    c = Cn;
    if ( n % 4 )
      c = -c;
  }
  else if ( Tn > d )
  {
    m = Tn;
    c = 0;
  }
  else
  {
    c = 1;
    m = d + 1;
    m2 = m * m;
    for ( int i = 2; i <= d; i++ )
      c += PowerMod( i, m2, n );

    c *= Dn;
    c /= m;
    if ( n % 4 == 0 )
      c = -c;
  }

  a = Mod( Nn - c, m );
  if ( a > 0 )
    Nn += m - a;
}

// n even, 2 <= n <= MAX_N
void CalcBn( const int n, mpz_class &Nn, mpz_class &Dn )
{
  int n2, b2, M, M2, d, p, p0, p1, prec, dl2;
  double ep, dlog, dnl2;
  mpz_class Tn, Cn;
  PrimeTable *pt;

  // prime table
  pt = new PrimeTable( n + 2 );

  // denominator
  CalcDenom( n, Dn, dlog, Cn, Tn, pt );
  if ( n < 10 )
  {
    Nn = n % 4 ? 1 : -1;
    delete pt;
    return;
  }

  // Exponents to improve power function
  n2 = n; b2 = 0;
  while ( !(n2 & 1) )
  {
    b2++;
    n2 >>= 1;
  }

  // precision and estimates
  ep = M_LN2 + dlog + (0.5 + n) * log( n ) + (0.5 - n) * MM_LN2PI - n
       + 1.0 / (12.0 * n);
  M = 1 + (int) exp( (ep - log( n - 1 )) / (n - 1) );
  M2 = 1 + (int) sqrt( M );
  prec = max( 14 + (int) (ep * MM_1_LN2), 48 );
  p0 = pt->NextPrime( M2 );
  p1 = pt->PrevPrime( M+1 );
  dnl2 = n * MM_1_LN2;
  dl2 = -8 + (int) (dlog * MM_1_LN2);

  // first approximation of numerator
  CalcApprNumer( n, Nn, Dn, prec );

  // sum1: iterated approximation of numerator via Euler product
  Nn += PowerDiv2( Nn, n );
  for ( p = 3, d = 6; p <= M2; p = pt->NextPrime( p ), d++ )
     Nn += PowerDivP( Nn, p, n2, b2 );

  // sum2: iterated approximation of numerator via prime zeta function
  if ( p0 <= p1 )
  {
    // skip lower terms which can be corrected later
    int prec2;
    for ( p = p1; (prec2 = prec - (int) (dnl2 * log( p ))) < dl2;
          p = pt->PrevPrime( p ) )
      ;

    if ( p >= p0 )
    {
      mpf_t s;
      mpf_init2( s, prec2 );
      mpf_set_ui( s, p );
      mpf_pow_ui( s, s, n2 );
      for ( int i=0; i < b2; i++ )
        mpf_mul( s, s, s );
      mpf_ui_div( s, 1, s );
      p = pt->PrevPrime( p );

      for ( ; p >= p0; p = pt->PrevPrime( p ) )
      {
        mpf_t pw;
        mpf_init2( pw, prec - (int) (dnl2 * log( p )) );
        mpf_set_ui( pw, p );
        mpf_pow_ui( pw, pw, n2 );
        for ( int i=0; i < b2; i++ )
          mpf_mul( pw, pw, pw );
        mpf_ui_div( pw, 1, pw );
        mpf_add( pw, pw, s );
        mpf_swap( pw, s );
        mpf_clear( pw );
      }

      mpf_class fs( s );
      mpf_clear( s );

      Nn += fs * Nn;
    }
  }

  // correction of numerator
  CalcCorrNumer( n, Nn, Dn, Cn, Tn, d );

  // sign of numerator
  if ( n % 4 == 0 )
    Nn = -Nn;

  delete pt;
}

//---------------------------------------------------------------------------
// threaded calculation
//---------------------------------------------------------------------------

class CalcBnThread : public ThreadContainer
{
private:
  int n, n2, b2, M, M2, d, p0, p1, pcurr, prec, dl2, nThr;
  double ep, dlog, dnl2;
  mpz_class Nn, Dn, Tn, Cn;
  mpf_class fs;
  PrimeTable *pt;

  int GetNextPrime();
  void F1();
  void F2();

protected:
  virtual void ThreadFunc( int sel, void* data );

public:
  CalcBnThread( int _n, int nthr );
  ~CalcBnThread();

  void Calc();
  void GetResult( mpz_class &_Nn, mpz_class &_Dn )
    { _Nn = Nn; _Dn = Dn; }
};

CalcBnThread::CalcBnThread( int _n, int nthr )
{
  n = _n;
  nThr = nthr;
  pt = new PrimeTable( n + 2 );
}

CalcBnThread::~CalcBnThread()
{
  delete pt;
}

void CalcBnThread::ThreadFunc( int sel, void* data )
{
  switch ( sel )
  {
    case 1:
      F1();
      F2();
      break;
    case 2:
      F2();
      break;
  }
}

int CalcBnThread::GetNextPrime()
{
  int p;
  WaitAccess();
  if ( pcurr > p0 )
    p = pcurr = pt->PrevPrime( pcurr );
  else
    p = -1;
  ReleaseAccess();
  return p;
}

void CalcBnThread::F1()
{
  // first approximation of numerator
  CalcApprNumer( n, Nn, Dn, prec );

  // sum1: iterated approximation of numerator via Euler product
  Nn += PowerDiv2( Nn, n );
  for ( int p = 3, d = 6; p <= M2; p = pt->NextPrime( p ), d++ )
    Nn += PowerDivP( Nn, p, n2, b2 );
}

void CalcBnThread::F2()
{
  int p = GetNextPrime();
  if ( p < p0 )
    return;

  mpf_t s;
  mpf_init2( s, prec - (int) (dnl2 * log( p )) );
  mpf_set_ui( s, p );
  mpf_pow_ui( s, s, n2 );
  for ( int i=0; i < b2; i++ )
    mpf_mul( s, s, s );
  mpf_ui_div( s, 1, s );
  p = GetNextPrime();

  for ( ; p >= p0; p = GetNextPrime() )
  {
    mpf_t pw;
    mpf_init2( pw, prec - (int) (dnl2 * log( p )) );
    mpf_set_ui( pw, p );
    mpf_pow_ui( pw, pw, n2 );
    for ( int i=0; i < b2; i++ )
      mpf_mul( pw, pw, pw );
    mpf_ui_div( pw, 1, pw );
    mpf_add( pw, pw, s );
    mpf_swap( pw, s );
    mpf_clear( pw );
  }

  WaitAccess();
  mpf_add( fs.get_mpf_t(), fs.get_mpf_t(), s );
  ReleaseAccess();

  mpf_clear( s );
}

void CalcBnThread::Calc()
{
  // denominator
  CalcDenom( n, Dn, dlog, Cn, Tn, pt );
  if ( n < 10 )
  {
    Nn = n % 4 ? 1 : -1;
    return;
  }

  // Exponents to improve power function
  n2 = n; b2 = 0;
  while ( !(n2 & 1) )
  {
    b2++;
    n2 >>= 1;
  }

  // precision and estimates
  ep = M_LN2 + dlog + (0.5 + n) * log( n ) + (0.5 - n) * MM_LN2PI - n
       + 1.0 / (12.0 * n);
  M = 1 + (int) exp( (ep - log( n - 1 )) / (n - 1) );
  M2 = 1 + (int) sqrt( M );
  prec = max( 14 + (int) (ep * MM_1_LN2), 48 );
  p0 = pt->NextPrime( M2 );
  p1 = pt->PrevPrime( M+1 );
  dnl2 = n * MM_1_LN2;
  dl2 = -8 + (int) (dlog * MM_1_LN2);

  if ( p0 <= p1 )
  {
    // skip lower terms which can be corrected later
    for ( pcurr = p1; prec - (int) (dnl2 * log( pcurr )) < dl2;
          pcurr = pt->PrevPrime( pcurr ) )
      ;

    pcurr = pt->NextPrime( pcurr );
    fs.set_prec( prec - (int) (dnl2 * log( p0 )) );

    // threads
    AddThread( 1 );
    for ( int i = 1; i < nThr; i++ )
      AddThread( 2 );
    WaitFor();

    Nn += fs * Nn;
  }
  else
  {
    F1();
  }

  // correction of numerator
  CalcCorrNumer( n, Nn, Dn, Cn, Tn, d );

  // sign of numerator
  if ( n % 4 == 0 )
    Nn = -Nn;
}

// n even, 2 <= n <= MAX_N
void CalcBn_thr( const int n, mpz_class &Nn, mpz_class &Dn, const int nThr )
{
  CalcBnThread *bnthr = new CalcBnThread( n, nThr );
  bnthr->Calc();
  bnthr->GetResult( Nn, Dn );
  delete bnthr;
}

//---------------------------------------------------------------------------
// check result via Kummer congruences
//---------------------------------------------------------------------------

void CheckBn( const int n, mpz_class &Nn, mpz_class &Dn )
{
  PrimeTable *pt = new PrimeTable( n );
  mpz_class Nm, Dm;
  int c = 0, m, p = 3;
  bool ok = true;

  cout << "checking ...";
  while ( c < MAX_CHECK )
  {
    p = pt->NextPrime( p );
    if ( p < 0 )
      break;

    m = n % (p-1);
    if ( m == 0 || n % p == 0 )
      continue;

    c++;
    CalcBn( m, Nm, Dm );
    if ( Mod( Nn * Dm * m, p ) != Mod( Nm * Dn * n, p ) )
    {
      cout << endl << "failed at prime " << p << " and index " << m << endl;
      ok = false;
      break;
    }
  }

  if ( ok )
    cout << " ok" << endl;

  delete pt;
}

//---------------------------------------------------------------------------
// main
//---------------------------------------------------------------------------

void Copyright()
{
  cout << "calcbn64mp - computation of Bernoulli numbers via Riemann zeta function" << endl
       << "  version 2.0 (64-bit / GMP / single & multi-threaded)" << endl
       << "  (c) copyright 2002-2008 by Bernd C. Kellner" << endl
       << "  www.bernoulli.org / bk@bernoulli.org" << endl << endl;

  cout << "pi computation using Chudnovsky's algorithm" << endl
       << "  (c) copyright 2002, 2005 by Hanhong Xue" << endl
       << "      and partially 2005 by Torbjorn Granlund" << endl << endl;

  cout << "GMP - GNU Multiple Precision Library" << endl
       << "  (c) copyright 1991-2007 Free Software Foundation, Inc." << endl << endl;

  cout << "This is free software; see the source for copying conditions. There is NO" << endl
       << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl;
}

void Usage()
{
  cout << "usage: calcbn64mp [option] index" << endl
       << "  -v, --version   copyright and version" << endl
       << "  -s, --silent    no output" << endl
       << "  -t, --thread n  use n (2.." << MAX_THREADS << ") threads" << endl
       << "  -c, --check     check result" << endl
       << "  index           even index (2.." << MAX_NUM << ")" << endl;
}

int main( int argc, char* argv[] )
{
  bool IsSilent = false, IsCheck = false;
  int nThr = 0, num = 0;

  if ( argc < 2 )
  {
    Usage();
    return 0;
  }

  for ( int pos = 1; pos < argc; pos++ )
  {
    if ( !strcmp( argv[pos], "-v" ) || !strcmp( argv[pos], "--version" ) )
    {
      Copyright();
      return 0;
    }

    if ( !strcmp( argv[pos], "-s" ) || !strcmp( argv[pos], "--silent" ) )
    {
      IsSilent = true;
      continue;
    }

    if ( !strcmp( argv[pos], "-c" ) || !strcmp( argv[pos], "--check" ) )
    {
      IsCheck = true;
      continue;
    }

    if ( !strcmp( argv[pos], "-t" ) || !strcmp( argv[pos], "--thread" ) )
    {
      pos++;
      if ( pos >= argc )
        nThr = -1;
      else
      {
        nThr = atoi( argv[pos] );
        if ( nThr < 2 || nThr > MAX_THREADS )
          nThr = -1;
      }
      continue;
    }

    num = atoi( argv[pos] );
    break;
  }

  if ( nThr < 0 || num < 2 || num & 1 || num > MAX_NUM )
  {
    Usage();
    return -1;
  }

  mpz_class Nn, Dn;

  if ( nThr == 0 )
    CalcBn( num, Nn, Dn );
  else
    CalcBn_thr( num, Nn, Dn, nThr );

  if ( !IsSilent )
    cout << num << ": " << Nn << "/" << Dn << endl;

  if ( IsCheck )
    CheckBn( num, Nn, Dn );

  return 0;
}

