/*
 * Persistence Library / Collections / TStaticQueue
 *
 * Copyright (c) 2016 Ondrej Sterba <osterba@inbox.com>
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

#ifndef TQUEUE___H
#define TQUEUE___H

template <typename T, int N> class TQueue 
{
    private:
        T               m_circularBuffer[N];
        unsigned short  m_circularBufferStart;
        unsigned short  m_count;

    public:

        TQueue();

        void  Clear();
        bool  Enqueue(T  value);
        bool  Dequeue(T& value);
        bool  IsFull();
        bool  IsEmpty();  

#ifdef STL_STYLE
		bool push(T value);
		bool pop();
		T    front();
		T    back();
		bool empty();
		bool full();

		int  size();
		int  max_size();
		void clear();
#endif

};

#include "TQueue.cpp"

#endif