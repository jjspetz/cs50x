## pset1 (Language: C)
This was my first time writing anything in C. I have learned C++ before but had not used it in years so my original solutions
could be significantly improved on.
### Tip 1: Remember that C has do-while loops.
If you don't know how to use them google it. Using them would have made my pset1 codes a lot cleaner than they were. After implementing some of my later code with do-while loops it seems they tend to cause failures when doing check50, so you might want to not use them if you want everything to pass the check. I still think it improves the overall code, but I will be leaving them out of solutions where it is causing check50 failures.
### Tip 2: Learn how returns from functions work in C
The lectures show how to write functions and declare them.
For example:
```
void function_name(int x)
```
where `int x` is the variable passed into the function and `void` is the type of variable to be returned. so a function can be changed like this:
```
int function_name(int x)
{
  return 0;
}
```
Now the function will return the integer 0.
