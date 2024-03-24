# Block Cipher Modes of Operation

Implementations are based on the [FIPS140-2](https://csrc.nist.gov/csrc/media/projects/cryptographic-module-validation-program/documents/fips140-2/fips1402ig.pdf), [Recommendation for Block Cipher Modes of Operation: Galois/Counter Mode (GCM) and GMAC](https://doi.org/10.6028/NIST.SP.800-38D) and the [Recommendation for Block Cipher Modes of Operation: Methods and Techniques](https://doi.org/10.6028/NIST.SP.800-38A)

A block cipher is a versatile cryptographic tool, essential for encryption and beyond. It enables various encryption methods, including block-based and stream ciphers, discussed in this chapter. Additionally, block ciphers are pivotal in constructing hash functions, MACs, and key establishment protocols, covered in subsequent chapters. They also serve as pseudo-random generators, highlighting their multifaceted utility in modern cryptography. Here there will be an implementation of the following block Cipher Operation Modes:

* Cipher Block Chaining mode (CBC),
* Counter mode (CTR),
* Galois Counter Mode (GCM).

## CBC
The Cipher Block Chaining (CBC) mode represents an enhancement over the Electronic CodeBook (ECB) mode, primarily through the introduction of an initialization vector (IV) which serves to randomize the encryption process, thereby mitigating the effectiveness of statistical analysis on encrypted blocks. Despite the introduction of randomized variability by the CBC mode, it does not inherently guarantee data integrity. Consequently, random bit flips within the encrypted data stream can lead to unpredictable and unauthorized behavior.

```
Encryption (first block): y1 = ek(x1 ⊕ IV)
Encryption (general block): yi = ek(xi ⊕ yi−1), i ≥ 2
Decryption (first block): x1 = e^(-1)_k (y1 ⊕ IV)
Decryption (general block): xi = e^(-1)_k (yi ⊕ yi−1), i ≥ 2
```
