# This will be  a simple implementation of:
    1. columnar transposition
    2. polyalphabetic substitiution

- I intend to have them as separate programs which cam feed each orther
  via a pipe or as coprocess.
- Them being different will make it easy for which to come first and
  also better maintenace.

- For the padding of columnar transposition i tend to go simple and use
  (`) since it is rare to find them in text and leave any complexities to the
algorithims.
- we have options  (-k) for keys which is mandatory
- and -f filename which is optinal.
(This implementaton is intended to be in c.)
