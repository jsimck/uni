////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	geometry.h
//
// summary:	Declaration and implementation of DynBuffer class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DYNBUFFER_H_
#define __DYNBUFFER_H_

#include <memory.h>
#include <malloc.h>
#include <dynArray.h>

template<typename T>
struct Buffer
{
	T* m_data;
	unsigned int m_sizeInBytes;
	unsigned int m_noElements;
	unsigned int m_elementSizeInBytes;

	inline Buffer() : m_data(nullptr), m_sizeInBytes(0), m_noElements(0), m_elementSizeInBytes(0) {}

	Buffer(const Buffer<T>& a) : m_sizeInBytes(a.m_sizeInBytes), m_noElements(a.m_noElements), m_elementSizeInBytes(a.m_elementSizeInBytes)	 			//Copy constructor
	{
		if (this != &a)
		{
			m_data = (T *)malloc(sizeof(T)*a.m_noElements);
			memcpy(m_data, a.m_data, sizeof(T)*a.m_noElements);
		}
	}

	Buffer(Buffer<T>&& a) : m_sizeInBytes(a.m_sizeInBytes), m_noElements(a.m_noElements), m_elementSizeInBytes(a.m_elementSizeInBytes), m_data(std::move(a.m_data)) //Move constructor
	{
		a.m_sizeInBytes = 0;
		a.m_noElements = 0;
		a.m_elementSizeInBytes = 0;
		a.m_data = nullptr;
	}

	inline ~Buffer() 
	{
		free();
	}

	Buffer& operator=(const Buffer& a);			//Copy assignment operator
	Buffer& operator=(Buffer&& a);				//Move assignment operator

	inline bool sizeMatch() { return sizeof(T) == m_elementSizeInBytes; }
	inline void free() 
	{
		if (m_data != nullptr)
		{
			std::free(m_data);
			m_data = nullptr;
		}
		m_sizeInBytes = 0;
		m_noElements = 0;
		m_elementSizeInBytes = 0;
	}
};

template<typename T>Buffer<T>& Buffer<T>::operator=(const Buffer &a)			//Copy assignment operator
{
	if (this != &a)
	{
		if (a.m_sizeInBytes == 0) free();
		else
		{
			m_sizeInBytes = a.m_sizeInBytes;
			m_noElements = a.m_noElements;
			m_elementSizeInBytes = a.m_elementSizeInBytes;
			m_data = (T *)malloc(m_sizeInBytes);
			memcpy(m_data, a.m_data, sizeof(T)*a.m_noElements);
		}
	}
	return *this;
}

template<typename T>Buffer<T>& Buffer<T>::operator=(Buffer &&a)				//Move assignment operator
{
	if (this != &a)
	{
		if (m_data)
			std::free(m_data);
		m_sizeInBytes = a.m_sizeInBytes;
		m_noElements = a.m_noElements;
		m_elementSizeInBytes = a.m_elementSizeInBytes;
		m_data = a.m_data;
		a.m_sizeInBytes = 0;
		a.m_noElements = 0;
		a.m_elementSizeInBytes = 0;
		a.m_data = nullptr;
	}
	return *this;
}


#endif