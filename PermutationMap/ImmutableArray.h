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
* 不可修改数组，数组元素值一旦设定即不可修改
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
    * 构造一个ImmutableArray对象
    * @param n: 数组元素个数
    * @param placeHolder: 数组元素在被设定值之前的占位符
    */
    ImmutableArray(size_t n, ValueType placeHolder): m_placeHolder(placeHolder), 
        m_data(n, placeHolder) 
    { }

    /*
    * 在数组的指定位置处存储值
    * @param index: 指示存储位置的数组下标，必须在范围[0, n)内
    * @param value: 待存储的值，不能与构造函数使用的参数值placeHolder相同
    * @return: 如果指定位置已经有值，则返回false，并且放弃存储操作；否则返回true
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
    * 读取数组指定存储位置处的值
    * @param index: 指示存储位置的数组下标，必须在范围[0, n)内
    * @return: 如果指定位置已经有值，则返回该值；
    *          否则，返回构造函数使用的参数值placeHolder
    */
    ValueType Get(size_t index) const
    {
        return m_data[index];
    }

    /*
    * 查询数组指定位置处是否为空（还未存储值）
    * @param index: 指示存储位置的数组下标，必须在范围[0, n)内
    * @return: 如果指定位置有值，则返回false；否则返回true
    */
    bool Empty(size_t index) const
    {
        return m_data[index] == m_placeHolder;
    }

    /*
    * 返回数组的元素个数（即返回构造的参数n的值）
    */
    size_t Size() const 
    {
        return m_data.size();
    }
};

/*
* 模板类ImmutableArray针对bool类型的特化类
* @note: 能够压缩存储空间，有效节约使用的内存资源
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

        // 指定位的0,1互换
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
