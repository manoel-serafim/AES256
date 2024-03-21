# Optimized AES-256 implementation
This AES-256 library is developed to function without reliance on external resources, utilizing proprietary code for all mathematical operations. Tailored adjustments enable its seamless integration into BareMetal systems without hardware acceleration or dynamic variables. The implementation prioritizes efficiency by minimizing function calls, optimizing looping assembly overheads, and choosing between lookup tables and mathematical functions. Thorough testing against FIPS-197 test vectors ensures compliance and accuracy.

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

## Dive into History

In 1997, the National Institute of Standards and Technology (NIST) initiated an open, administered process to assess and define a new Advanced Encryption Standard (AES). This call resulted in a staggering 15 submissions in 1998.

The following requirements for all AES candidate submissions were mandatory:
- 128 bit block cipher
- three key legths to be supported: 128, 192 and 256 bit
- efficiency in software and hardware
- relative security 

On August 9, 1999, NIST announced the selection of five finalist algorithms:
- Mars by IBM Corporation
- RC6 by RSA Laboratories
- [Rijndael, by Joan Daemen and Vincent Rijmen](https://csrc.nist.gov/csrc/media/projects/cryptographic-standards-and-guidelines/documents/aes-development/rijndael-ammended.pdf)
- Serpent, by Ross Anderson, Eli Biham, and Lars Knudsen
- Twofish, by Bruce Schneier

In 2001, after extensive review by NIST and the international scientific community, Rijndael emerged as the new AES, as specified in [FIPS PUB 197](http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf).

By 2003, the National Security Agency (NSA) permitted the use of AES to encrypt TOP SECRET classified documents for key lengths of either 192 or 256 bits. This marked a significant shift, as previously only non-public algorithms had been employed for this purpose.

Post-2005, a notable increase in side-channel attacks occurred following the disclosure of a cache-timing attack by Daniel J. Bernstein. This technique exploited data leakage to compromise a custom OpenSSL AES encryption, necessitating over 200 million chosen plaintexts. Notably, the server was intentionally designed to provide maximum timing information, exacerbating the vulnerability.

## AES Design Rationale

The Advanced Encryption Standard (AES) incorporates several design principles to ensure robust security and efficient encryption:

- **Round Transformation Composition**: AES employs a round transformation composed of three distinct invertible uniform transformations. This approach enhances security while allowing for efficient encryption and decryption processes.

- **Linear Mixing Layer**: A critical component of AES, the linear mixing layer ensures high diffusion over multiple rounds. This layer contributes to the algorithm's resistance against differential and linear cryptanalysis.

- **Non-linear Layer (S-Box)**: AES integrates a non-linear layer, achieved through parallel application of S-boxes with optimal worst-case nonlinearity properties. This non-linearity adds complexity to the encryption process, enhancing the algorithm's resistance against attacks.

- **Key Addition Layer**: In the key addition layer, a simple XOR operation is performed between the round key and the intermediate state. This straightforward operation ensures that each round of encryption introduces unique transformations to the data.

## Technical Details of AES

Unlike Feistel networks, which operate on half-blocks, AES encrypts entire blocks per iteration. AES comprises three primary layers, each manipulating all 128 bits of the algorithm state:

- **Key Addition Layer**: In this layer, a 128-bit round key, derived from the main key during the key schedule, is XORed with the state, introducing key-dependent transformations.

- **Byte Substitution Layer (S-Box)**: Each element within the state undergoes a nonlinear transformation using lookup tables possessing special mathematical properties. This nonlinear transformation enhances confusion in the data, crucial for robust encryption.

- **Diffusion Layer**: The diffusion layer facilitates dispersion of data across all state bits, crucial for preventing patterns from emerging in the ciphertext. This layer includes sublayers such as ShiftRows and MixColumn, performing linear operations to ensure comprehensive diffusion.

### Finite Fields AKA Galois Fields (the G of GCM)

Finite fields, also known as Galois fields, play a pivotal role in AES, particularly in the S-Box and the diffusion layer. Finite fields are sets with a finite number of elements where arithmetic operations like addition, subtraction, multiplication, and inversion are defined. These fields exhibit properties of groups and fields, ensuring the requisite mathematical properties for secure encryption. Finite fields are foundational to AES, contributing to its resilience against various cryptographic attacks.

#### Group
Is a set with one operation and the corresponding inverse operation. It has this following properties:
1. The group operation ◦ is closed. That is, for all a,b,∈ G, it holds that a ◦ b = c ∈ G.
2. The group operation is associative. That is, a◦(b◦c) (a◦b)◦c for all a,b,c ∈ G.
3. There is an element 1 ∈ G, called the neutral element (or identity element), such that a ◦ 1 = 1 ◦ a = a for all a ∈ G.
4. For each a ∈ G there exists an element a−1 ∈ G, called the inverse of a, such that a ◦ a−1 = a−1 ◦ a = 1.
5. A group G is abelian (or commutative) if, furthermore, a ◦ b = b ◦ a for all a,b ∈ G.

#### Field
A field F is a set of elements with the following properties:
1. All elements of F form an additive group with the group operation “+” and the neutral element 0.
2. All elements of F except 0 form a multiplicative group with the group operation “×” and the neutral element 1.
3. When the two group operations are mixed, the distributivity law holds, i.e., for all a,b,c ∈ F: a(b+c)=(ab)+(ac)

A finite field is just a field with a finite number of elements.😲

### Cardinality Importance

A finite field with an order of \( m \) exists only if \( m \) is a prime power, represented as \( m = p^n \), where \( p \) is a prime integer and \( n \) is a positive integer. \( p \) is termed the characteristic of the finite field. This implies that finite fields can have elements like 5, 25, 125, 3, 9, 27, or 81, but not 15 elements.

### Prime Fields

A Galois Field with a prime number of elements, denoted as \( GF(p) \), encompasses elements ranging from 0 to \( p-1 \). For instance, \( GF(2) \) includes elements 0 and 1.

**Addition:** In \( GF(p) \), addition is computed as \( A + B = (A + B) \mod p \). In \( GF(2) \), addition translates to the XOR operation.

**Multiplication:** Multiplication in \( GF(p) \) follows \( A \times B = (A \times B) \mod p \). In \( GF(2) \), multiplication corresponds to the AND operation.

### Extension Fields

Extension Fields, denoted as \( GF(p^m) \), emerge from Galois Fields raised to a prime power exponent. Elements in \( GF(p^m) \) are represented not as integers but as polynomials with coefficients in \( GF(p) \). In \( GF(2^8) \), used in AES, each element \( A \) in \( GF(2^8) \) is represented as a polynomial: \( A(x) = a_7x^7 + a_6x^6 + \ldots + a_1x + a_0 \), where \( a_i \in GF(2) \).

**Addition:** The key addition layer in AES involves adding polynomials and computing the modulo \( p \) of each coefficient.

\[ \sum_{i=0}^{m-1} c_i x^i , \quad c_i \equiv a_i + b_i \mod 2 \]

**Multiplication:** Multiplication in \( GF(2^8) \) underpins the MixColumn operation. It necessitates irreducible polynomials, akin to prime numbers, to minimize factors. The core steps involve plain polynomial product calculation followed by modulo reduction using the irreducible polynomial.

**Inversion:** In \( GF(2^8) \), inversion is central to the Byte Substitution transformation in AES. The inverse of a nonzero element \( A \) in \( GF(2^m) \) is computed using the [Extended Euclidean Algorithm](https://www.youtube.com/watch?v=qqvtGGKQTLI&list=PLKXdxQAT3tCssgaWOy5vKXAR4WTPpRVYK&index=24).

S-Box Pre computed Inverse Lookup Table
X/Y  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
0   00 01 8D F6 CB 52 7B D1 E8 4F 29 C0 B0 E1 E5 C7
1   74 B4 AA 4B 99 2B 60 5F 58 3F FD CC FF 40 EE B2
2   3A 6E 5A F1 55 4D A8 C9 C1 0A 98 15 30 44 A2 C2
3   2C 45 92 6C F3 39 66 42 F2 35 20 6F 77 BB 59 19
4   1D FE 37 67 2D 31 F5 69 A7 64 AB 13 54 25 E9 09
5   ED 5C 05 CA 4C 24 87 BF 18 3E 22 F0 51 EC 61 17
6   16 5E AF D3 49 A6 36 43 F4 47 91 DF 33 93 21 3B
7   79 B7 97 85 10 B5 BA 3C B6 70 D0 06 A1 FA 81 82
8   83 7E 7F 80 96 73 BE 56 9B 9E 95 D9 F7 02 B9 A4
9   DE 6A 32 6D D8 8A 84 72 2A 14 9F 88 F9 DC 89 9A
A   FB 7C 2E C3 8F B8 65 48 26 C8 12 4A CE E7 D2 62
B   0C E0 1F EF 11 75 78 71 A5 8E 76 3D BD BC 86 57
C   0B 28 2F A3 DA D4 E4 0F A9 27 53 04 1B FC AC E6
D   7A 07 AE 63 C5 DB E2 EA 94 8B C4 D5 9D F8 90 6B
E   B1 0D D6 EB C6 0E CF AD 08 4E D7 E3 5D 50 1E B3
F   5B 23 38 34 68 46 03 8C DD 9C 7D A0 CD 1A 41 1C
The S-box is composed of this plus a step called affine mapping.

### Internal Structure of AES

At a high level, AES operates on a 16-byte input plaintext and a key of 128, 192, or 256 bits to produce an output. The number of rounds varies based on the key length: 128-bit key requires 10 rounds, 192-bit key requires 12 rounds, and 256-bit key requires 14 rounds. Unlike a Feistel network, AES encrypts all 128 bits in a single round, introducing the concept of key whitening by adding a subkey at the start and end of each round.


#### The AES Encryption Round
Each AES round (except the last) consists of four layers and starts with 16 bytes of input [16 elements of G(2⁸)]:
1.  This is fed byte-wise into 16 S-Boxes (Byte Substitution Layer) [Inversion] - Confusion
2.  The output from the substitution boxes is permuted byte-wise in the ShiftRows [Permutation] - Diffusion
3.  Then 4 blocks of 4 bytes are fed into the MixedColumn transformation [Multiplication] - Diffusion
4.  Finally the 128-bit subkey is XORed with the intermediate result.[XOR/Addition]

Details:

1. **Byte Substitution Layer**: In this layer, 16 S-boxes are applied in parallel to each byte of the input. These S-boxes perform a nonlinear substitution using an inversion lookup table for \( GF(2^8) \), ensuring confusion in the data.
   
2. **Diffusion Layer**:
   - **Shift Rows**: Bytes within each row of the state are circularly shifted to the left, introducing permutation.
   - **Mix Column**: Each column of the state undergoes a matrix operation in \( GF(2^8) \), contributing to diffusion.

3. **Key Addition Layer**: The intermediate result from the MixColumn is XORed with a 128-bit subkey, adding another layer of security through key mixing.


#### Byte Substitution Layer

The Byte Substitution Layer employs a substitution box (S-box) to replace each byte based on an inversion lookup table for \( GF(2^8) \). This non-linear operation ensures optimal defense against analytical attacks, particularly when combined with an affine step that disrupts the algebraic structure inherent in the Galois field.
Here each state byte is replaced based on the inversion table for GF(2⁸):

X/Y  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
0   63 7C 77 7B F2 6B 6F C5 30 01 67 2B FE D7 AB 76
1   CA 82 C9 7D FA 59 47 F0 AD D4 A2 AF 9C A4 72 C0
2   B7 FD 93 26 36 3F F7 CC 34 A5 E5 F1 71 D8 31 15
3   04 C7 23 C3 18 96 05 9A 07 12 80 E2 EB 27 B2 75
4   09 83 2C 1A 1B 6E 5A A0 52 3B D6 B3 29 E3 2F 84
5   53 D1 00 ED 20 FC B1 5B 6A CB BE 39 4A 4C 58 CF
6   D0 EF AA FB 43 4D 33 85 45 F9 02 7F 50 3C 9F A8
7   51 A3 40 8F 92 9D 38 F5 BC B6 DA 21 10 FF F3 D2
8   CD 0C 13 EC 5F 97 44 17 C4 A7 7E 3D 64 5D 19 73
9   60 81 4F DC 22 2A 90 88 46 EE B8 14 DE 5E 0B DB
A   E0 32 3A 0A 49 06 24 5C C2 D3 AC 62 91 95 E4 79
B   E7 C8 37 6D 8D D5 4E A9 6C 56 F4 EA 65 7A AE 08
C   BA 78 25 2E 1C A6 B4 C6 E8 DD 74 1F 4B BD 8B 8A
D   70 3E B5 66 48 03 F6 0E 61 35 57 B9 86 C1 1D 9E
E   E1 F8 98 11 69 D9 8E 94 9B 1E 87 E9 CE 55 28 DF
F   8C A1 89 0D BF E6 42 68 41 99 2D 0F B0 54 BB 16

This inversion lookup table was computed based on the Extended Field inversion and then the affine mapping of each input element of GF(2⁸). The affine mapping is done in order to destroy all the structured mathematical properties.

#### Diffusion Layer

The Diffusion Layer spreads the influence of individual bits across the entire state linearly:
- **Shift Rows**: Rotates bytes within each row of the state matrix, contributing to permutation. [Permute]
- **Mix Column**: Executes a matrix operation in \( GF(2^8) \) on each column of the state matrix, promoting diffusion. [Galois Field Matrix Multiplication]

#### Key Addition Layer

In the Key Addition Layer, the intermediate result from the MixColumn is combined with a subkey via XOR operation, enhancing security through key mixing.

#### Key Scheduler

To incorporate a subkey in each round, the AES algorithm generates \( Nr + 1 \) subkeys, enabling key whitening for the block cipher. This iterative process ensures robust encryption by varying the key at each stage of the encryption process.

This inversion lookup table was computed based on the Extended Field inversion and then the affine mapping of each input element of GF(2⁸). The affine mapping is done in order to destroy all the structured mathematical properties.


#### Decryption
Just do the same, but in the inverse direction. (See the comments on the code to get to know more)

## Lessons Learned:
* While in this project I almost implemented lookup tables for the multiplication:
> This was the wrong approach because it takes more space and the multiplication in the GF(2⁸) is really simple and based on shifts and xors (not as computationally heavy as memory access)
* AES is really simple:
> optimizing is really tricky
> optimizing math code is very fun 🙂


