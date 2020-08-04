#include<stdio.h>

static bool a[8] ;
static bool b[15] ;
static bool c[15] ;
static int  x[8] ;
void tryq(int i,  bool q)
{ 
   int j;
   j  = 0;
   do{ 
      j  = j + 1; 
      q  = false;
      if( a[j - 1] && b[i + j - 2] && c[ i - j + 7] )
      { 
         x[ i - 1]  = j;
         a[ j - 1]  = false; 
         b[ i + j - 2]  = false; 
         c[ i - j + 7]  = false;
         if (i < 8 )
         { 
            tryq( i + 1, q);
            if (! q )
            { 
               a[ j - 1]  = true; 
               b[ i + j - 2]  = true; 
               c[ i - j + 7]  = true;
            }
         } 
         else 
         {
            q  = true;
         }
      }
   }while( !q && (j < 8));
}

void eightqueen1( )
{
   int i   ;
   bool q   ;

   for( i = 1; i <= 8 ; ++i) 
      a[ i - 1]  = true;
   for( i = 2; i <= 16 ; ++i) 
      b[ i - 2]  = true;
   for( i = -7; i <= 7 ; ++i) 
      c[ i + 7]  = true;
   tryq( 1, q);
   if (q)
      for( i = 1; i <= 8 ; ++i ) 
         printf("%d    ", x[ i-1]);
   printf("\n");
}