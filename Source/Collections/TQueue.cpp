/*
 * Persistence Library / Collections / TStaticQueue
 *
 * Copyright (c) 2007-2016 Ondrej Sterba <osterba@inbox.com>
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

#ifndef TQUEUE___INL
#define TQUEUE___INL

#include "TQueue.h"
#include "TList.h"

template <class T, int N>
TQueue<T,N>::TQueue()
{
    m_circularBufferStart = 0;
    m_count = 0;
}

template <class T, int N>
void TQueue<T,N>::Clear()
{
    m_circularBufferStart = 0;
    m_count = 0;
}

template <class T, int N>
bool TQueue<T,N>::Enqueue(T value)
{
    if (m_count>=N)
    {
        return false;
    }
    long i = m_circularBufferStart + m_count;
    if (i>=N)
    {
        i-=N;
    }
    m_circularBuffer[i] = value;
    m_count++;
    return true;
}

template <class T, int N>
bool TQueue<T,N>::Dequeue(T& value)
{
    if (m_count==0)
    {
        return false;
    }
    long i = m_circularBufferStart;
    value = m_circularBuffer[i];

    m_circularBufferStart++;
    if (m_circularBufferStart>=N)
    {
        m_circularBufferStart = 0;
    }
    m_count--;
    return true;
}

template <class T, int N>
bool TQueue<T,N>::IsFull()
{
    return m_count>=N;
}

template <class T, int N>
bool TQueue<T,N>::IsEmpty()
{
    return m_count==0;
}

#ifdef STL_STYLE
template <class T, int N>
bool TQueue<T,N>::push(T value)
{
	return Enqueue(value);
}

template <class T, int N>
bool TQueue<T,N>::pop()
{
	static T tmp;
	return Dequeue(tmp);
}

template <class T, int N>
T TQueue<T,N>::front()
{
	int index = m_circularBufferStart;
	if (N)
	{
		while(index>=N) index-=N;
	}	
	return m_circularBuffer[index];
}

template <class T, int N>
T TQueue<T,N>::back()
{
	int index = m_circularBufferStart + m_count - 1;
	if (N)
	{
		while(index>=N) index-=N;
	}	
	return m_circularBuffer[index];
}

template <class T, int N>
bool TQueue<T,N>::empty()
{
	return m_count==0;
}

template <class T, int N>
bool TQueue<T,N>::full()
{
	return m_count>=N;
}

template <class T, int N>
int TQueue<T,N>::size()
{
	return m_count;
}

template <class T, int N>
int TQueue<T,N>::max_size()
{
	return N;
}

template <class T, int N>
void TQueue<T,N>::clear()
{
	memset(m_circularBuffer, 0, sizeof(m_circularBuffer));
	m_circularBufferStart = 0;
	m_count = 0;
}
#endif

#endif