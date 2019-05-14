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

#ifndef TSTACK___H
#define TSTACK___H

template <typename T, int N> class TStack
{
    private:
        T               m_linearBuffer[N];
        unsigned short  m_count;

    public:

        TStack();

        void  Clear();
        bool  Push(const T  value);
        bool  Pop(T& value);
        bool  IsFull();
        bool  IsEmpty(); 

#ifdef STL_STYLE
		bool push(const T value);
		bool pop();
		T    top();
		bool empty();
		bool full();

		int  size();
		int  max_size();
		void clear();
#endif

};

#include "TStack.cpp"

#endif
