/*
 * Persistence Library / Persistence / TSerializedShortCollection
 *
 * Copyright (c) 2007-2016 Ondrej Sterba <osterba@inbox.com>
 *
 * https://github.com/embedded-tools/PersistenceLibrary
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef TSERIALIZEDSHORTCOLLECTION___H
#define TSERIALIZEDSHORTCOLLECTION___H

#include "TSerializedBaseCollection.h"
#include "tstaticobjectlist.h"

template<class ITEM>
class TSerializedShortCollection : public TSerializedBaseCollection
{
	private:
		TStaticObjectList<ITEM, 32> itemCollection;

		unsigned short iteratorIndex;

		virtual void* First()
		{
			iteratorIndex = 0;
			if (iteratorIndex>=itemCollection.Count()) return NULL;
			return (TSerializedItem*) itemCollection[iteratorIndex];
		};

		virtual void* Next()
		{
			iteratorIndex++;
			if (iteratorIndex>=itemCollection.Count()) return NULL;
			return (TSerializedItem*) itemCollection[iteratorIndex];
		};	

		virtual short Count()
		{
			return itemCollection.Count();
		};
	
	public:

		virtual TSerializedItem*  CreateItem(unsigned int uid=0, unsigned short subType=0)
		{
			//Creates object instance depending on class type		
			TSerializedItem* item = new ITEM();

			//overriden method CreateItem can create various classes depending on subtype like this:
			/*
			ITEM* item = NULL;
			switch(SubType)
			{
				case 0: item = new ITEM();					
				case 1: item = new OTHERITEM1();
				case 1: item = new OTHERITEM2();
				default: item = NULL;
			}
			*/

			//Sets UID and SubType
			if (item!=NULL)
			{
				item->SetUID(uid);
				item->SetSubType(subType);
			}
			return (TSerializedItem*) item;
		}

		virtual bool AddItem(TSerializedItem* item)
		{
			return itemCollection.Add((ITEM*)item);
		}

		virtual bool RemoveItem(TSerializedItem* item)
		{
			if ( itemCollection.Del((ITEM*)item) )
			{
				delete item;
				return true;
			}
			return false;
		}

		virtual void Clear()
		{
			itemCollection.UnallocAndClear();
		}


};



#endif

