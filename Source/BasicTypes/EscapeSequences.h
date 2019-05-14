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

#ifndef ESCAPE_SEQUENCES___H
#define ESCAPE_SEQUENCES___H

int GetEscapedChar(char asciiCode, char* escapedChar);

/**
 *  It replaces characters CR LF TAB by "\r", "\n", "\t" strings
 *
 */
int AddEscapedChars(const char* text, int textLength, char* escapedText, int* escapedTextLength, int maxTextLength );

/**
 *  It restores characters CR LF TAB from "\r", "\n", "\t" strings
 *
 */
int RemoveEscapedChars(const char* escapedText, int escapedTextLength, char* outputText, int* outputTextLength, int maxOutputTextLength);

#endif
