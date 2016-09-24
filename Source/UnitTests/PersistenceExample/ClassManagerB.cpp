#include "ClassManagerB.h"

ClassManagerB::ClassManagerB()
{
	PinLength = 4;
}

ClassManagerB::~ClassManagerB()
{


}

void ClassManagerB::Serialize(unsigned short version)
{
	SerializeHeader(version);

	SerializeByte("PinLength", PinLength, 4, 6);

	SerializeBody(version);
	SerializeFooter(version);

}





