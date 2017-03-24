## pset3 (Language: C)
### Tip 1: Check out find (more comfortable)
Even if you are not willing to implement this version of find it introduces
another sort algorithm, the counting sort, which is more efficient in some cases
than anything in the lectures so you should check it out.

### Tip 2: fifteen.c check50
The check forces you to change your free space to a '0'. This can cause your former
working code to no longer work. When trying to implement the code for check50 understand
that your array called board is most likely larger than the part you display to the
screen and these unseen parts are most likely initialized with the value 0. For example
you see this in a 3x3 array:

 8 7 6

 5 4 3

 2 1 0

but actually the array can be thought of as looking like this:

  0 0 0 0 0

  0 8 7 6 0

  0 5 4 3 0

  0 2 1 0 0

  0 0 0 0 0

so you must make sure your code deals with these outside zeros.
