# Timesafe

A simple command line tool for file encryption/decryption using AES-CBC-256 (openssl)

V1.0 - Created 28.Okt.2017 - sven.pohl@zen-systems.de 

Based partly on https://stackoverflow.com/questions/18152913/aes-aes-cbc-128-aes-cbc-192-aes-cbc-256-encryption-decryption-with-openssl-c
from "WhozCraig"


Usage
-----

```ruby
encrypt [file-to-encrypt] [new_name.alb] "[your-password]"
decrypt [encrypted_file.alb] [file-to-decryp†]  "[your-password]"

```

Example
-----

```ruby
$ encrypt secret.txt secret.alb "cat4sT$1022b"

later:

$ decrypt secret.alb secret.txt "cat4sT$1022b"


```

Description
-----
*Timesafe* uses a AES-CBC-256 function to protect you secret files with a password.
After file encoding the function will reload the created file, decrypt it, and check
the consistence of the encoded sha256-hash value in order to get sure, the encoding is
like expected.




System requirements
-----

gcc/c++ - compiler
openssl - libraries
