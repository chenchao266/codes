#include <cassert>

#include "HugeNumber.hpp"

int main()
{
    //std::cout<<(-32>>5)<<"\n";
    HugeNumber hg1;
    std::cout<<hg1;
    HugeNumber hg2("-0x1234512345");
    std::cout<<hg2;
	
    HugeNumber hg3("-0x34294755555555.5555557803");
    std::cout<<hg3;
    HugeNumber hg33=hg3<<48;
    std::cout<<hg33;
    HugeNumber hg4("bx1011111111111111111010100111.100111111111111110101");
    std::cout<<hg4;
    HugeNumber hg5("0x1016747775645635434324330111.10010101");
    std::cout<<hg5;
    HugeNumber hg6("0x5501044fffeeaa0111.aa100101010001");
    std::cout<<hg6;
    HugeNumber hg66=hg6>>48;
    std::cout<<hg66;



    //HugeNumber hgpi("3.141592653589793238462643383279502884197169399375105");
    //std::cout<<hgpi;
    HugeNumber hg7(3333333, 3, 3);
    HugeNumber hg8(hg7);
    std::cout<<hg8;
    HugeNumber hg9=hg6+hg5;
    std::cout<<hg9;

    hg66=hg6-hg5;
    std::cout<<hg66;

    hg66=hg6*hg5;
    std::cout<<hg66;

    assert(hg2<hg6);

}
