#ifndef CLASSD___H
#define CLASSD___H

#include "TSerializedItem.h"
#include "BaseClass.h"

class ClassD : public BaseClass
{
	protected:
		virtual void Serialize(unsigned short version=0);

	public:
		ClassD();
		~ClassD();

		TShortString   KeypadName;	
		unsigned short Rights;	

};


#endif


