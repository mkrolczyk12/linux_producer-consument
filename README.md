
## Table of contents

- [Description](#description)
- [Status](#status)
- [Contact](#contact)


## Description

### Producer and consumer problem with the use of linux semaphores and shared memory

The project consists of two programs that run independently: producer (producent.c) and consumer (konsument.c).
The producer produces the goods (e.g. numbers, characters) and places them in a common buffer
(shared memory) that can only hold one unit of good at a time. Consumer downloads
the commodity (without destroying the buffer) and consumes it.

For harmony to reign, following conditions must be met:

- each unit of goods produced must be consumed,

- no unit of a good can be consumed twice (even if the consumer is faster
than the manufacturer).

Implement the above task using the semaphore mechanism. To demonstrate that
there was no loss or multiplication of the goods, let the producer collect the "raw material" (numbers, characters) for
producing goods from a text file, and the consumer places the downloaded goods in another file
text. After the programs are finished (the "raw" resources are exhausted), both files
text files should be identical. Use e.g.
sleep() function with a random number of seconds. Implement a function call interrupt handler
system semop().

## Status

_completed_

## Contact

Created by @mkrolczyk12 - feel free to contact me!

- E-mail: m.krolczyk66@gmail.com
