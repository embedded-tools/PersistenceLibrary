#include "teventserializer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool TEventSerializer::serialize(const TEvent& ev, char* outputBuffer, int outputBufferLength)
{
	int bytesWritten = 0;

	if ((bytesWritten + 12) >= outputBufferLength)
	{
		return false;
	}

	bytesWritten += sprintf(outputBuffer + bytesWritten, "%i,", ev.eventID);
	if ((bytesWritten + 12) >= outputBufferLength)
	{
		return false;
	}

	bytesWritten += sprintf(outputBuffer + bytesWritten, "%i,", ev.paramL);
	if ((bytesWritten + 11) >= outputBufferLength)
	{
		return false;
	}

	bytesWritten += sprintf(outputBuffer + bytesWritten, "%i", ev.paramH);

	if (ev.dataLength>0)
	{
		if ((bytesWritten + 2*ev.dataLength + 3) >= outputBufferLength)
		{
			return false;
		}
		outputBuffer[bytesWritten++] = ',';
		outputBuffer[bytesWritten++] = '\"';

		unsigned char c1,c2;
		for (int i = 0; i<ev.dataLength; i++)
		{
			c1  = (ev.data[i] >> 4);
			c1 += '0';
			if (c1>'9') c1 += 7;

			c2  = (ev.data[i] & 0x0F);
			c2 += '0';
			if (c2>'9') c2 += 7;

			bytesWritten += sprintf(outputBuffer + bytesWritten, "%c%c", c1, c2);
		}

		outputBuffer[bytesWritten++] = '\"';
		outputBuffer[bytesWritten++] = 0;
	}
	return true;
}

bool TEventSerializer::deserialize(const char* inputData, int inputDataLength, TEvent& ev)
{
	if (sscanf(inputData, "%d,%d,%d", &ev.eventID, &ev.paramL, &ev.paramH) == 3)
	{
		int dataOffset = -1;
		for (int i = 0; i < inputDataLength; i++)
		{
			if (inputData[i] == '\"')
			{
				dataOffset = i;
				break;
			}
		}
		if (dataOffset == -1)
		{
			ev.data = NULL;
			ev.dataLength = 0;
			return true;
		}
		
		if (inputData[inputDataLength - 1] != '\"')
		{
			return false;
		}
		ev.dataLength = (inputDataLength - dataOffset - 2) / 2;

		char c1, c2;
		ev.data = (unsigned char*)malloc(ev.dataLength);
			
		const char* src = inputData + dataOffset + 1;
		unsigned char* dst = ev.data;
		while ((*src) != '\"')
		{
			c1 = *src++; 
			if (c1 > '9') c1 -= 7; c1 -= '0';

			c2 = *src++; if (c2 == '\"') break;				
			if (c2 > '9') c2 -= 7; c2 -= '0';

			*dst++ = c1 * 16 + c2;
		}
		return true;
	}
	memset(&ev, 0, sizeof(ev));
	return false;
}
