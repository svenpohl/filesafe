//
//
// FILESAFE file encryption - sven.pohl@zen-systems.de - https://github.com/svenpohl
// Created 28.Okt.2017 
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesafe.h"


//
//
// main 
//
//
int main(int argc, char **argv)
{
char     filename_in [256];    
char     filename_out[256];    
char     password    [256];
filesafe fs;

printf("# --- \n");        
printf("# FILESAFE %s file encryption - MIT License\n",FILESAFE_VERSION);
printf("# 2017 by sven.pohl@zen-systems.de - https://github.com/svenpohl \n");    
printf("# --- \n");

if (REMOVE_PLAINTEXT_FILE_AFTER_ENCODING)
   {
   printf("!!! Plaintext will be removed after encoding !!!\n\n");
   }

if (argc < 4)
   {
   printf("usage:\n");   
   printf("encrypt [file-to-encrypt] [new_name.alb] \"[your-password]\"\n");
   printf("\n");
   exit(0);
   }
     
     
strcpy(password,argv[3]);
if ( strlen(password) < 6 )
   {       
   printf("Password [%s] to short!\n",password);
   return(0);
   }
    
strcpy(filename_in, argv[1]);
strcpy(filename_out, argv[2]);
    
    
int ret = fs.encrypt( filename_in, filename_out, password );
if (ret)
   {
   printf("OK! -> File %s successfully encrytpted to %s\n\n ",filename_in, filename_out);
   } else
     {
     printf("Error!\n");
     }
   
return 0;
} // int main()
