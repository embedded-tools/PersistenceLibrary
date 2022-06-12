#ifndef TLOG___H
#define TLOG___H

extern void LOG_DEBUG(void* objectId, const char* message, long arg = -2147483647);
extern void LOG_INFO(void* objectId, const char* message, long arg = -2147483647);
extern void LOG_WARNING(void* objectId, const char* message, long arg = -2147483647);
extern void LOG_ERROR(void* objectId, const char* message, long arg = -2147483647);
extern void LOG_EXCEPTION(void* objectId, const char* message, long arg = -2147483647);
    
#endif
