/*
 * Persistence Library / Basic types / EscapeSequences
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

#include "EscapeSequences.h"
#include <stdlib.h>

int GetEscapedChar(char asciiCode, char* escapedChar)
{
    switch(asciiCode)
    {
        case 0   : *escapedChar = '0';  return 1;
        case '\\': *escapedChar = '\\'; return 1;
        case '\"': *escapedChar = '\"'; return 1;
        case 7   : *escapedChar = 'a';  return 1;
        case 8   : *escapedChar = 'b';  return 1;
        case 9   : *escapedChar = 't';  return 1;
        case 10  : *escapedChar = 'n';  return 1;
        case 11  : *escapedChar = 'v';  return 1; 
        case 12  : *escapedChar = 'f';  return 1; 
        case 13  : *escapedChar = 'r';  return 1; 
    }
    return 0;
}

int AddEscapedChars(const char* text, int textLength, char* escapedText, int* escapedTextLength, int maxTextLength )
{
    int i = 0;
    int outputLength = 0;

    if (text  == NULL) return 0;
    if (escapedText == NULL) return 0;
    if (escapedTextLength == NULL) return 0;

    *escapedTextLength = 0;    
    while(i<textLength)
    {
        char c, escChar;
        c = text[i];
        if (GetEscapedChar(text[i], &escChar))
        {
            escapedText[outputLength++] = '\\';  if (outputLength>=maxTextLength) return 0;
            escapedText[outputLength++] = escChar;
        } else {
            escapedText[outputLength++] = c;
        }
        if (outputLength>=maxTextLength) return 0;

        i++;
    }
    if (outputLength>=maxTextLength) return 0;
    escapedText[outputLength++] = 0;

    *escapedTextLength = outputLength;
    return 1;
}

int RemoveEscapedChars(const char* escapedText, int escapedTextLength, char* outputText, int* outputTextLength, int maxOutputTextLength)
{
    int i = 0;
    unsigned char* output = outputText;
    int outputLength = 0;

    if (escapedText == NULL) return 0;
    if (outputText  == NULL) return 0;
    if (outputTextLength == NULL) return 0;

    *outputTextLength = 0;

    
    while(i<escapedTextLength)
    {        
        if (escapedText[i]=='\\')
        {
            if (outputLength>=maxOutputTextLength) return 0;

            i++;
            if (i>=escapedTextLength) return 0;
            switch(escapedText[i++])
            {
                case '0' : output[outputLength++] = 0;   break;
                case '\\': output[outputLength++] = '\\';break;
                case '\"': output[outputLength++] = '\"';break;
                case 'a' : output[outputLength++] = 7;   break;
                case 'b' : output[outputLength++] = 8;   break;
                case 't' : output[outputLength++] = 9;   break;
                case 'n' : output[outputLength++] = 10;  break;
                case 'v' : output[outputLength++] = 11;  break;
                case 'f' : output[outputLength++] = 12;  break;
                case 'r' : output[outputLength++] = 13;  break;
            }

        } else {
            output[outputLength++] = escapedText[i++];
        }
    }
    if (outputLength>=maxOutputTextLength) return 0;
    output[outputLength++] = 0;

    *outputTextLength = outputLength;
    return 1;
}