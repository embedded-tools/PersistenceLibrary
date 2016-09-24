#include "ClassManagerA.h"

ClassManagerA::ClassManagerA()
{
	subManagers.Add(&ManagerB);
	subManagers.Add(&ManagerC);
	subManagers.Add(&ManagerD);
}

ClassManagerA::~ClassManagerA()
{


}

void ClassManagerA::Serialize(unsigned short version)
{
	SerializeHeader(version);

	ManagerD.Serialize(version);	
	ManagerC.Serialize(version);
	ManagerB.Serialize(version);
	
	SerializeBody(version);
	SerializeFooter(version);
}

void ClassManagerA::Clear()
{
	ManagerB.Clear();
	ManagerC.Clear();
	ManagerD.Clear();
}
