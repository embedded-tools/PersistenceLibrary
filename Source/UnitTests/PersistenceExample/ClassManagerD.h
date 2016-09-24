#ifndef CLASSMANAGERD___H
#define CLASSMANAGERD___H

#include "TSerializedShortCollection.h"
#include "ClassD.h"

class ClassManagerD : public TSerializedShortCollection<ClassD>
{
	public:
		ClassManagerD();
		~ClassManagerD();
};


#endif