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
- Cannot hande more than 4096 bytes from a file but if it is is stdin it
  will truncate

## WHY 4096
- since i explictly asked for that in the very begining from the memory
  manager.

(will be handled on version 1.2 release)
(newton irungu)
