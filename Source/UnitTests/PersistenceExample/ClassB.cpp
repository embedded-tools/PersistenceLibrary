#include "ClassB.h"
#include "MainObject.h"

ClassB::ClassB()
{
	UserName = "-";
	Pin = 1234;
	Delay = 999;
	AssignedGroup = NULL;
}

ClassB::~ClassB()
{


}

void ClassB::Serialize(unsigned short version)
{
	SerializeString  ("Username", UserName, false, 6, 24);
	SerializeShortInt("Pin", Pin);
	SerializeShortInt("Delay", Delay);
	SerializeUID     ("UserGroupUID", (void**) &AssignedGroup, true, &MainObject::MainManager.ManagerC);
	SerializeUIDArray("AssignedKeypads", (void**) &AssignedKeypads, 0, &MainObject::MainManager.ManagerD);
}