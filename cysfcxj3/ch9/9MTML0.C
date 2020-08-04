 
  #include "stdio.h"
  #include "9mtml.c"
  main()
  { static double a[3]={ 1.0,1.0,1.0};
    static double b[3]={ 2.0,2.0,2.0};
    double  mtmlf(int,double []);
    printf("\n");
    printf("s=%e\n",mtml(3,a,b,mtmlf));
    printf("\n");
  }

  double mtmlf(n,x)
  int n;
  double x[];
  { int i;
    double f;
    f=0.0;
    for (i=0; i<=n-1; i++)
      f=f+x[i]*x[i];
    return(f);
  }

