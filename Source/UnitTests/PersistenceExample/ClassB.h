#ifndef CLASSB___H
#define CLASSB___H

#include "TSerializedItem.h"
#include "TObjectList.h"
#include "BaseClass.h"
#include "ClassC.h"
#include "ClassD.h"

class ClassB : public BaseClass
{
	protected:
		virtual void Serialize(unsigned short version=0);

	public:
		ClassB();
		~ClassB();

		TShortString UserName;
		short		 Pin;
		short        Delay;		
		ClassC*      AssignedGroup;
		TObjectList<ClassD> AssignedKeypads;

};


#endif


