/*
 * Persistence Library / FileUtils / TStringList
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

#ifndef TSTRINGLIST___H
#define TSTRINGLIST___H

#include <stdlib.h> 
#include <stdio.h>
#include "tstring.h"
#include "tstream.h"
#include "tfilestream.h"
#include "stringutils.h"


//class is designed for storing, sorting, loading and saving text files, 
//internally it stores all strings in one memory block, strings are separated
//by zero, final string is followed by two zeros. 
class TStringList 
{
private:	
    //buffer for storing string data
    int   buf_capacity;
    int   buf_size;
    char* buf;

    //buffer for storing pointers to string beginnings
    int*  pstr;
    
    //buffer for storing additional tag for each string item
    int*  obj;  

    //counter shared by PSTR and OBJ
    int   item_num;
    int   item_capacity;
    
    //fsorted=true -> data are being sorted during inserting
    bool  fsorted;

    void  InsertBytes (int i, int len);
    bool  Compare (const char *s1, int len1, const char *s2, int len2);

    void AddObject (const char* s, int sLength, void* AObject);
    void InsertObject (int i, const char*s, int sLength, void* AObject);
    
    void UpdateBufSizes ();

public:

    TStringList (bool sorted=false);
    TStringList (TStringList &list);
    ~TStringList ();

    void Clear ();

    void Add (const char* s);
    void AddObject (const char* s, void* AObject);
    int  Count ();

    int  IndexOf (const char* s);
    void Insert (int i, const char*s);
    void InsertObject (int i, const char*s, void* AObject);
    void Delete (int i);
    void Exchange (int i1, int i2);

    void Sort (bool ascending=true);	

    bool LoadFromStream (TStream *stream);
    bool SaveToStream   (TStream *stream);
    bool LoadFromFile (const char* filename);
    bool SaveToFile (const char* filename);
    
    TString Strings (int i);
	TString operator[] (int i);

    void*  Objects (int i);
    void  SetText(const char* val);
    char* Get();
    int   GetLength();
    int   GetCapacity();

    void Copy(TStringList* stl);
};


#endif
