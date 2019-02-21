# PersistenceLibrary

C++ library designed especially for embedded system with very limited resources, e.g. 32kB RAM.

Library is divided to more parts:

-BasicTypes (optimized for Atmel AVR)
  -TString - simple string with possibility to allocate memory in both ways - statically or dynamically
  -TStringList - class for working with list of strings in one memory block
  -TParamString - class for parsing multiple parameters from one string, e.g. "TEXT=\"Hello\";AUDIO=Ding.wav;VIDEO=\"Dong.avi\"";
  -TFilePath - string class with additional functions for extracting file path, combining file paths etc.
  -TDate, TTime, TDateTime - classes for date time calculations
 
-Collections - container classes with both STL like interface and .NET like interface. All classes are designed to achieve low memory fragmentation.
  -TDictionary - dictionary compatible with STL dictionary template
  -TList       - list compatible with STL vector/list template
  -TObjectList - list designed for storing class instances
  -TStack      - class compatible with STL stack
  -TQueue      - class compatible with STL queue
 
 -FileUtils 
   a) classes for reading and writing data from/to generic source.   
     -TFileStream - reading/writing data from/to file
     -THandleStream - reading/writing data from generic source (e.g. COM port on Windows)
     -TRomStream - reading data from specific address (read only)
     -TMemoryStream - reading/writing data from/to specific address
     -TEEPROMStream - reading/writing data via specific callback function     
   b) stream decorators
     -TCachedStream - cache - forces reading/writing data through cache, e.g. TEEpromStream+TCacheStream can be used for writing data to Flash memory - data are not written to Flash until cache is full.
     -TLZ77Stream - contains internal cache. When cache is full, compression algorithm is used and compressed data are written to parent stream. Reading works in opposite way - cache is filled by data, compression algorithm is used to decompress data and data are read from this cache.
   c) JsonDoc class - class for loading json file to memory. You can choose whether you want to use static memory allocation (TJsonTagStaticPool) or dynamic memory allocation (TJsonTagDynamicPool)
   d) XmlDoc class - class for loading xml file to memory. Class uses simplified XPath syntax for searching tags in XML doc. You can choose whether you want to use static memory allocation (TJsonTagStaticPool) or dynamic memory allocation (TJsonTagDynamicPool)
  
