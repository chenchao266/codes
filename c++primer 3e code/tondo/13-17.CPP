#include <iostream>
#include <bitset>
using namespace std;

enum { READ, WRITE };

class File {
  public:
    inline int isRead() { return mode.test(READ); }
    inline int isWrite() { return mode.test(WRITE); }
    void write();
    void close();

    bitset< 2 > mode;
    bitset< 1 > modified;
    bitset< 3 > prot_owner;
    bitset< 3 > prot_group;
    bitset< 3 > prot_world;
    // ...
};

void File::write()
{
    modified.set(0);
    // ...
}

void File::close()
{
    if(modified.test(0)) {
    // ... save contents
    }
}

int main()
{
    File myFile;

    myFile.mode.set(READ);
    if(myFile.isRead())
        cout << "myFile.mode is set to READ\n";
}
