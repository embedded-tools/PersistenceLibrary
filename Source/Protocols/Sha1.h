#ifndef SHA1___H
#define SHA1___H

#if _MSC_VER && (_MSC_VER<1600)
  typedef unsigned char uint8_t;
  typedef unsigned long uint32_t;
#else 
  #include <stdint.h>
#endif

#define SHA1_DIGEST_LENGTH 20

typedef struct 
{
  uint32_t state[5];
  uint32_t count[2];
  uint8_t  buf[64];
} Sha1_CTX;

void Sha1(const char* data, uint32_t dataLength, uint8_t digest[]);

void Sha1Init(Sha1_CTX *sha);
void Sha1Update(Sha1_CTX *sha, const uint8_t *data, uint32_t length);
void Sha1Final(Sha1_CTX *sha, uint8_t digest[SHA1_DIGEST_LENGTH]);

#endif
