// file:	dynArray.h
// version: 3.0
// summary:	Dynamic Array
// author:  Petr Gajdoš
// Copyright (c) 2009-2014 All Rights Reserved

#ifndef __DYNARRAY_
#define  __DYNARRAY_

#ifndef DA_ASSERT_COUNT			
	#define DA_ASSERT_COUNT 1	// IFF 0, m_capacity is used in asserts, otherwise m_count is used in asserts
#endif

#include <assert.h>

template<typename T>
class DynArray
{

protected:
    const static unsigned int initCapacity;		// initial m_capacity of array memory (elements)
    const static float resizeFactor;			// multiplier (enlarge array)

    T*				m_data;
    unsigned int	m_capacity;
    unsigned int	m_count;

public:

    DynArray(unsigned int _capacity = initCapacity);
    DynArray(unsigned int unCount, const T& val);
    DynArray(unsigned int unCount, const T* _data);
    DynArray(const DynArray<T>& a);					//Copy constructor
    DynArray(DynArray<T>&& a);						//Move constructor
    
    DynArray(const T* begin, const T* end);
    template<typename S>DynArray(const S* begin, const S* end);
    ~DynArray();

    DynArray& operator=(const DynArray& a);			//Copy assignment operator
    DynArray& operator=(DynArray&& a);		//Move assignment operator

#if DA_ASSERT_COUNT == 1
    inline const T& operator[](unsigned int unIndex) const { assert(unIndex < m_count &&"DynArray::[] - Index out of bounds"); return m_data[unIndex]; }
    inline const T& at(unsigned int unIndex) const { assert(unIndex < m_count &&"DynArray::[] - Index out of bounds"); return m_data[unIndex]; }

    inline T& operator[](unsigned int unIndex) { assert(unIndex < m_count &&"DynArray::[] - Index out of bounds"); return m_data[unIndex]; }
    inline T& at(unsigned int unIndex) { assert(unIndex < m_count &&"DynArray::[] - Index out of bounds"); return m_data[unIndex]; }
#else

	inline const T& operator[](unsigned int unIndex) const { assert(unIndex < m_capacity &&"DynArray::[] - Index out of bounds");  return m_data[unIndex]; }
    inline const T& at(unsigned int unIndex) const { assert(unIndex < m_capacity &&"DynArray::[] - Index out of bounds"); return m_data[unIndex]; }

	inline T& operator[](unsigned int unIndex) { assert(unIndex < m_capacity &&"DynArray::[] - Index out of bounds"); if (unIndex >= m_count) m_count = unIndex + 1; return m_data[unIndex]; }
	inline T& at(unsigned int unIndex) { assert(unIndex < m_capacity &&"DynArray::[] - Index out of bounds"); if (unIndex >= m_count) m_count = unIndex + 1; return m_data[unIndex]; }
#endif


	//inline friend istream& operator>>(istream &in, DynArray &v){ return in; }

	//inline friend ostream& operator<<(ostream &stream, const DynArray &v)
	//{
	//	for (unsigned int i = 0; i < v.m_count; ++i)
	//		stream << v[i] << " ";
	//	return stream;
	//}

    template<typename S> void copy(const S* begin, const S* end);

    void push_back(const T& item);
	void push_back(const T&& item);

    //T pop_back();
    void pop_back();
    void insert_at_end(const T* p, unsigned int uCount);
    void insert_at(unsigned int position, const T& item);
    void swap(unsigned int from, unsigned int to);

    void enlarge();
    void reserve(unsigned int unNewCapacity);
    void resize(unsigned int unNewSize);
    void resize(unsigned int unNewSize, const T& newElementsFromThis);

    void clear();
	void clear(const unsigned int _capacity);
	void erase(unsigned int unIndex);
    void minimize();
	void setTo(const T& value);

	void deleteItems();
	void freeItems();

    inline unsigned int size() const { return m_count; }
	inline unsigned int capacity() const { return m_capacity; }
    inline T* front() const { return m_data; }
    inline T* back() const { assert(0 < m_count &&"DynArray::[] - No m_data"); return &m_data[m_count-1]; }
    inline T* end() const { assert(0 < m_count &&"DynArray::[] - No m_data"); T* ptr = &m_data[m_count - 1]; return ++ptr; }
    inline bool	empty() const { return m_count == 0; }
    inline T* dataPtr(unsigned int unIndex) const { assert(unIndex < m_count &&"DynArray::[] - Index out of bounds"); return &m_data[unIndex]; }
};

template<typename T> const unsigned int DynArray<T>::initCapacity = (1<<20);  //cca 1M values
template<typename T> const float DynArray<T>::resizeFactor = 2.0f;

