#ifndef BASECLASS___H
#define BASECLASS___H

#include "TSerializedItem.h"

class BaseClass : public TSerializedItem
{
	protected:

		unsigned long   UID;
		EItemState      State;
		unsigned short  SubType;	
		unsigned char   Tag;

	public:
		
		virtual unsigned long  GetUID();
		virtual unsigned short GetSubType();		
		virtual EItemState     GetState();					

		virtual void		   SetUID(unsigned long uid);
		virtual void           SetSubType(unsigned short subType);
		virtual void		   SetState(EItemState state);

		virtual void Serialize(unsigned short version)=0;
};

#endif