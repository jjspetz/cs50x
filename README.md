# cs50x
This is a repository for keeping my HarvardX: CS50 Introduction to Computer Science files as i work through them.
I sometimes find I like to go back and change code after I have submitted it which I can do here. My programing experience is mainly in JavaScript and Python, so I have little experience with lower-level languages such as C.

### WARNING...
If you are taking the class it is against academic rules to copy (or even read other peoples code from what I gather)
and pass it off as your own, so don't do it. However I will add tips and thought I had while working through the psets
in this read me. I have included some code snippets but nothing specific. If you are not taking the class and would
like to see my solutions feel free to check them out. All the code uses a cs50 library designed to make C simpler for
beginners but might cause some confusion for people more familiar with C.

### pset0 (Language: Scratch)
See my code here: [Catch the Butterfly](https://scratch.mit.edu/projects/149761466/)

### pset1 (Language: C)
This was my first time writing anything in C. I have learned C++ before but had not used it in years so my original solutions
could be significantly improved on.
#### Tip 1: Remember that C has do-while loops.
If you don't know how to use them google it. Using them would have made my pset1 codes a lot cleaner than they were.
#### Tip 2: Learn how returns from functions work in C
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

### pset2 (Language: C)
#### Tip 1: Pay attention to the lectures
This pset seemed easier than the pset one as long as you stick to the easier
problems. All the code you need to build the two ciphers is presented in the lectures
or in the problems themselves.
#### Tip 2: Focus on C data types
My biggest problem this week was not understanding how to change a variable from
form to another in C. For example changing a sting to an int or understanding the
difference between a char* and a char. This lead to me getting segmentation fault
run time errors I did not expect from my experience with other languages.
