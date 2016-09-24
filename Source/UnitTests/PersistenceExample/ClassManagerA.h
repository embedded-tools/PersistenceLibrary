#ifndef CLASSMANAGERA___H
#define CLASSMANAGERA___H

#include "TSerializedShortCollection.h"
#include "ClassA.h"
#include "ClassManagerB.h"
#include "ClassManagerC.h"
#include "ClassManagerD.h"
#include "TMainSerializer.h"


class ClassManagerA : public TSerializedShortCollection<ClassA>
{
	public:
		ClassManagerA();
		~ClassManagerA();		

		ClassManagerB ManagerB;
		ClassManagerC ManagerC;
		ClassManagerD ManagerD;

		virtual void Serialize(unsigned short version);
		virtual void Clear();

};


#endif
