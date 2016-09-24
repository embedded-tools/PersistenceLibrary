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

#ifndef TFILEPATH___H
#define TFILEPATH___H

#include "tcustomstring.h"
#include "stringutils.h"

#define FILEPATH_MAXLENGTH 128


class TFilePath : public TCustomString<FILEPATH_MAXLENGTH>
{
	public:
		TFilePath();
		TFilePath(const char* text, unsigned short textLength=0);

		const char* ExtractFileName(const char* filename);
		const char* ExtractFileExt(const char* filename);

		TFilePath   ExtractFileDirectory(const char* filename);
		TFilePath   ExtractRelativePath (const char* dir, const char* basedir);

		bool        ChangeFileExt  (const char* pszNewExtension);
		bool        ChangeFileName (const char* pszNewFilename);


		TFilePath operator = ( TFilePath& oString )
		{
			CopyFrom(oString.ToPChar());
			return *this;    
		}

		TFilePath& operator = ( TString& oString )
		{
			CopyFrom(oString.ToPChar());
			return *this;
		}

		TFilePath& operator = ( const char* pChar)
		{
			CopyFrom(pChar);
			return *this;
		}
        
        void DeleteLastDir(TString &s)
        {
            unsigned short i;
            for (i = s.Length()-2; i>2; i--) 
            {
                if ((s[i]=='\\') && (s[i-2]==':'))
                {
                    //detects "c:\", it is not considered a directory
                    break;
                }
                if (s[i]=='\\')
                {
                    s.SetLength(i);
                    break;
                }
            }
        }

		TFilePath operator + ( const char* pChar )
		{
			TFilePath result = *this;
			result+=pChar;
			return result;
		}

		TFilePath& operator += ( const char* pChar )
		{
            int pCharLen = 0;

            if (Length()>0)
            {
                while (pChar[0]=='.')
                {
                    if ((pChar[1]=='.'))
                    {
                        pChar += 2;
                        DeleteLastDir(*this);
                    } else
                    if (pChar[1]=='\\')
                    {
                        pChar += 1;
                    } else
                    {
                        break;
                    }
                }
                if (Length()>0)
                {
                    if ((*this)[Length()-1]=='\\')
                    {
                        pChar++;
                    }
                }
            };			
			if (pChar!=NULL)
			{
				pCharLen = 0;
				if (pChar!=NULL)
				{
					pCharLen = strlen(pChar);    
				}
			}
			if (pCharLen==0) return *this;
			unsigned short oldLength = Length();
			unsigned short newLength = oldLength + pCharLen;
			if (newLength<oldLength) newLength=65534;
			if (newLength==65535) newLength=65534;
			newLength = SetLength(newLength);        
			if (newLength>oldLength)
			{
				memcpy(&PData[oldLength], pChar, newLength-oldLength);
			}
			return *this;
		}



		bool operator == (TCustomString<FILEPATH_MAXLENGTH>& s)
		{
			if ((ToPChar()==NULL) && (s.ToPChar()==NULL)) return true;
			if (ToPChar()==NULL) return false;
			if (s.ToPChar()==NULL) return false;
			return strcmp(ToPChar(), s.ToPChar()) == 0;
		}

		bool operator == (const char* pChar)
		{
			if ((ToPChar()==NULL) && (pChar==NULL)) return true;
			if ((ToPChar()==NULL) && (*pChar==0)) return true;
			if (ToPChar()==NULL) return false;
			if (pChar==NULL) return false;
			return strcmp(ToPChar(), pChar) == 0;
		}

		bool operator != (TCustomString<FILEPATH_MAXLENGTH>& s)
		{
			if ((ToPChar()==NULL) && (s.ToPChar()==NULL)) return false;
			if (ToPChar()==NULL) return true;
			if (s.ToPChar()==NULL) return true;
			return strcmp(ToPChar(), s.ToPChar()) != 0;
		}

		bool operator != (const char* pChar)
		{
			if ((ToPChar()==NULL) && (pChar==NULL)) return false;
			if (ToPChar()==NULL) return true;
			if (pChar==NULL) return true;
			return strcmp(ToPChar(), pChar) != 0;
		}

};




#endif