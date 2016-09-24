/*
 * Persistence Library / Basic types / TFilePath
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

#include "TFilePath.h"

TFilePath::TFilePath()
:TCustomString<FILEPATH_MAXLENGTH>()
{

}

TFilePath::TFilePath(const char* text, unsigned short textLength)
:TCustomString<FILEPATH_MAXLENGTH>(text, textLength)
{

}

const char* TFilePath::ExtractFileName(const char* filename)
{
	const char* result = NULL;
	int i = StrLen(filename)-1;
	while(i >= 0)
	{
		if ((filename[i]==92) || (filename[i]=='/')) break;
		i--;
	};
	return result;
};

const char* TFilePath::ExtractFileExt (const char* filename)
{
	const char* result = NULL;
	int i = StrLen(filename);
	while(i >= 0)
	{
		if (filename[i] == '.')	break;
		i--;
	};
	return result;
};

TFilePath TFilePath::ExtractFileDirectory(const char* filename)
{
	int i = StrLen(filename);
	while(i >= 0)
	{
		if (filename[i] == '\\') break;
		if (filename[i] == '/') break;
		i--;
	}
	TFilePath result(filename, i);
	return result;
};

TFilePath TFilePath::ExtractRelativePath (const char* dir, const char* basedir)
{
	int c,n;
	char* ss1;
	char* ss2;
	char* pp1;
	char* pp2;

	TFilePath result;

	pp1=(char*) dir;     ss1=pp1;
	pp2=(char*) basedir; ss2=pp2;
	n=0;
	for (c=1; c<=StrLen(dir); c++) 
	{
		//case insensitive string comparison
		if ((*ss1 & 0xDF) != (*ss2 & 0xDF)) break;
		if (*ss1=='\\') 
        {
			pp1=ss1;  pp2=ss2; 
			pp1++;    pp2++;
		}
		ss1++; ss2++; n++;
	}
	if (n==StrLen(basedir)) 
	{ 
		result = (const char*)pp1;
	}
	else 
	{
		while (*pp2!=0) 
		{
			if ((*pp2=='\\') || (*pp2=='/')) result += "..\\";
			pp2++;
		}
		if ((basedir[StrLen(basedir)-1]!='\\') && (basedir[StrLen(basedir)-1]!='/')) 
		{
			result += "..\\";
		}
		if (pp1==dir) result=(const char*)pp1;
		else result += (const char*)pp1;
	}
	return (result);  
}

bool TFilePath::ChangeFileExt (const char* ext)
{
	int i = Length();
	while(i >= 0)
	{
		if (PData[i] == '.') break;
		i--;
	}
	if (i==-1)
	{
		i = Length();
	}
	unsigned short extLen = StrLen(ext);
	if (Length()+extLen>=FILEPATH_MAXLENGTH)
	{
		return false;
	}

	memcpy(PData+i, ext, extLen+1);
	DataLen = i+extLen;

	return true;
};

bool TFilePath::ChangeFileName (const char* filename)
{
	int i = Length();
	while(i >= 0)
	{
		if (filename[i] == '\\')	break;
		i--;
	}
	if (i==-1)
	{
		i = Length();
	}
	unsigned short extLen = StrLen(filename);
	if (Length()+extLen>=FILEPATH_MAXLENGTH)
	{
		return false;
	}
	memcpy(PData+i, filename, extLen+1);
	DataLen = i+extLen;
	return true;
};


