/*
 * Persistence Library / Graphics / TFilter7x7
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

#include "TFilter7x7.h"

TFilter7x7::TFilter7x7(int f11, int f12, int f13, int f14, int f15, int f16, int f17,
					   int f21, int f22, int f23, int f24, int f25, int f26, int f27,
					   int f31, int f32, int f33, int f34, int f35, int f36, int f37,
					   int f41, int f42, int f43, int f44, int f45, int f46, int f47,
					   int f51, int f52, int f53, int f54, int f55, int f56, int f57,
					   int f61, int f62, int f63, int f64, int f65, int f66, int f67,
					   int f71, int f72, int f73, int f74, int f75, int f76, int f77,
  					   int divisor, int bias)
{
	_11 = f11; _12 = f12; _13=f13; _14=f14; _15=f15; _16=f16; _17=f17;
	_21 = f21; _22 = f22; _23=f23; _24=f24; _25=f25; _26=f26; _27=f27;
	_31 = f31; _32 = f32; _33=f33; _34=f34; _35=f35; _36=f36; _37=f37;
	_41 = f41; _42 = f42; _43=f43; _44=f44; _45=f45; _46=f46; _47=f47;
	_51 = f51; _52 = f52; _53=f53; _54=f54; _55=f55; _56=f56; _57=f57;
	_61 = f61; _62 = f62; _63=f63; _64=f64; _65=f65; _66=f66; _67=f67;
	_71 = f71; _72 = f72; _73=f73; _74=f74; _75=f75; _76=f76; _77=f77;
	Divisor = divisor;
	Bias = bias;

	bool negative=(f11<0)||(f12<0)||(f13<0)||(f14<0)||(f15<0)||(f16<0)||(f17<0)||
		(f21<0)||(f22<0)||(f23<0)||(f24<0)||(f25<0)||(f26<0)||(f27<0)||
		(f31<0)||(f32<0)||(f33<0)||(f34<0)||(f35<0)||(f36<0)||(f37<0)||
		(f41<0)||(f42<0)||(f43<0)||(f44<0)||(f45<0)||(f46<0)||(f47<0)||
		(f51<0)||(f52<0)||(f53<0)||(f54<0)||(f55<0)||(f56<0)||(f57<0)||
		(f61<0)||(f62<0)||(f63<0)||(f64<0)||(f65<0)||(f66<0)||(f67<0)||
		(f71<0)||(f72<0)||(f73<0)||(f74<0)||(f75<0)||(f76<0)||(f77<0);

	Divisor = 0;
	if (f11>0) Divisor+=f11;
	if (f12>0) Divisor+=f12;
	if (f13>0) Divisor+=f13;
	if (f14>0) Divisor+=f14;
	if (f15>0) Divisor+=f15;
	if (f16>0) Divisor+=f16;
	if (f17>0) Divisor+=f17;

	if (f21>0) Divisor+=f21;
	if (f22>0) Divisor+=f22;
	if (f23>0) Divisor+=f23;
	if (f24>0) Divisor+=f24;
	if (f25>0) Divisor+=f25;
	if (f26>0) Divisor+=f26;
	if (f27>0) Divisor+=f27;

	if (f31>0) Divisor+=f31;
	if (f32>0) Divisor+=f32;
	if (f33>0) Divisor+=f33;
	if (f34>0) Divisor+=f34;
	if (f35>0) Divisor+=f35;
	if (f36>0) Divisor+=f36;
	if (f37>0) Divisor+=f37;

	if (f41>0) Divisor+=f41;
	if (f42>0) Divisor+=f42;
	if (f43>0) Divisor+=f43;
	if (f44>0) Divisor+=f44;
	if (f45>0) Divisor+=f45;
	if (f46>0) Divisor+=f46;
	if (f47>0) Divisor+=f47;

	if (f51>0) Divisor+=f51;
	if (f52>0) Divisor+=f52;
	if (f53>0) Divisor+=f53;
	if (f54>0) Divisor+=f54;
	if (f55>0) Divisor+=f55;
	if (f56>0) Divisor+=f56;
	if (f57>0) Divisor+=f57;

	if (f61>0) Divisor+=f61;
	if (f62>0) Divisor+=f62;
	if (f63>0) Divisor+=f63;
	if (f64>0) Divisor+=f64;
	if (f65>0) Divisor+=f65;
	if (f66>0) Divisor+=f66;
	if (f67>0) Divisor+=f67;

	if (f71>0) Divisor+=f71;
	if (f72>0) Divisor+=f72;
	if (f73>0) Divisor+=f73;
	if (f74>0) Divisor+=f74;
	if (f75>0) Divisor+=f75;
	if (f76>0) Divisor+=f76;
	if (f77>0) Divisor+=f77;

	Bias = 0;
	if (negative) Bias=127;

}
