#include "sha1.h"
#include <string.h>

#define rol(v,b) (((v) << (b)) | ((v) >> (32 - (b))))
#ifdef __BIG_ENDIAN__
#define B0(i) (buf[i] = buf[i])
#else
#define B0(i) (buf[i] = (((buf[i] & 0xff000000) >> 24) | ((buf[i] & 0x00ff0000) >> 8) |\
                         ((buf[i] & 0x0000ff00) << 8)  | ((buf[i] & 0x000000ff) << 24)))
#endif
#define B1(i) (buf[i & 15] = rol(buf[i & 15] ^ buf[(i-14) & 15] ^ buf[(i-8) & 15] ^ buf[(i-3) & 15], 1))
#define F0(x,y,z) ((x & (y ^ z)) ^ z)
#define F1(x,y,z) ( x ^ y ^ z )
#define F2(x,y,z) (((x | y) & z) | (x & y))
#define R0(a,b,c,d,e,i) e += F0(b,c,d) + B0(i) + 0x5A827999 + rol(a,5); b = rol(b,30);
#define R1(a,b,c,d,e,i) e += F0(b,c,d) + B1(i) + 0x5A827999 + rol(a,5); b = rol(b,30);
#define R2(a,b,c,d,e,i) e += F1(b,c,d) + B1(i) + 0x6ED9EBA1 + rol(a,5); b = rol(b,30);
#define R3(a,b,c,d,e,i) e += F2(b,c,d) + B1(i) + 0x8F1BBCDC + rol(a,5); b = rol(b,30);
#define R4(a,b,c,d,e,i) e += F1(b,c,d) + B1(i) + 0xCA62C1D6 + rol(a,5); b = rol(b,30);

void Sha1(const char* data, uint32_t dataLength, uint8_t digest[])
{
  Sha1_CTX ctx;
  Sha1Init(&ctx);
  Sha1Update(&ctx, data, dataLength);
  Sha1Final(&ctx, digest);
}

/* Hash a single 512-bit block. This is the core of the algorithm. */
void Sha1Transform(uint32_t state[5], const uint8_t data[64])
{
  uint32_t a, b, c, d, e;
  uint32_t buf[16];

  /* Copy context->state[] to working vars */
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];

  memcpy(buf, data, 64);

  /* 4 rounds of 20 operations each. Loop unrolled. */
  R0(a,b,c,d,e, 0);  
  R0(e,a,b,c,d, 1);  
  R0(d,e,a,b,c, 2);  
  R0(c,d,e,a,b, 3);
  R0(b,c,d,e,a, 4);  
  R0(a,b,c,d,e, 5);  
  R0(e,a,b,c,d, 6);  
  R0(d,e,a,b,c, 7);
  R0(c,d,e,a,b, 8);  
  R0(b,c,d,e,a, 9);  
  R0(a,b,c,d,e,10);  
  R0(e,a,b,c,d,11);
  R0(d,e,a,b,c,12);  
  R0(c,d,e,a,b,13);  
  R0(b,c,d,e,a,14);  
  R0(a,b,c,d,e,15);
  R1(e,a,b,c,d,16);  
  R1(d,e,a,b,c,17);  
  R1(c,d,e,a,b,18);  
  R1(b,c,d,e,a,19);
  R2(a,b,c,d,e,20);  
  R2(e,a,b,c,d,21);  
  R2(d,e,a,b,c,22);  
  R2(c,d,e,a,b,23);
  R2(b,c,d,e,a,24);  
  R2(a,b,c,d,e,25);  
  R2(e,a,b,c,d,26);  
  R2(d,e,a,b,c,27);
  R2(c,d,e,a,b,28);  
  R2(b,c,d,e,a,29);  
  R2(a,b,c,d,e,30);  
  R2(e,a,b,c,d,31);
  R2(d,e,a,b,c,32);  
  R2(c,d,e,a,b,33);  
  R2(b,c,d,e,a,34);  
  R2(a,b,c,d,e,35);
  R2(e,a,b,c,d,36);  
  R2(d,e,a,b,c,37);  
  R2(c,d,e,a,b,38);  
  R2(b,c,d,e,a,39);
  R3(a,b,c,d,e,40);  
  R3(e,a,b,c,d,41);  
  R3(d,e,a,b,c,42);  
  R3(c,d,e,a,b,43);
  R3(b,c,d,e,a,44);  
  R3(a,b,c,d,e,45);  
  R3(e,a,b,c,d,46);  
  R3(d,e,a,b,c,47);
  R3(c,d,e,a,b,48);  
  R3(b,c,d,e,a,49);  
  R3(a,b,c,d,e,50);  
  R3(e,a,b,c,d,51);
  R3(d,e,a,b,c,52);  
  R3(c,d,e,a,b,53);  
  R3(b,c,d,e,a,54);  
  R3(a,b,c,d,e,55);
  R3(e,a,b,c,d,56);  
  R3(d,e,a,b,c,57);  
  R3(c,d,e,a,b,58);  
  R3(b,c,d,e,a,59);
  R4(a,b,c,d,e,60);  
  R4(e,a,b,c,d,61);  
  R4(d,e,a,b,c,62);  
  R4(c,d,e,a,b,63);
  R4(b,c,d,e,a,64);  
  R4(a,b,c,d,e,65);  
  R4(e,a,b,c,d,66);  
  R4(d,e,a,b,c,67);
  R4(c,d,e,a,b,68);  
  R4(b,c,d,e,a,69);  
  R4(a,b,c,d,e,70);  
  R4(e,a,b,c,d,71);
  R4(d,e,a,b,c,72);  
  R4(c,d,e,a,b,73);  
  R4(b,c,d,e,a,74);  
  R4(a,b,c,d,e,75);
  R4(e,a,b,c,d,76);  
  R4(d,e,a,b,c,77);  
  R4(c,d,e,a,b,78);  
  R4(b,c,d,e,a,79);

  /* Add the working vars back into context.state[] */
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;

  /* Wipe variables */
  a = b = c = d = e = 0;
  memset(buf, 0, 64);
}


