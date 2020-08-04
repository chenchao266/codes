
  #include "stdio.h"
  #include "9mtcl.c"
  main()
  { double mtclf(double);
    printf("\n");
    printf("s=%e\n",mtcl(2.5,8.4,mtclf));
    printf("\n");
  }

  double mtclf(x)
  double x;
  { double y;
    y=x*x+sin(x);
    return(y);
  }

