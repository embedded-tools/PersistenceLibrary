/*
 * Persistence Library / Collections / TStaticQueue
 *
 * Copyright (c) 2016-2018 Ondrej Sterba <osterba@atlas.cz>
 *
 * https://github.com/embedded-tools/PersistenceLibrary
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef TSTACK___INL
#define TSTACK___INL

#include "TStack.h"

template <class T, int N>
TStack<T,N>::TStack()
{
    m_count = 0;
}

template <class T, int N>
void TStack<T,N>::Clear()
{
    m_count = 0;
}

template <class T, int N>
bool TStack<T,N>::Push(const T value)
{
    if (m_count>=N)
    {
        return false;
    }
    long i = m_count;
    m_linearBuffer[i] = value;
    m_count++;
    return true;
}

template <class T, int N>
bool TStack<T,N>::Pop(T& value)
{
    if (m_count==0)
    {
        return false;
    }
	value = m_linearBuffer[m_count-1];
    m_count--;
    return true;
}

template <class T, int N>
bool TStack<T,N>::IsFull()
{
    return m_count>=N;
}

template <class T, int N>
bool TStack<T,N>::IsEmpty()
{
    return m_count==0;
}

#ifdef STL_STYLE
template <class T, int N>
bool TStack<T,N>::push(const T value)
{
	return Push(value);
}

template <class T, int N>
bool TStack<T,N>::pop()
{
	static T tmp;
	return Pop(tmp);
}

template <class T, int N>
T TStack<T,N>::top()
{
	if (m_count==0) return 0;

	int index = m_count - 1;
	return m_linearBuffer[index];
}

template <class T, int N>
bool TStack<T,N>::empty()
{
	return m_count==0;
}

template <class T, int N>
bool TStack<T,N>::full()
{
	return m_count>=N;
}

template <class T, int N>
int TStack<T,N>::size()
{
	return m_count;
}

template <class T, int N>
int TStack<T,N>::max_size()
{
	return N;
}

template <class T, int N>
void TStack<T,N>::clear()
{
	memset(m_linearBuffer, 0, sizeof(m_linearBuffer));
	m_linearBufferStart = 0;
	m_count = 0;
}
#endif

#endif