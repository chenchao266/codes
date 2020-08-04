#include <iostream>
#include <string>
using namespace std;

// 程式執行方式：
// (1) 先準備一個文字檔例如 test.txt
// (2) 在 DOS box 中執行 c:\> 20-6 < test.txt
//     意思是將 test.txt 重導入，做為 20-6.exe 的輸入。

const  int lineLength = 100;
char   line[lineLength];
string largestLine = "", tmp = "";
int    len, _max = 0;

int main()
{
int i = 0;  // jjhou testing

    /*
    譯註：本程式的邏輯其實是有問題的。查閱 iostream 的規格可知，以下函式：
    istream& istream::getline(char* str, streamsize count);
    最多只能讀入 count-1 個字元。上例將 count 設為 100，因此凡長度超過 100 的
    文字行皆不可能被讀入。讀者應重新思考如何處理長度超過 100 的文字行。
    */

    while (cin.getline(line, lineLength)) {
        len = cin.gcount();            // length of current line
        tmp += line;
        if (len < lineLength - 1) {    // found newline?
            if (tmp.size() > _max) {
                _max = tmp.size();
                largestLine = tmp;
            }
            tmp = "";                  // empty string
        }
    }

    cout << "largestLine: \n" << largestLine << endl;
    cout << "has length  " << largestLine.size() << endl;
}

