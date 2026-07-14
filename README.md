# This will be  a simple implementation of:
    1. columnar transposition
    2. polyalphabetic substitiution

- I intend to have them as separate programs which can feed each other
  via a pipe or as coprocess.

- Them being different will make it easy for which to come first and
  also better maintenance.
# implementation

- For the padding of columnar transposition i tend to go simple and use
  (+) since it is rare to find them in text and leave any complexities to the
algorithims.
- after profiling you will note it spends most of the time (95%) in
  kernel space hence maitaining the rule of not tampering with user
space all over.

# usage
- -k key (for they key and it is mandatory)
- -o outputfile ( default is stdout)
- -f file(file name holding the plain text default is stdin)

# LIMITATION
- can only stomach 4096 bytes for 64 bit machine 
haven't tried on a 32 bit machine
- When you exceed this limit the program with smash the stack and crash
  or rather abort execution
## WHY 4096
- Since it is the page size of 64 bit machines and mmap works with
  factors of pagesize (_SC_PAGESIZE)

(will be handled on version 1.2 release)
(newton irungu)