template<typename T> DynArray<T>::DynArray(unsigned int _capacity) 
    :m_capacity(_capacity), m_count(0)
{
    m_data = (T*)malloc(m_capacity*sizeof(T));
    //m_data = (T*)calloc(m_capacity,sizeof(T));
}

template<typename T> DynArray<T>::DynArray(unsigned int unCount, const T& val)
    :m_capacity(unCount),m_count(unCount)
{
    m_data = (T*)malloc(m_capacity*sizeof(T));
    while(unCount--)
        memcpy(m_data + unCount,&val,sizeof(T));
}

template<typename T> DynArray<T>::DynArray(unsigned int unCount, const T* _data)
    :m_capacity(unCount),m_count(unCount)
{
    m_data = (T*)malloc(m_capacity*sizeof(T));
    memcpy(m_data,_data,m_capacity*sizeof(T));
}

template<typename T> DynArray<T>::DynArray(const DynArray<T>& a)
    :m_capacity(a.m_capacity),m_count(a.m_count)
{
    if(this != &a)
    {
        m_data = (T *)malloc(sizeof(T)*a.m_capacity);
        memcpy(m_data, a.m_data, sizeof(T)*a.m_count);
    }
}

//template<typename T> DynArray<T>::DynArray(DynArray<T>&& a) : DynArray<T>(a.m_capacity)
//{
//	m_count = a.m_count;
//	m_data = a.m_data;
//	a.m_count = 0;
//    a.m_capacity = 0;
//    a.m_data = nullptr;
//}

template<typename T> DynArray<T>::DynArray(DynArray<T>&& a) : m_capacity(a.m_capacity), m_count(a.m_count), m_data(std::move(a.m_data))
{
	a.m_count = 0;
	a.m_capacity = 0;
	a.m_data = nullptr;
}


template<typename T> DynArray<T>::DynArray(const T* begin, const T* end)		//END is the first element address BEYOND the range og elements that will be copied. 
{
    m_count = m_capacity = (end - begin);
    if (m_count == 0)
    {
        m_data = 0;
        return;
    }
    m_data = (T*)malloc(m_capacity*sizeof(T));
    memcpy(m_data, begin, m_capacity*sizeof(T));
}


template<typename T>
template<typename S> DynArray<T>::DynArray(const S* begin, const S* end)		//END is the first element address BEYOND the range og elements that will be copied. 
{
    m_count = m_capacity = (end - begin);
    if (m_count == 0)
    {
        m_data = 0;
        return;
    }
    m_data = (T*)malloc(m_capacity*sizeof(T));
    for (unsigned int i = 0; i < m_count; i++)
    {
        m_data[i] = *(begin + i);
    }
}


template<typename T>DynArray<T>::~DynArray() 
{ 
    if(m_data) 
    { 
        free(m_data); 
        m_data = 0; 
    } 
}

template<typename T>DynArray<T>& DynArray<T>::operator=(const DynArray &a)			//Copy assignment operator
{
    if(this != &a)
    {
        if(a.m_count == 0) clear();
        else
        {
            if (m_capacity != a.m_capacity)
                resize(a.m_capacity);
            //resize(a.m_count);
            m_count = a.m_count;
            memcpy(m_data, a.m_data, sizeof(T)*a.m_count);
        }
    }
    return *this;
}

template<typename T>DynArray<T>& DynArray<T>::operator=(DynArray &&a)			//Move assignment operator
{
    if (this != &a)
    {
        if (m_data)
            free(m_data);
        m_capacity = a.m_capacity;
        m_count = a.m_count;
        m_data = a.m_data;
        a.m_count = 0;
        a.m_capacity = 0;
        a.m_data = nullptr;
    }
    return *this;
}

template<typename T>
template<typename S> void DynArray<T>::copy(const S* begin, const S* end)		//END is the first element address BEYOND the range og elements that will be copied. 
{
    m_count = m_capacity = (end - begin);
    if (m_count == 0)
    {
        m_data = 0;
        return;
    }
    m_data = (T*)malloc(m_capacity*sizeof(T));
    for (unsigned int i = 0; i < m_count; i++)
    {
        m_data[i] = *(begin + i);
    }
}


template<typename T>void DynArray<T>::enlarge()
{
    m_capacity = (unsigned int)(m_capacity * resizeFactor);
    m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);
}

template<typename T>void DynArray<T>::resize(unsigned int unNewSize)
{
    if(unNewSize)
    {
        if((unNewSize > m_capacity) || (unNewSize < m_capacity/2))
        {
            m_capacity = unNewSize;
            m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);	
        }
    }
    else
        clear();
    m_count = unNewSize;
}

