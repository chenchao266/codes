#include <iostream>
#include <string>
using namespace std;

// �{������覡�G
// (1) ���ǳƤ@�Ӥ�r�ɨҦp test.txt
// (2) �b DOS box ������ c:\> 20-6 < test.txt
//     �N��O�N test.txt ���ɤJ�A���� 20-6.exe ����J�C

const  int lineLength = 100;
char   line[lineLength];
string largestLine = "", tmp = "";
int    len, _max = 0;

int main()
{
int i = 0;  // jjhou testing

    /*
    Ķ���G���{�����޿���O�����D���C�d�\ iostream ���W��i���A�H�U�禡�G
    istream& istream::getline(char* str, streamsize count);
    �̦h�u��Ū�J count-1 �Ӧr���C�W�ұN count �]�� 100�A�]���Z���׶W�L 100 ��
    ��r��Ҥ��i��QŪ�J�CŪ�������s��Ҧp��B�z���׶W�L 100 ����r��C
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