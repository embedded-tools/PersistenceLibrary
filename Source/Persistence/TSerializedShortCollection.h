/*
 * Persistence Library / Persistence / TSerializedShortCollection
 *
 * Copyright (c) 2016-2018 Ondrej Sterba <osterba@atlas.cz>
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
#include "TObjectList.h"

template<class ITEM>
class TSerializedShortCollection : public TSerializedBaseCollection
{
	private:
		TObjectList<ITEM> itemCollection;
	
	protected:

		virtual TEnumerator<TSerializedItem*> GetEnumerator()
		{
			if (itemCollection.Count()==0)
			{
				TEnumerator<TSerializedItem*> enumerator(NULL, NULL);
				return enumerator;
			} else {
				TEnumerator<TSerializedItem*> enumerator((TSerializedItem**)&itemCollection.ToArray()[0], ( (TSerializedItem**)&itemCollection.ToArray()[0] ) + itemCollection.Count());
				return enumerator;
			}			
		}
	
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
			if ( itemCollection.Remove((ITEM*)item) )
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