void Sha1Init(Sha1_CTX *sha)
{
    /* SHA1 initialization constants */
    sha->state[0] = 0x67452301;
    sha->state[1] = 0xEFCDAB89;
    sha->state[2] = 0x98BADCFE;
    sha->state[3] = 0x10325476;
    sha->state[4] = 0xC3D2E1F0;

    sha->count[0] = sha->count[1] = 0;
}

void Sha1Update(Sha1_CTX *sha, const uint8_t *data, uint32_t length)
{
    uint32_t buffer_offset = (sha->count[0] >> 3) & 63;
    uint32_t data_offset = 0;

	/* data are being added to the buffer until buffer is full */
    if (length + buffer_offset >= 64) 
    {
        data_offset = 64 - buffer_offset;
        memcpy(&sha->buf[buffer_offset], data, data_offset);
        Sha1Transform(sha->state, sha->buf);
        while (data_offset + 64 <= length) 
        { 
            Sha1Transform(sha->state, &data[data_offset]);
            data_offset += 64;
        }
        buffer_offset = 0;
    }
    memcpy(&sha->buf[buffer_offset], &data[data_offset], length - data_offset);

    /* update data counter */
    buffer_offset = sha->count[0];
    sha->count[0] += length << 3;
    if (sha->count[0] < buffer_offset) sha->count[1]++;
    sha->count[1] += length >> 29;
}

void Sha1Final(Sha1_CTX *sha, uint8_t digest[SHA1_DIGEST_LENGTH])
{
    uint8_t d, counter[8];

    /* setup counter */
    for (d = 0; d < 4; d++) 
    {
        counter[d]   = (sha->count[1] >> (24 - d * 8)) & 0xff;
        counter[d+4] = (sha->count[0] >> (24 - d * 8)) & 0xff;
    }

    /* add padding */
    d = 0x80;
    Sha1Update(sha, &d, 1);

    d = 0x00;
    while ((sha->count[0] & (63 * 8)) != (56 * 8)) 
    {
        Sha1Update(sha, &d, 1);
    }

    /* add counter */
    Sha1Update(sha, counter, 8);
    for (d = 0; d < SHA1_DIGEST_LENGTH; d++) 
    {
        digest[d] = (uint8_t)(sha->state[d >> 2] >> (8 * (3 - (d & 3))) & 0xff);
    }

    /* overwrite all used variables */
    memset(sha, 0, sizeof(*sha));
    memset(counter, 0, sizeof(counter));
}
