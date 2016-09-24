#ifndef MAINCLASS___H
#define MAINCLASS___H

#include "ClassManagerA.h"
#include "TMainSerializer.h"
#include "TObjectList.h"

class MainObject : public TMainSerializer
{
	protected:

		virtual bool Serialize(unsigned short version);
		virtual void Clear();

		virtual unsigned short GetVersion();

	public:
		MainObject();
		~MainObject();

		static ClassManagerA MainManager;



};


#endif


