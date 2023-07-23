# mkgg

## Warning: Intentionally terrible coding practices ahead

**The Goal:** Golfy (aka minimized) C interpreter for a capable language. I don't have a target golf size, but I will probably try to golf it to the closest power of two for nerd credit.

`mkgg.c` is a semi-golfed interpreter. It has comments, although most global symbols are single letters. For golfiness. `mkgg.min.c` will contain the fully golfed interpreter.

`std.mkgg` contains a list of functions that are useful for general programming but not *strictly* necessary to include as builtin functions.


`test-cases.rb` is a Ruby script which has a bunch of test programs and expected outputs for those programs.

TODO:

 * Read input from STDIN/files through the course of a program
 * More type checking?
