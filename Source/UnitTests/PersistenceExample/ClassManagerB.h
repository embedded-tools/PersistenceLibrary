#ifndef CLASSMANAGERB___H
#define CLASSMANAGERB___H

#include "TSerializedShortCollection.h"
#include "ClassB.h"

class ClassManagerB : public TSerializedShortCollection<ClassB>
{
	private:

		unsigned char PinLength;

	public:
		ClassManagerB();
		~ClassManagerB();



		virtual void Serialize(unsigned short version);


};


#endif
