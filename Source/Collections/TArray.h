#ifndef TARRAY___H
#define TARRAY___H

template <typename T> class TArray
{
private:
	T* 		m_itemArray;		   
	short   m_itemCount;
	short   m_itemPitch;

public:	
	TArray(T* pData, short itemCount, short itemPitch = 0);
	TArray(const TArray& a);

	T& operator [] (int index);

	operator T*();
	operator void*();

	int Length() { return m_itemCount; };
	T*  Pointer() { return m_itemArray; };

#ifdef STL_STYLE
	T* begin();
	T* end();
#endif

};

#include "TArray.cpp"


#endif