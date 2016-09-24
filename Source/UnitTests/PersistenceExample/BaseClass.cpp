#include "BaseClass.h"

unsigned long BaseClass::GetUID()
{
	return UID;
}

void BaseClass::SetUID(unsigned long uid)
{
	UID = uid;
}

unsigned short  BaseClass::GetSubType()
{
	return SubType;
}

EItemState  BaseClass::GetState()
{
	return State;
}

void BaseClass::SetState(EItemState aState)
{
	State = aState;
}

void  BaseClass::SetSubType(unsigned short aSubType)
{
	this->SubType = aSubType;
}
 
