/*
 * Persistence Library / Graphics / TFilter3x3
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

#ifndef TFILTER3X3___H
#define TFILTER3X3___H

class TFilter3x3
{

public:
	TFilter3x3(int f11, int f12, int f13,
		       int f21, int f22, int f23,
		       int f31, int f32, int f33,
		       int divisor, int bias);

	signed char _11;
	signed char _12;
	signed char _13;
	signed char _21;
	signed char _22;
	signed char _23;
	signed char _31;
	signed char _32;
	signed char _33;
	signed char Divisor;
	signed char Bias;
};

#endif 
