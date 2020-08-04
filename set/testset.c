/* 
 * testset.c from the Set of Integers example, 
 * Ch. 7, C: A Reference Manual 5/e
 * Samuel P. Harbison III and Guy L. Steele Jr.
 */
#include "set.h"
int main(void)
{
	print_k_of_n(0, 4);
	print_k_of_n(1, 4);
	print_k_of_n(2, 4);
	print_k_of_n(3, 4);
	print_k_of_n(4, 4);
	print_k_of_n(3, 5);
	print_k_of_n(3, 6);
	return 0;
}
