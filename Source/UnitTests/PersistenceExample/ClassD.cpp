#include "ClassD.h"

ClassD::ClassD()
{
	KeypadName = "Keypad1";;	
	Rights    = (unsigned short)123456;	
}

ClassD::~ClassD()
{

}


void ClassD::Serialize(unsigned short version)
{
	SerializeString("Name", KeypadName, false, 2, 24);
	SerializeShortUInt("Rights", Rights);
}
