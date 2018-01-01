#ifndef TPAIR___H
#define TPAIR___H

template <typename KEY, typename VALUE> struct TPair
{
	public:
		KEY   first;
		VALUE second;

		TPair() 
		{
			memset(&first,  0, sizeof(first)); 
			memset(&second, 0, sizeof(second)); 
		};
		TPair(KEY aKey, VALUE aValue) { first = aKey; second=aValue; };
};

#endif