#include <iostream>
using namespace std;

inline bool bit_on( unsigned int ui, int pos )
{
    return ui & (1 << pos );
}

inline void bit_turn_on(unsigned int &ui, int pos)
{
    ui |= (1 << pos);
}

inline void bit_turn_off(unsigned int &ui, int pos)
{
    ui &= ~ (1 << pos);
}

inline void flip_bit(unsigned int &ui, int pos)
{
    ui ^= (1 << pos);
}

inline bool bit_off(unsigned int ui, int pos)
{
    return !bit_on(ui, pos);
}

int main()
{
    unsigned int ui =0xd3;    // 1101 0011 in binary
                              // bits are numbered from the right
                              // starting at position 0
    cout << "ui in hex: "
         << hex << ui << '\n';
    // turn on the 4th bit from the right
    bit_turn_on(ui, 3);
    cout << "result should be 'db', it is "
         << hex << ui << '\n';
    // turn off the 4th bit from the right
    bit_turn_off(ui, 3);
    cout << "result should be 'd3', it is "
         << hex << ui << '\n';
    // flip the 4th bit from the right
    flip_bit(ui, 3);
    cout << "result should be 'db', it is "
         << hex << ui << '\n';
    // flip the 4th bit from the right
    flip_bit(ui, 3);

    cout << "result should be 'd3', it is "
         << hex << ui << '\n';
    cout << "4th bit should be 0, it is "
         << bit_on(ui, 3) << '\n';
    cout << "1st bit should be 1, it is "
         << bit_on(ui, 0) << '\n';

    return 0;
}