template<typename T>void DynArray<T>::resize(unsigned int unNewSize, const T& newElementsFromThis)
{
    if(unNewSize)
    {
        if((unNewSize > m_capacity) || (unNewSize < m_capacity/2))
        {
            m_capacity = unNewSize;
            m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);	
        }
        for(unsigned int i = m_count; i < unNewSize; ++i) m_data[i] = newElementsFromThis;
    }
    else
        clear();
    m_count = unNewSize;
}

template<typename T>void DynArray<T>::minimize()
{
    m_capacity = m_count;
    m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);
}

template<typename T>void DynArray<T>::erase(unsigned int unIndex)
{
    if(m_count == 1)
        clear();
    else
    {
        assert(unIndex < m_count && "DynArray<>::Erase(unsigned int) - Index out of bounds");
        memmove(m_data + unIndex, m_data + unIndex + 1, (m_count - 1 - unIndex) * sizeof(T) );
        m_count--;
    }
}

template<typename T>void DynArray<T>::pop_back()
{
    if (m_count == 0) return;
  //  if(m_count == 1)
  //  {
		//clear(m_capacity);
  //  }
  //  else
  //  {
  //      --m_count;
  //  }
	--m_count;
}

template<typename T>void DynArray<T>::deleteItems()
{
	for (unsigned int i = 0; i < m_count; i++)
	{
		delete m_data[i];
	}
	clear();
}

template<typename T>void DynArray<T>::freeItems()
{
	for (unsigned int i = 0; i < m_count; i++)
	{
		free(m_data[i]);
	}
	clear();
}


template<typename T>void DynArray<T>::clear()
{
    if(0 == m_count) return;
    m_data = (T *)realloc(m_data, sizeof(T)*initCapacity); 
    m_capacity = initCapacity;
    m_count = 0;
}

template<typename T>void DynArray<T>::clear(const unsigned int _capacity = initCapacity)
{
    if (0 == m_count) return;
    if (m_capacity != _capacity)
    {
        m_data = (T *)malloc(sizeof(T)*_capacity);
        m_capacity = _capacity;
    }
    m_count = 0;
}

template<typename T>void DynArray<T>::push_back(const T& item)
{
    m_count++;
    if(m_count > m_capacity)
    {
        m_capacity = (unsigned int)(m_capacity * resizeFactor);
        m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);
    }
    //memcpy(m_data + m_count - 1, &item, sizeof(T) );
    m_data[m_count - 1] = item;
}

template<typename T>void DynArray<T>::push_back(const T&& item)
{
	m_count++;
	if (m_count > m_capacity)
	{
		m_capacity = (unsigned int)(m_capacity * resizeFactor);
		m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);
	}
	//memcpy(m_data + m_count - 1, &item, sizeof(T) );
	m_data[m_count - 1] = std::move(item);
}

template<typename T>void DynArray<T>::reserve(unsigned int unNewCapacity)
{
    m_capacity = unNewCapacity;
    m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);
    if(m_count > m_capacity) m_count = m_capacity;
}

template<typename T>void DynArray<T>::insert_at_end(const T* p, unsigned int uCount)
{
    unsigned int uNewSize = m_count + uCount;
    if(uNewSize > m_capacity)
    {
        m_capacity = (unsigned int)(m_capacity * resizeFactor);
        if(m_capacity < uNewSize) m_capacity = uNewSize;
        m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);
    }
    //for(unsigned int i = 0; i < uCount; ++i) m_data[m_count + i] = p[i];
    memcpy(&m_data[m_count], &p[0], uCount*sizeof(T));
    m_count += uCount;
}

template<typename T>void DynArray<T>::swap(unsigned int from, unsigned int to)
{
    T tmp = m_data[to];
    m_data[to] = m_data[from];
    m_data[from] = tmp;
}

template<typename T>void DynArray<T>::insert_at(unsigned int position, const T& item)
{
    if (position > m_count) return;
    if (position == m_count)
    {
        push_back(item);
        return;
    }

    unsigned int uNewSize = m_count + 1;
    if (uNewSize > m_capacity)
    {
        m_capacity = (unsigned int)(m_capacity * resizeFactor);
        if (m_capacity < uNewSize) m_capacity = uNewSize;
        m_data = (T *)realloc(m_data, sizeof(T)*m_capacity);
    }

    for (unsigned int i = m_count; i > position; i--)		//shift right
    {
        swap(i, i - 1);
    }
    
    m_data[position] = item;

    m_count += 1;
}

template<typename T>void DynArray<T>::setTo(const T& value)
{
	T* ptr = m_data;
	for (unsigned int i = 0; i < m_capacity; ++i, ++ptr)
	{
		*ptr = value;
	}
	//std::fill_n(m_data, m_capacity, value);
}
#endif
