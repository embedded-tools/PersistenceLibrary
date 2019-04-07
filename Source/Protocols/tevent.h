#ifndef TEVENT___H
#define TEVENT___H

struct TEvent
{	
	int				eventID;
	int				paramL;
	int			    paramH;	
	unsigned char*  data;
	int				dataLength;	
};

#endif
