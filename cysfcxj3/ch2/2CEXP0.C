
  #include "stdio.h"
  #include "2cexp.c"
  main()
  { double x,y,u,v;
    x=1.0; y=4.0;
    cexp(x,y,&u,&v);
    printf("\n");
    printf("  %13.7e +j %13.7e\n",u,v);
    printf("\n");
  }

