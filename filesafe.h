//
//
// filesafe.h
// 
// Class for encryption and decryption files by AES-CBC-256 
//
// Based partly on https://stackoverflow.com/questions/18152913/aes-aes-cbc-128-aes-cbc-192-aes-cbc-256-encryption-decryption-with-openssl-c
// from "WhozCraig"
//
// V1.0 - Created 28.Okt.2017 - sven.pohl@zen-systems.de 
// 
//

#define FILESAFE_VERSION "V1.0"
#define MAX_FILESIZE 1048576
#define REMOVE_PLAINTEXT_FILE_AFTER_ENCODING 0

#include <unistd.h>
#include <stddef.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

class filesafe
{
public:

void   sha256(unsigned char *string, long length, char outputBuffer[65]);
void   hex_print(const void* pv, size_t len);
int    encrypt( char *filename_in, char *filename_out, char *password );
int    decrypt( char *filename, char *password, char *filename_encrypted );

}; // class filesafe
