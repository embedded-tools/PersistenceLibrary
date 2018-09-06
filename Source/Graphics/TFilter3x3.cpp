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

#include "TFilter3x3.h"

TFilter3x3::TFilter3x3(int f11, int f12, int f13,
								int f21, int f22, int f23,
								int f31, int f32, int f33,
								int divisor, int bias)
{
	_11 = f11; _12 = f12; _13=f13;
	_21 = f21; _22 = f22; _23=f23;
	_31 = f31; _32 = f32; _33=f33;
	Divisor = divisor;
	Bias = bias;

	bool negative=(f11<0)||(f12<0)||(f13<0)||
              	  (f21<0)||(f22<0)||(f23<0)||
		          (f31<0)||(f32<0)||(f33<0);

	Divisor = 0;
	if (f11>0) Divisor+=f11;
	if (f12>0) Divisor+=f12;
	if (f13>0) Divisor+=f13;
	if (f21>0) Divisor+=f21;
	if (f22>0) Divisor+=f22;
	if (f23>0) Divisor+=f23;
	if (f31>0) Divisor+=f31;
	if (f32>0) Divisor+=f32;
	if (f33>0) Divisor+=f33;

	Bias = 0;
	if (negative) Bias=127;

}