#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 程式執行方式：
// (1) 先準備一個文字檔例如 test.txt
// (2) 在 DOS box 中執行 c:\> 20-3 < test.txt
//     意思是將 test.txt 重導入，做為 20-3.exe 的輸入。

template <class InputIterator>
void filter_string(InputIterator first, InputIterator last,
                   string filt_elems = string("\",?."))
{
    for ( ; first != last; first++) {
        string::size_type pos =0;
        while ((pos = (*first).find_first_of(filt_elems, pos))
                                              != string::npos)
            (*first).erase(pos, 1);
    }
}


bool length_less(const string &s1, const string &s2)
{
    return s1.size() < s2.size();
}

bool length_greater(const string &s1, const string &s2)
{
    return s1.size() > s2.size();
}


int main()
{
    istream_iterator<string> input(cin), eos;
    vector<string> text;

    copy(input, eos, back_inserter(text));

    string filt_elems("\",.?;:");
    filter_string(text.begin(), text.end(), filt_elems);

    vector<string>::iterator max =
        max_element(text.begin(), text.end(), length_less);
    vector<string>::iterator min =
        max_element(text.begin(), text.end(), length_greater);

    cout << "The number of words read is " << text.size() << endl;
    cout << "The longest word has length of " << max->size() << endl;
    cout << "The longest word is " << *max << endl;
    cout << "The shortest word has length of " << min->size() << endl;
    cout << "The shortest word is " << *min << endl;

    return 0;
}

