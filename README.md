# This will be  a simple implementation of:
    1. columnar transposition
    2. polyalphabetic substitiution

- I intend to have them as separate programs which cam feed each orther
  via a pipe or as coprocess.
- Them being different will make it easy for which to come first and
  also better maintenace.

- For the padding of columnar transposition i tend to go simple and use
  nulls '\0' (well known in C) and leave any complexities to the
algorithims.
(This implementaton is intended to be in c.)
