
  #include "stdio.h"
  #include "2cdiv.c"
  main()
  { double e,f;
    cdiv(-1.3,4.5,7.6,-3.6,&e,&f);
    printf("\n");
    printf(" e+jf=%10.7f +j %10.7f",e,f);
    printf("\n");
  }

