#ifndef CLASSC___H
#define CLASSC___H

#include "TSerializedItem.h"
#include "BaseClass.h"

class ClassB;

class ClassC : public BaseClass
{
	protected:
		virtual void Serialize(unsigned short version=0);

	public:
		ClassC();
		~ClassC();

		TShortString   GroupName;	
		unsigned short Rights;	

};


#endif


