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

};

#include "TStaticQueue.cpp"

#endif