#ifndef CLASSMANAGERC___H
#define CLASSMANAGERC___H

#include "TSerializedShortCollection.h"
#include "ClassC.h"

class ClassManagerC : public TSerializedShortCollection<ClassC>
{
	public:
		ClassManagerC();
		~ClassManagerC();
};


#endif
