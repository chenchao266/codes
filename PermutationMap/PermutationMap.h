/**
* PermutationMap.h
* @Author   Tu Yongce <yongce (at) 126 (dot) com>
* @Created  2008-10-7
* @Modified 2008-10-7
* @Version  0.1
*/

#ifndef PERMUTATION_MAP_H_INCLUDED
#define PERMUTATION_MAP_H_INCLUDED

#include <vector>
#include <stdexcept>
#include <cassert>
#include "ImmutableArray.h"

template <typename T>
class PermutationMap 
{
public:
    typedef T ValueType;

private:
    int m_num;
    std::vector<size_t> m_factorials;
    ImmutableArray<ValueType> m_data;

public:
    // n <= 12, 12! = 479001600
    PermutationMap(int n, ValueType initValue): m_num(n), m_factorials(n, 0), 
        m_data(CaclFactorial(), initValue) { }

    template <typename ElemType>
    bool Put(const ElemType permutation[], ValueType value) 
    {
        return m_data.Put(PermutationToNumber(permutation), value);
    }

    template <typename ElemType>
    bool Put(const std::vector<ElemType> &permutation, ValueType value)
    {
        assert(permutation.size() == m_num);
        return Put(&permutation[0], value);
    }

    template <typename ElemType>
    ValueType Get(const ElemType permutation[]) const
    {
        return m_data.Get(PermutationToNumber(permutation));
    }

    template <typename ElemType>
    ValueType Get(const std::vector<ElemType> &permutation) const
    {
        assert(permutation.size() == m_num);
        return Get(&permutation[0]);
    }

    size_t Size() const
    {
        return m_factorials[m_num - 1];
    }

private:
    size_t CaclFactorial()
    {
        m_factorials[0] = 1;
        for (int i = 2; i <= m_num; ++i) {
            if (m_factorials[i - 2] * i / i != m_factorials[i - 2])
                throw std::overflow_error("overflow in PermutationMap::CaclFactorial");
            m_factorials[i - 1] = m_factorials[i - 2] * i;
        }

        return m_factorials[m_num - 1];
    }

    template <typename ElemType>
    size_t PermutationToNumber(const ElemType permutation[]) const
    {
        size_t result = 0;
        for (int i = 1; i < m_num; ++i) {
            int count = 0;
            for (int k = 0; k < i; ++k) {
                if (permutation[k] > permutation[i])
                    ++count;
            }
            result += count * m_factorials[i - 1];
        }

        return result;
    }
};


#endif // PERMUTATION_MAP_H_INCLUDED
