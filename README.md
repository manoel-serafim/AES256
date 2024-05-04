# Optimized AES-256 implementation
This is one of my big projects, in here I have developed a cryptographic library 

This cryptographic library is developed to function without reliance on any external resources, utilizing my own code for all mathematical operations, random number generation, cipher operation modes, cipher algorithms, etc. It will be easy to integrate this into BareMetal systems without hardware acceleration or dynamic allocation. The implementation minimizes function calls, optimizes looping assembly overheads, and will choose the best approach between between lookup tables and mathematical functions. A lot of testing has to go into into the cipher operation modes and the random number generators.



**Usage Instructions:**

To utilize the AES-256 encryption and decryption functions , follow these simple steps:

1. **Include Header File:**
   ```c
   #include "encryption.h"
   ```
   or:
   ```c
   #include "decryption.h"
   ```
   
3. **Encryption:**
   - Use the `encrypt` function to encrypt plaintext using a 256-bit key.
   - Pass the plaintext (`plain_text`) and the 256-bit key (`key`) as arguments.
   - Ensure that the plaintext is in a 16-byte array format and the key is in a 32-byte array format.
   
   Example:
   ```c
   BYTE plain_text[16] = { /* fill with plaintext */ };
   BYTE key[32] = { /* fill with key */ };
   encrypt(plain_text, key);
   ```

4. **Decryption:**
   - Use the `decrypt` function to decrypt ciphertext encrypted with AES-256.
   - Pass the ciphertext (`cipher_text`) and the 256-bit key (`key`) as arguments.
   - Ensure that the ciphertext is in a 16-byte array format and the key is in a 32-byte array format.
   
   Example:
   ```c
   BYTE cipher_text[16] = { /* fill with ciphertext */ };
   BYTE key[32] = { /* fill with key */ };
   decrypt(cipher_text, key);
   ```
To compile the library just use the command "make"

## Motivation

I embarked on this project driven by several key factors:

0. **Optimization Beyond Compiler Flags**: I observed a lack of optimized implementations of the AES algorithm tailored to the level of performance I desired, particularly for single-core CPUs. While compiler optimization flags like `-O4` exist, I aimed to craft an implementation where I had complete control over resource utilization. My encryption object occupies only 5880 bytes, ensuring efficient use of memory resources. Moreover, my implementation doesn't rely on assumptions of dynamic allocation support in the system. It's finely tuned to minimize looping and function calling overheads, making it suitable for integration into bare-metal embedded systems. Additionally, I exclusively utilize standard 32 and 8-bit data types for broad compatibility.

1. **Significance of AES**: AES stands as one of the paramount symmetric algorithms globally. Its pervasive use underscores its importance in safeguarding digital communications and data transfers.

2. **Embedded System Optimization**: Optimized AES implementations for embedded systems typically demand minimal code space, often under 10 kilobytes. My endeavor aimed to deliver a lean yet potent AES implementation suitable for resource-constrained embedded environments.

3. **Global Encryption Standard**: AES serves as the cornerstone of encryption in a multitude of TLS and SSL connections worldwide. Its adoption secures trillions of data transmissions and communications daily, underscoring its critical role in cybersecurity.

4. **NIST Certification**: Over 80% of encryption products certified by the National Institute of Standards and Technology (NIST) for government use integrate AES. Its widespread acceptance in security protocols underscores its reliability and trustworthiness.

6. **Transparency and Open Development**: AES represents one of the pioneering openly developed cryptographic algorithms, embodying principles of transparency and collaborative development.

7. **Acknowledgment by NSA**: The adoption of AES by the National Security Agency (NSA) further underscores its robustness and reliability. Encrypted data disseminated by the NSA, viewable on platforms like WikiLeaks, serves as a testament to its real-world application and efficacy.

## Lessons Learned:
* While in this project I almost implemented lookup tables for the multiplication:
> This was the wrong approach because it takes more space and the multiplication in the GF(2⁸) is really simple and based on shifts and xors (not as computationally heavy as memory access)
* AES is really simple:
> optimizing is really tricky
> optimizing math code is very fun 🙂


