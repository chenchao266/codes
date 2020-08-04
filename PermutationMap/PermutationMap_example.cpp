/**
* PermutationMap_example.cpp
* @Author   Tu Yongce <yongce (at) 126 (dot) com>
* @Created  2008-10-7
* @Modified 2008-10-7
* @Version  0.1
*/

#include <iostream>
#include <algorithm>
#include "PermutationMap.h"
#include "Assure.h"

using namespace std;

ANONYMOUS_NAMESPACE_START

class UnitTest
{
private:
    ostream &m_log;
public:
    UnitTest(ostream &log): m_log(log)
    {
        m_log << "TestPermutationMap Start...\n";

        DoTest1();
        DoTest2();
        DoTest3();

        m_log << "TestPermutationMap End\n\n";
    }

private:
    void DoTest1()
    {
        m_log << "DoTest1 Start...\n";

        try {
            PermutationMap<bool> permMap(9, false);

            char perm[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

            Assure(m_log, permMap.Get(perm) == false);
            Assure(m_log, permMap.Put(perm, true));
            Assure(m_log, permMap.Get(perm) == true);
            Assure(m_log, !permMap.Put(perm, true));
            int count = 1;

            while (next_permutation(perm, perm + 9)) {
                Assure(m_log, permMap.Get(perm) == false);
                Assure(m_log, permMap.Put(perm, true));
                Assure(m_log, permMap.Get(perm) == true);
                Assure(m_log, !permMap.Put(perm, true));
                ++count;
            }

            Assure(m_log, count == permMap.Size());
        }
        catch (AssureException) {
        }

        m_log << "DoTest1 End\n";
    }

    void DoTest2()
    {
        m_log << "DoTest2 Start...\n";

        try {
            PermutationMap<bool> permMap(9, true);

            char perm[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

            Assure(m_log, permMap.Get(perm) == true);
            Assure(m_log, permMap.Put(perm, false));
            Assure(m_log, permMap.Get(perm) == false);
            Assure(m_log, !permMap.Put(perm, false));
            int count = 1;

            while (next_permutation(perm, perm + 9)) {
                Assure(m_log, permMap.Get(perm) == true);
                Assure(m_log, permMap.Put(perm, false));
                Assure(m_log, permMap.Get(perm) == false);
                Assure(m_log, !permMap.Put(perm, false));
                ++count;
            }

            Assure(m_log, count == permMap.Size());
        }
        catch (AssureException) {
        }

        m_log << "DoTest2 End\n";
    }

    void DoTest3()
    {
        m_log << "DoTest3 Start...\n";

        try {
            PermutationMap<int> permMap(9, 0);

            char perm[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

            int count = 1;
            Assure(m_log, permMap.Get(perm) == 0);
            Assure(m_log, permMap.Put(perm, count));
            Assure(m_log, permMap.Get(perm) == count);
            Assure(m_log, !permMap.Put(perm, count));

            while (next_permutation(perm, perm + 9)) {
                ++count;
                Assure(m_log, permMap.Get(perm) == 0);
                Assure(m_log, permMap.Put(perm, count));
                Assure(m_log, permMap.Get(perm) == count);
                Assure(m_log, !permMap.Put(perm, count));
            }

            Assure(m_log, count == permMap.Size());
        }
        catch (AssureException) {
        }

        m_log << "DoTest3 End\n";
    }

    void DoTest4()
    {
        m_log << "DoTest4 Start...\n";

        try {
            PermutationMap<int> permMap(9, 0);

            char data[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
            vector<char> perm(data, data + 9);

            int count = 1;
            Assure(m_log, permMap.Get(perm) == 0);
            Assure(m_log, permMap.Put(perm, count));
            Assure(m_log, permMap.Get(perm) == count);
            Assure(m_log, !permMap.Put(perm, count));

            while (next_permutation(perm.begin(), perm.end())) {
                ++count;
                Assure(m_log, permMap.Get(perm) == 0);
                Assure(m_log, permMap.Put(perm, count));
                Assure(m_log, permMap.Get(perm) == count);
                Assure(m_log, !permMap.Put(perm, count));
            }

            Assure(m_log, count == permMap.Size());
        }
        catch (AssureException) {
        }

        m_log << "DoTest4 End\n";
    }
};

#ifdef SYMBOL_DO_TEST
UnitTest obj(std::clog);  // do test
#endif // SYMBOL_DO_TEST

ANONYMOUS_NAMESPACE_END
