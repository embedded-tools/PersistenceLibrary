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
	private:
		char m_separator;
	public:
		TFilePath();
		TFilePath(const char* text, unsigned short textLength=0);

		void ChangeSeparator(char separator);

		const char* ExtractFileName(const char* filename);
		const char* ExtractFileExt(const char* filename);

		TFilePath   ExtractFileDirectory(const char* filename);
		TFilePath   ExtractRelativePath (const char* dir, const char* basedir);

		bool        ChangeFileExt  (const char* pszNewExtension);
		bool        ChangeFileName (const char* pszNewFilename);

		void		DeleteFileName();
		bool		DeleteLastDir();
		TFilePath	operator = ( TFilePath& oString );
		TFilePath&	operator = ( TString& oString );
		TFilePath&	operator = ( const char* pChar);
		TFilePath	operator + ( const char* pChar );
		TFilePath&	operator += ( const char* pChar );
		TFilePath&	operator += ( const char c);
		bool		operator == (TCustomString<FILEPATH_MAXLENGTH>& s);
		bool		operator == (const char* pChar);
		bool		operator != (TCustomString<FILEPATH_MAXLENGTH>& s);
		bool		operator != (const char* pChar);

};




#endif