/**
* ImmutableArray_example.cpp
* @Author   Tu Yongce <yongce (at) 126 (dot) com>
* @Created  2008-10-7
* @Modified 2008-10-7
* @Version  0.1
*/

#include <iostream>
#include "ImmutableArray.h"
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
        m_log << "TestImmutableArray Start...\n";

        DoTest1();
        DoTest2();
        DoTest3();

        m_log << "TestImmutableArray End\n\n";
    }

private:
    void DoTest1()
    {
        m_log << "DoTest1 Start...\n";

        try {
            const size_t NUM = 0x100000;  // 2^20
            ImmutableArray<bool> arr(NUM, false);

            Assure(m_log, arr.Size() == NUM);

            for (size_t i = 0; i < NUM; ++i) {
                Assure(m_log, arr.Empty(i));
                Assure(m_log, arr.Get(i) == false);
                Assure(m_log, arr.Put(i, true));
                Assure(m_log, !arr.Empty(i));
                Assure(m_log, arr.Get(i) == true);
                Assure(m_log, !arr.Put(i, true));
            }
        }
        catch (AssureException) {
        }

        m_log << "DoTest1 End\n";
    }

    void DoTest2()
    {
        m_log << "DoTest2 Start...\n";

        try {
            const size_t NUM = 0x100000;  // 2^20
            ImmutableArray<bool> arr(NUM, true);

            Assure(m_log, arr.Size() == NUM);

            for (size_t i = 0; i < NUM; ++i) {
                Assure(m_log, arr.Empty(i));
                Assure(m_log, arr.Get(i) == true);
                Assure(m_log, arr.Put(i, false));
                Assure(m_log, !arr.Empty(i));
                Assure(m_log, arr.Get(i) == false);
                Assure(m_log, !arr.Put(i, false));
            }
        }
        catch (AssureException) {
        }

        m_log << "DoTest2 End\n";
    }

    void DoTest3()
    {
        m_log << "DoTest3 Start...\n";

        try {
            const size_t NUM = 0x100000;  // 2^20
            ImmutableArray<int> arr(NUM, -1);

            Assure(m_log, arr.Size() == NUM);

            for (size_t i = 0; i < NUM; ++i) {
                Assure(m_log, arr.Empty(i));
                Assure(m_log, arr.Get(i) == -1);
                Assure(m_log, arr.Put(i, i));
                Assure(m_log, !arr.Empty(i));
                Assure(m_log, arr.Get(i) == i);
                Assure(m_log, !arr.Put(i, i));
            }
        }
        catch (AssureException) {
        }

        m_log << "DoTest3 End\n";
    }
};

#ifdef SYMBOL_DO_TEST
UnitTest obj(std::clog);  // do test
#endif // SYMBOL_DO_TEST

ANONYMOUS_NAMESPACE_END
