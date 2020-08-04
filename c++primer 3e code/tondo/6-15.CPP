#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s = "/.+(STL).*$1STL/";
    string filter = "STL";
    string t = s;

    cout << t << endl;
    string::size_type pos = 0;
    string::size_type last = 0;
    while ((pos = t.find(filter, pos)) != string::npos) {
        t.erase(last, pos - last);
        pos = last = last + filter.size();
        cout << '\t' << t << "pos: " << pos << endl;
    }
    // �����ѤU���D STL �r��
    t.erase(last, t.size() - last);
    cout << t << endl;

    t = s;
    string::iterator here = t.begin();
    string::iterator there;
    string::size_type cur = 0;  // �ѤU�r��ثe��m
    pos = 0;

    while ((pos = t.find(filter, pos)) != string::npos) {
        there = here + pos - cur;
        t.erase(here, there);
        cur += filter.size();
        pos = cur;
        here = t.begin() + cur;
    }
    // �����ѤU���D STL �r��
    t.erase(here, t.end());
    cout << t <<endl;

    return 0;
}
