# LockDown 🔒

## File Encryption Program

LockDown is a file encryption program written by Ben Klein and Eitan Michaelov.
**The project runs only on Windows!**

## What did we implement in the project?

- AES 128 CBC
- KMS(Key Management Sever)
- Key Generator
- FrontEnd Client Application

## We used:

- PSQL DataBase -> KMS
- C -> AES and CBC implementation
- C++ -> Key Generator implementation
- C#/WPF -> FrontEnd Client
- C++ -> Server backend

## Installations For The Project To Run

- CIMG Library
- opencv Library
- PostgreSQL
- pqxx(PSQL Library for C++)

We suggest to download the libraries using VCPKG

## How To Run The Project

First the server(**LockDown.sln**) must be turned on and then the client(**LockDownClient/LockDownClient/LockDownClient.sln**). Afterwards you just choose a file to encrypt and you can decrypt
it later on :)
