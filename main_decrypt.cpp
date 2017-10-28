//
//
// FILESAFE file decryption - sven.pohl@zen-systems.de - https://github.com/svenpohl
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
printf("# FILESAFE %s file decryption - MIT License\n",FILESAFE_VERSION);
printf("# 2017 by sven.pohl@zen-systems.de - https://github.com/svenpohl \n");    
printf("# --- \n");        

if (argc < 4)
   {
   printf("usage:\n");
   printf("decrypt [encrypted_file.alb] [file-to-decryp†]  \"[your-password]\"\n");
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
    
    
//
// Decryption
//
       
int ret = fs.decrypt( filename_in, password, filename_out );
if (ret)
   {
   printf("OK! -> File %s successfully decrytpted to %s\n\n ",filename_in, filename_out);
   } else
     {
     printf("Error!\n");
     }
           
    
    
return 0;
} // main()