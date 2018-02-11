/*
 * Persistence Library / Graphics / TFilter7x7
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

#ifndef TFILTER7X7___H
#define TFILTER7X7___H

class TFilter7x7
{

public:

	TFilter7x7(int f11, int f12, int f13, int f14, int f15, int f16, int f17,
		       int f21, int f22, int f23, int f24, int f25, int f26, int f27,
		       int f31, int f32, int f33, int f34, int f35, int f36, int f37,
		       int f41, int f42, int f43, int f44, int f45, int f46, int f47,
		       int f51, int f52, int f53, int f54, int f55, int f56, int f57,
		       int f61, int f62, int f63, int f64, int f65, int f66, int f67,
		       int f71, int f72, int f73, int f74, int f75, int f76, int f77,
		       int divisor, int bias);

	signed char _11;
	signed char _12;
	signed char _13;
	signed char _14;
	signed char _15;
	signed char _16;
	signed char _17;
	signed char _21;
	signed char _22;
	signed char _23;
	signed char _24;
	signed char _25;
	signed char _26;
	signed char _27;
	signed char _31;
	signed char _32;
	signed char _33;
	signed char _34;
	signed char _35;
	signed char _36;
	signed char _37;
	signed char _41;
	signed char _42;
	signed char _43;
	signed char _44;
	signed char _45;
	signed char _46;
	signed char _47;
	signed char _51;
	signed char _52;
	signed char _53;
	signed char _54;
	signed char _55;
	signed char _56;
	signed char _57;
	signed char _61;
	signed char _62;
	signed char _63;
	signed char _64;
	signed char _65;
	signed char _66;
	signed char _67;
	signed char _71;
	signed char _72;
	signed char _73;
	signed char _74;
	signed char _75;
	signed char _76;
	signed char _77;

	signed char Divisor;
	signed char Bias;
};

#endif
