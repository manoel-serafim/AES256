# Optimized AES-256 implementation
This is a full cryptographic library.

This cryptographic library is developed to function without reliance on any external resources, utilizing my own code for all mathematical operations, random number generation, cipher operation modes, cipher algorithms, etc. It will be easy to integrate this into BareMetal systems without hardware acceleration or dynamic allocation. The implementation minimizes function calls, optimizes looping assembly overheads, and will choose the best approach between between lookup tables and mathematical inline functions. A lot of testing has to go into into the cipher operation modes and the random number generators.


