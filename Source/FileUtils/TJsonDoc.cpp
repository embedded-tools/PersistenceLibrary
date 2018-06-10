#include "TJsonDoc.h"

TJsonDoc::TJsonDoc()
{
	
	
}

TJsonDoc::TJsonDoc(TJsonTagBasePool& tagPool)
{

}

TJsonDoc::~TJsonDoc()
{
	
}

void TJsonDoc::SetPool(TJsonTagBasePool* tagPool)
{

}

TJsonTag* TJsonDoc::Header()
{
    return NULL;
}

TJsonTag* TJsonDoc::Root()
{
    return NULL;
}

void TJsonDoc::Clear()
{
    
}

bool TJsonDoc::LoadFromBuffer (char* rewriteableBuffer, int jsonLength)
{
    return false;
}

bool TJsonDoc::LoadFromString (TString &rewriteableString)
{
    return false;
}

bool TJsonDoc::LoadFromFile   (const char*   filename)
{
    return false;
}

bool TJsonDoc::SaveToStream (TStream& stream)
{
    return false;
}

bool TJsonDoc::SaveToFile (const char* filename)
{
    return false;
}
