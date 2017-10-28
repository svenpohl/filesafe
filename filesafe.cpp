//
//
// filesafe.cpp
//
// Class for encryption and decryption files by AES-CBC-256
//
// Based partly on https://stackoverflow.com/questions/18152913/aes-aes-cbc-128-aes-cbc-192-aes-cbc-256-encryption-decryption-with-openssl-c
// from "WhozCraig"
//
// V1.0 - Created 28.Okt.2017 - sven.pohl@zen-systems.de 
//
//
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "filesafe.h"



//
//
// sha256 - calculates the sha256 hashvalue
//
//
void filesafe::sha256(unsigned char *string, long length, char outputBuffer[65])
{
unsigned char hash[SHA256_DIGEST_LENGTH];
SHA256_CTX sha256;
SHA256_Init(&sha256);
SHA256_Update(&sha256, string, length);
SHA256_Final(hash, &sha256);
int i = 0;
for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
    sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
outputBuffer[64] = 0;
} //// sha256



//
//
// a simple hex-print routine. could be modified to print 16 bytes-per-line
//
//
void filesafe::hex_print(const void* pv, size_t len)
{
const unsigned char * p = (const unsigned char*)pv;
    if (NULL == pv)
        printf("NULL");
    else
    {
        size_t i = 0;
        for (; i<len;++i)
            printf("%02X ", *p++);
    }
    printf("\n");
} // hex_print



//
//
// encrypt - file encryption via AES-CBC-256.
//
//
int filesafe::encrypt( char *filename_in, char *filename_out, char *password )
{
int  ret = 0;
int  keylength;
char buffer_fsize[256];
char buffer_hash[256];
    
        
FILE *fp = fopen(filename_in, "rb");
    
if (fp != NULL)
   { 
  
   //  
   // Get size of file.
   //
   fseek(fp, 0, SEEK_END);
   long fsize = ftell(fp);
   fseek(fp, 0, SEEK_SET);
       
   if ( fsize <= MAX_FILESIZE )
      {
          
      sprintf(buffer_fsize,"%020d",fsize);                            
      fsize = atof(buffer_fsize);

      unsigned char aes_input[fsize+10];
          
      //
      // Prepare password, generate a key with a given length 
      //
      
      keylength = 256;
      unsigned char aes_key[keylength/8];  
      memset(aes_key, 0, keylength/8);
      strncpy((char*)aes_key,password, strlen(password) );

      //
      // Read the plaintext file.
      //
      fread(aes_input, fsize, 1, fp);                  
      fclose(fp);
                
      sha256(aes_input, fsize, buffer_hash);
          
      FILE *fpout = fopen(filename_out, "wb");

          //
          // Write 20 bytes fsize
          //
          fwrite(buffer_fsize, sizeof(char),strlen(buffer_fsize), fpout);

          //
          // Write sha256 hash
          //
          fwrite(buffer_hash, sizeof(char),strlen(buffer_hash), fpout);

          //
          // Encryption
          //
          
          //
          // init vector 
          //
          unsigned char iv_enc[AES_BLOCK_SIZE];
          unsigned char  iv_dec[AES_BLOCK_SIZE];
          RAND_bytes(iv_enc, AES_BLOCK_SIZE);
          memcpy(iv_dec, iv_enc, AES_BLOCK_SIZE);

          //
          // buffers for encryption and decryption
          //
          const size_t encslength = ((fsize + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
          unsigned char enc_out[encslength];          
          memset(enc_out, 0, sizeof(enc_out));
          //
          // so i can do with this aes-cbc-128 aes-cbc-192 aes-cbc-256
          //
          AES_KEY enc_key, dec_key;
          AES_set_encrypt_key(aes_key, keylength, &enc_key);                   
          AES_cbc_encrypt(aes_input, enc_out, fsize, &enc_key, iv_enc, AES_ENCRYPT);


          //
          // Write iv_enc
          //         
          fwrite(iv_dec, sizeof(char),AES_BLOCK_SIZE, fpout);
                                              
          //
          // Write encrypted data
          //
          fwrite(enc_out, sizeof(char),sizeof(enc_out), fpout);

          fclose(fpout);
        
          
          //
          // Now check decryption.
          //       
          int ret = decrypt( filename_out, password, NULL );
          
          if ( ret == 1 )
             {
             
             if ( REMOVE_PLAINTEXT_FILE_AFTER_ENCODING )
                {
                if ( unlink (filename_in) == 0)
                   {
                   printf("Plaintext file [%s] successfully removed! \n", filename_in);
                   }
                }  
             
             return 1;   
             } // if ...
        
      
      } // if (fsize <= MAX_FILESIZE)
      else
         {
         printf("File is to big (max:%d) \n",MAX_FILESIZE);
         return 0;
         }

   return 0;
   } // if (fp != NULL)
   else
       {
       printf("File [%s] not found!\n",filename_in);
       return 0;
       }

return(0);
} // encrypt








//
//
// decrypt - file decryption via AES-CBC-256.
//
//
int filesafe::decrypt( char *filename, char *password, char *filename_encrypted )
{
int  ret = 0;
int  keylength;
char buffer_fsize[256];
char buffer_hash[256];
char buffer_hash_check[256];
long fsize;
long fsize2;

    
//
// Prepare password and generate a key with a given length. 
//
keylength = 256;
unsigned char aes_key[keylength/8];  
memset(aes_key, 0, keylength/8);
strncpy((char*)aes_key,password, strlen(password) );

FILE *fp = fopen(filename, "rb");
    
if (fp != NULL)
   {              
   fseek(fp, 0, SEEK_END);
   long fsize = ftell(fp);
   fseek(fp, 0, SEEK_SET);
   
   fread(buffer_fsize, 20, 1, fp);       
   buffer_fsize[20] = '\0';           

   fread(buffer_hash, 64, 1, fp);                  
   buffer_fsize[64] = '\0';           

   unsigned char iv_dec[AES_BLOCK_SIZE];
   fread(iv_dec, AES_BLOCK_SIZE, 1, fp); 
   //hex_print(iv_dec, AES_BLOCK_SIZE);

   unsigned char enc_out[fsize - 20-64];   
   fread(enc_out, sizeof(enc_out), 1, fp); 
   
   
   fsize2 = atof(buffer_fsize);
   unsigned char dec_out[fsize2];
   
   memset(dec_out, 0, sizeof(dec_out));

      
   AES_KEY enc_key, dec_key;
   AES_set_decrypt_key(aes_key, keylength, &dec_key);
   AES_cbc_encrypt(enc_out, dec_out, fsize2, &dec_key, iv_dec, AES_DECRYPT);
   
   
   //
   // Check hashvalue
   //
   sha256(dec_out, fsize2, buffer_hash_check);

          
   if ( strcmp( buffer_hash, buffer_hash_check ) == 0)
      {      
      ret = 1;
                  
      if (filename_encrypted != NULL)
         {            
         FILE *fpout = fopen(filename_encrypted, "wb");
         fwrite(dec_out, sizeof(char),fsize2, fpout);
         fclose(fpout);                           
         } // if (filename_encrypted != NULL)
      
      } // if [hash identical]
 
   
   fclose(fp);   
   } // if (fp != NULL)
   else 
      {
      ret = 0;
      }

return(ret);
} // decrypt



 