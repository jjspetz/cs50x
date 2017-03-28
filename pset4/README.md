## pset4 (Language: C)
### Lessons Learned
I spent a lot of time trying to debug recover and the error turned out to be that
when I called the fwrite function I was writing over my input variable as opposed
to my output variable. My input variable was named 'in' which seems fine, and my
output variable was named 'img' as the problem set suggested. If i had names that
were less similar I probably would have caught the mistake earlier instead of
spending a lot of time trying to figure out a logic error that was not there.
Lesson learned, put more care into variable names.
