#include "ClassC.h"

ClassC::ClassC()
{
	GroupName = "<Admin>";
	Rights    = (unsigned short)12345678;
	
}

ClassC::~ClassC()
{

}


void ClassC::Serialize(unsigned short version)
{
	SerializeString("Name", GroupName, false, 2, 24);
	SerializeShortUInt("Rights", Rights);
}
