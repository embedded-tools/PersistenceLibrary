/*
 * Persistence Library / Persistence / ESerializationMode
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

#ifndef TSERIALIZATIONMODE___H
#define TSERIALIZATIONMODE___H

enum ESerializationMode
{     
   esmXMLOutput,        //writing configuration data to xml stream
   esmXMLInput,         //reading configuration data from xml parser
   esmBinaryOutput,     //writing configuration data to binary stream
   esmBinaryInput,      //reading configuration data from binary stream
   esmXSDOutput         //output to XSD file (description of xml and binary format)
};

#endif
