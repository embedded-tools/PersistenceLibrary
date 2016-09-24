#ifndef CLASSA___H
#define CLASSA___H

#include "TSerializedItem.h"
#include "BaseClass.h"

class ClassA : public BaseClass
{
	protected:
		virtual void Serialize(unsigned short version=0);

	public:
		ClassA();
		~ClassA();
			
		TShortString DeviceName;
		TShortString DeviceType;
		unsigned long Flags;

		

};


#endif


