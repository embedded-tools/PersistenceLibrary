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