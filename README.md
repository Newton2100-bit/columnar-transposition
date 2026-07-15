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
- it will handle 4096 * lenght of key input size
