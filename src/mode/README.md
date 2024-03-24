# Block Cipher Modes of Operation

A block cipher is a versatile cryptographic tool, essential for encryption and beyond. It enables various encryption methods, including block-based and stream ciphers, discussed in this chapter. Additionally, block ciphers are pivotal in constructing hash functions, MACs, and key establishment protocols, covered in subsequent chapters. They also serve as pseudo-random generators, highlighting their multifaceted utility in modern cryptography. Here there will be an implementation of the following block Cipher Operation Modes:

* Cipher Block Chaining mode (CBC),
* Counter mode (CTR),
* Galois Counter Mode (GCM).

## CBC
```
Encryption (first block): y1 = ek(x1 ⊕ IV)
Encryption (general block): yi = ek(xi ⊕ yi−1), i ≥ 2
Decryption (first block): x1 = e^(-1)_k (y1 ⊕ IV)
Decryption (general block): xi = e^(-1)_k (yi ⊕ yi−1), i ≥ 2
```
