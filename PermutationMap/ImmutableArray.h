/**
* ImmutableArray.h
* @Author   Tu Yongce <yongce (at) 126 (dot) com>
* @Created  2008-10-7
* @Modified 2008-10-7
* @Version  0.1
*/

#ifndef IMMUTABLE_ARRAY_H_INCLUDED
#define IMMUTABLE_ARRAY_H_INCLUDED

#include <vector>
#include <cassert>

/*
* �����޸����飬����Ԫ��ֵһ���趨�������޸�
*/
template <typename T>
class ImmutableArray
{
public:
    typedef T ValueType;

private:
    ValueType m_placeHolder;
    std::vector<ValueType> m_data;

public:
    /*
    * ����һ��ImmutableArray����
    * @param n: ����Ԫ�ظ���
    * @param placeHolder: ����Ԫ���ڱ��趨ֵ֮ǰ��ռλ��
    */
    ImmutableArray(size_t n, ValueType placeHolder): m_placeHolder(placeHolder), 
        m_data(n, placeHolder) 
    { }

    /*
    * �������ָ��λ�ô��洢ֵ
    * @param index: ָʾ�洢λ�õ������±꣬�����ڷ�Χ[0, n)��
    * @param value: ���洢��ֵ�������빹�캯��ʹ�õĲ���ֵplaceHolder��ͬ
    * @return: ���ָ��λ���Ѿ���ֵ���򷵻�false�����ҷ����洢���������򷵻�true
    */
    bool Put(size_t index, ValueType value) 
    {
        assert(index < m_data.size());
        assert(value != m_placeHolder);

        if (m_data[index] != m_placeHolder) 
            return false;

        m_data[index] = value;

        return true;
    }

    /*
    * ��ȡ����ָ���洢λ�ô���ֵ
    * @param index: ָʾ�洢λ�õ������±꣬�����ڷ�Χ[0, n)��
    * @return: ���ָ��λ���Ѿ���ֵ���򷵻ظ�ֵ��
    *          ���򣬷��ع��캯��ʹ�õĲ���ֵplaceHolder
    */
    ValueType Get(size_t index) const
    {
        return m_data[index];
    }

    /*
    * ��ѯ����ָ��λ�ô��Ƿ�Ϊ�գ���δ�洢ֵ��
    * @param index: ָʾ�洢λ�õ������±꣬�����ڷ�Χ[0, n)��
    * @return: ���ָ��λ����ֵ���򷵻�false�����򷵻�true
    */
    bool Empty(size_t index) const
    {
        return m_data[index] == m_placeHolder;
    }

    /*
    * ���������Ԫ�ظ����������ع���Ĳ���n��ֵ��
    */
    size_t Size() const 
    {
        return m_data.size();
    }
};

/*
* ģ����ImmutableArray���bool���͵��ػ���
* @note: �ܹ�ѹ���洢�ռ䣬��Ч��Լʹ�õ��ڴ���Դ
*/
template<>
class ImmutableArray<bool>
{
public:
    typedef bool ValueType;

private:
    typedef unsigned char uint8_t;
    size_t m_size;
    bool m_placeHolder;
    std::vector<uint8_t> m_data;

public:
    ImmutableArray(size_t n, bool placeHolder) : m_size(n),
        m_placeHolder(placeHolder), 
        m_data((n + 7) / 8, (placeHolder ? 0xFF : 0x00)) 
    { }

    bool Put(size_t index, bool value)
    {
        assert(index < m_size);
        assert(value != m_placeHolder);

        bool tag = (m_data[index / 8] & (uint8_t(0x01) << (index % 8))) != 0;
        if (tag != m_placeHolder)
            return false;

        // ָ��λ��0,1����
        m_data[index / 8] ^= (uint8_t(0x01) << (index % 8));

        return true;
    }

    bool Get(size_t index) const
    {
        return (m_data[index / 8] & (uint8_t(0x01) << (index % 8))) != 0;
    }

    bool Empty(size_t index) const
    {
        bool tag = (m_data[index / 8] & (uint8_t(0x01) << (index % 8))) != 0;
        return tag == m_placeHolder;
    }

    size_t Size() const 
    {
        return m_size;
    }
};

#endif // IMMUTABLE_ARRAY_H_INCLUDED
