#ifndef TEVENTSERIALIZER___H
#define TEVENTSERIALIZER___H

#include "tevent.h"

class TEventSerializer
{
	public:
		static bool serialize(const TEvent& ev, char* outputBuffer, int outputBufferLength);
		static bool deserialize(const char* inputBuffer, int inputBufferLength, TEvent& ev);
};

#endif