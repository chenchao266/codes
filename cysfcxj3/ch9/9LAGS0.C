
  #include "stdio.h"
  #include "9lags.c"
  main()
  { double lagsf(double);
    printf("\n");
    printf("g=%e\n",lags(lagsf));
    printf("\n");
  }

  double lagsf(x)
  double x;
  { double y;
    y=x*exp(-x);
    return(y);
  }

