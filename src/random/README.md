# Random Number/Bit Generators 

RNBs are involved in some sketchy bizz, search about : [Bullrun](https://en.wikipedia.org/wiki/Bullrun_decryption_program) and [Reuters](https://www.reuters.com/article/us-usa-security-rsa-idUSBRE9BJ1C220131220)

It is important to note that Random Number Generators will be used here to generate the Initialization Vector that is used in most of the cipher operation modes.
This will be a summary of a lot of resources I used to develop and test my algorithms. Mainly:
> The NIST Special Publication (SP) 800-90 series of Recommendations provides
guidance on the construction and validation of Random Bit Generators (RBGs) in the form of
Deterministic Random Bit Generators (DRBGs) (also known as pseudorandom number
generators) or Non-deterministic Random Bit Generators (NRBGs) that can be used for
cryptographic applications.


Lets Start...

> As a note, keep in mind that people create some really complicated names for really simple things. Don't get spooked with the unknown terminology. 

> In my empirical observation, individuals have demonstrated a predilection for resorting to archaic neuro-cognitive constructs as a veiled stratagem to obfuscate their intellectual shortcomings. This phenomenon may be attributed to a psychological inclination toward concealing cognitive deficiencies through the adoption of antiquated notation systems, thereby camouflaging their true cognitive capacities or the real implications of their work in real life. Such behavior is indicative of a complex interplay between neuro-cognitive processes and socio-psychological factors, warranting further interdisciplinary investigation at the intersection of neuroscience, cognitive psychology, and sociolinguistics. TLDR: Monkey dumb&boring Monky utilizes the oxford dictionary a lot

## Components
* A source of random bits (an entropy
source) -- NIST SP 800-90B
* An algorithm for accumulating and providing random bits to the consuming applications -- NIST SP 800-90A
* A way to combine the first two components appropriately for cryptographic applications --NIST SP 800-90C

## Entropy Sources
These are elements or processes utilized within random number generation systems to introduce unpredictability and randomness.
Noise is turned into bits with some min-entropy.

## Types of entropy soruces
Where to get entropy(randomness)?
This is a very interesting question... As we will see, we can get entropy from:
* the varying phase of a signal;
* the metastability in certain circuits;
* enviromental noise samples;
* noise amplifiers;

### Phase Error Accumulation
In this types, the phase of the electrical signals has entropy. To extract it we accumulate this behaviour using the following methods.
#### Ring Oscilators
It is a series of inverters that are connected serially where the output is wrapbacked to the input. This forms a ring.  Then it sampled at a given time. Here we get entropy from the entropic phase noise (An electrical noise that causes the cycles of the oscilators to be longer or shorter on each cycle that we accumullate).
Clock trees can cause some deterministic Jitter and that causes the output to look more random than it really is.

The idea is that over different cycles the uncertainty of the state of the output gets larger. Because of this they are quite slow. To make them faster, ...

Here we have loop that have some noise and we are sampling it at a given regular interval.

#### Fast/Slow Ring Oscillators with noisy sampling
Here puts the noise in the sample time instead than in the loop. Here we will have a really fast oscilllator and a really slow oscilator that gots some variation on when it samples. When sampling somewhere in the phase, the slow oscilator will get something from the fast oscilator that is difficult to know.

### Metastable Collapse
Metastability occurs when a system is unable to settle into a definitive state within a specified timeframe, leading to uncertainty in the output. At a certain moment, a small difference will couse the output to set. This difference (or the event that couses this difference) caries a lot of entropy. 
This is like an amp op that is working without feedback. It may be working at a linear operation state, but then a small change on the input may cause the output to tend to "explode". Or just imagine a ball in the top of a steep two-sided hill.

#### Forced Latch
Imagine an inversor A and another B. The output of A is the input of B and the output of B is the input of A. When forcing the two sides of the circuit to be one for a brief amount of time, we create a condition of metastability. This condition hold wor an amount of time. This amount is probabilisticly determined by an exponentially decreasing function as time increases.

#### Metastable Ring Oscillator
Imagine a ring oscilator (a series of inversors that are chained) with a prime number of inversors. At a certain time, one inversor that generates a one will input in one that has generated a one in the previous cycle. At this point there is a discontinuity that will shift one inversor at a time

### Noise Sampling
In here we will just sample some enviromental noises from commonly available sources in order to get some signals that have a lot of randomness. 

#### Camera Sensor Noise
This is an array of pixel sensors. This have a signal + noise that will be passed into an amplifier which will add a little more noise into the circuit. As the camera signals can get an amount of noise, we can use this noise.
It is interesting to note that pixels have a high  correlation with nearby pixels. To get rid of this correlation, we need to sample pixels that are far from each other

#### Zener Diode Noise
Zener diodes can be employed as entropy sources by exploiting the inherent randomness in their breakdown voltage characteristics. When operated in reverse bias, zener diodes exhibit a breakdown phenomenon where they transition from a non-conducting state to a conducting state at a specific voltage, known as the zener voltage. The fluctuations in this breakdown voltage arise due to various factors including manufacturing imperfections, temperature variations, and quantum effects. By sampling the voltage fluctuations across a zener diode, one can capture unpredictable variations, thus generating random bits of entropy that can be utilized for cryptographic applications or random number generation.

#### Demodulation Error Vectors
Wireless communication devices have them. They take a signal and at specific time points it looks at a signal comming in and subtracts the noise from this data. The size of the error can be a source of noise.

### Amplifying Noise

#### Infinite Noise Multiplier (Modular Entropy Multiplier)
It's mathematical structure is equivalent to modular multiplication. 
It bases its operation on Succesive Approximation Analog to Digital
It employs an infinite noise source and analog-to-digital conversion to produce a stream of digital values. These values undergo modular multiplication, often implemented through successive approximation techniques, to ensure cryptographic robustness. The resulting output, subjected to entropy extraction, yields uniformly distributed and statistically independent random numbers suitable for cryptographic applications.
### Racing Noise Circuits
Basically works as a race of delays of the same pulse generation. The pulse that reaches first is the one that defines the state. The randomness here is in the rise and fall and propagation of phase shifts from one to the other contestant on the race.

## NIST SP 800-90B
This Recommendation specifies the design principles and requirements for the entropy sources used by Random Bit Generators, and the tests for the validation of entropy sources. These entropy sources are intended to be combined with Deterministic Random Bit Generator mechanisms that are specified in SP 800-90A to construct Random Bit Generators, as specified in SP 800-90C.

### Properties For Use in Cryptographic RBG

# Entropy Extractors
