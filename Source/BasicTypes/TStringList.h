/*
 * Persistence Library / FileUtils / TStringList
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

#ifndef TSTRINGLIST___H
#define TSTRINGLIST___H

#include "tstring.h"

const static int NUMBER_OF_TERMINAL_ZEROS = 1;

//class is designed for storing, sorting, loading and saving text files, 
//internally it stores all strings in one memory block, strings are separated
//by zero, final string is followed by two zeros. 
class TStringList 
{
private:	
    

    //buffer for storing string data
	int   m_stringDataCapacity;
	int   m_stringDataSize;
	char* m_stringData;

    //buffer for storing pointers to string beginnings
    int*  m_substringOffset;
    
    //counter shared by PSTR and OBJ
    int   m_substringCount;
    int   m_substringCapacity;
    
    //fsorted=true -> data are being sorted during inserting
    bool  m_sorted;

    void  InsertBytes (int i, int len);
    bool  Compare (const char *s1, int len1, const char *s2, int len2);

    void AddObject (const char* s, int sLength);
    void Insert    (int i, const char*s, int sLength);   

public:

    TStringList (bool sorted=false);
    TStringList (const TStringList &list);
    ~TStringList ();

    void Clear ();

    void Add (const char* s, int stringLength=-1);        
    void Insert (int i, const char*s);
    void Delete (int i);

    int   Count () const;
    int   GetLength() const;
    int   GetCapacity() const;

    int  IndexOf (const char* s) const;
    void Exchange (int i1, int i2);

    void Sort (bool ascending=true);
    
    const char* operator[] (int i) const;
    const char* GetString (int i) const;
    const char* GetStringArray() const;
    const TString GetStringAsObject (int i) const;

    void  SetText(const char* val);
    

    bool  Copy(TStringList* stl);
};


#endif
