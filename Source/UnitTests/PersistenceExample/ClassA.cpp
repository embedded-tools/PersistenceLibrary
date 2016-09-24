#include "ClassA.h"

ClassA::ClassA()
{
	DeviceName = "<None>";
	DeviceType = "<None>";
	Flags      = 0;
}

ClassA::~ClassA()
{
	
}

void ClassA::Serialize(unsigned short version0)
{
	SerializeString("Name", DeviceName, false);
	SerializeString("Device", DeviceType, false);
	SerializeLongUInt("Flags", Flags);
}

