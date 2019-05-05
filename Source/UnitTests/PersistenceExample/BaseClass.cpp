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

eItemState  BaseClass::GetState()
{
	return State;
}

void BaseClass::SetState(eItemState aState)
{
	State = aState;
}

void  BaseClass::SetSubType(unsigned short aSubType)
{
	this->SubType = aSubType;
}
 
