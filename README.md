# breakable_scope
C++ scope that can be broken using `break` which triggers an optional `else` scope

## What does it do?

The `breakable_scope` macro is a prefix which turns a regular scope `{ ... }` into a *breakable scope*:

    int foo() {
        int i = 0;
        breakable_scope {
        }
        return i; // returns 1;
    }

A breakable scope behaves just like a regular scope but allows the use of the `break` and `continue` keywords, as well as an optional `else` clause at the end. We'll cover every feature in turn:

### break

The original goal of the `breakable_scope` was, in fact, to break out of a pure C++ scope using `break`:

    int foo() {
        int i = 0;
        breakable_scope {
            i = 1;
            break;
            i = 2;
        }
        return i; // returns 1
    }

... which works pretty much like you'd expect.

### else

One nice side-effect of the macro is that it allows an optional `else` clause at the end of the breakable scope, that is executed if and only if the scope was exited via `break`:

    int foo() {
        int i = 0;
        breakable_scope {
            i = 1;
            // no break;
            i = 2;
        } else {
            i = 3;
        }
        return i; // returns 2
    }
    
    int bar() {
        int i = 0;
        breakable_scope {
            i = 1;
            break;
            i = 2;
        } else {
            i = 3;
        }
        return i; // returns 3
    }
    
This way, you can (for example) put error handling code at the end of the scope:

    void error_prone() {
        breakable_scope {
            if(error_condition_1) break;
            ...
            if(error_condition_2) break;
            ...
            // success
        } else {
            // generic error handling here
        }
    }

Nice.

### continue

One last feature of the `breakable_scope` is the use of the `continue` keyword, which enables you to break the scope **without** hitting the `else` clause, if one exists:

    int foo() {
        int i = 0;
        breakable_scope {
            i = 1;
            continue;
            i = 2;
        } else {
            i = 3;
        }
        return i; // returns 1
    }

Note that a `continue` within the `else` clause is syntactically correct but has no effect:

    int foo() {
        int i = 0;
        breakable_scope {
            i = 1;
            breakable_scope {
                i = 2;
                break;
                i = 3;
            } else {
                continue; // no effect
            }
            i = 4;
        } else {
            i = 5;
        }
        return i; // returns 4
    }

## How does it work?

There's no magic involved here, just plain old C++. Let's look at a trivial implementation of a prototypical `breakable_scope` (which was coincidentally, how it all started):

    #define breakable_scope for(bool cond = true; cond; cond = false)
    
    breakable_scope {
        break; // works :)
    }

A simple for-loop that is guaranteed to run only once and can be broken using `break`. We could stop right here...
Or we could totally over-engineer it, to see if it is possible to mimic python's behaviour of a [`for-else` loop](http://book.pythontips.com/en/latest/for_-_else.html) (spoilers: yes we can -- also FYI: Python's `for` loop has an optional `else` clause, who knew?)

The trick is to append an `if` statement to the `for` loop and ensure, that the loop runs again (to cover the `else` clause) should the `if` scope be broken:

    // DOESN'T WORK YET
    #define breakable_scope for(bool cond = true; cond; cond = false) if(cond)

This way, the user can attach an `else` clause after the `breakable_scope` and it will be executed on the second run ... but only if there is **no** `break` statement in the condition scope. Which is kind of the opposite of what we want :(

Fortunately, we can make things even more complicated. By adding yet another `for` loop in the mix and using an integer instead of a boolean, we get a working `breakable_scope` to impress our friends with:

    // this is already almost the final breakable_scope macro
    #define breakable_scope for (int i = 0; i < 2; ++i) for (; i < 2; i += 2) if (i == 0)
    //                       ^                           ^                                ^
    //                   first loop                 second loop                    condition scope
    
Now, if the condition scope runs through unbroken, `i` is incremented by two which breaks out of both loops at the same time and no `else` clause is evaluated. If on the other hand, the condition scope contains the `break` keyword, the second `for` loop is broken, `i` is incremented by one and we are back at the condition `i == 0` which fails the second time around, executing the optional `else` clause if existent.

Bam! 100 points for Gryffindor.

The use of the `continue` keyword was a happy accident, I admit. If put into the condition scope, the second loop increments `i` by two immediately, which has the same effect as running through to the end without breaking; thereby skipping the `else` clause altogether.

And all without leaking our condition variable into the surrounding scope - and you can even nest them!
The only potential hazard is the hardcoded name of our condition variable, which might override an important variable from the outer scope. This is why it is **not** called `i` in the actual macro but `__breakable_scope_condition_variable`, which is not something I expect to encounter out in the wild. Also I made it `auto` because why not.

And that's that! 

Is it cool? Yes! 
Does it make code hard to read/reason about/I hate macros? ...Maybe? You might stumble a bit the first time you see it, but I would argue that the `breakable_scope` may improve readability in situations that would otherwise require cod duplication or visible success-tracking variables at least.
I see it as the fish-knife at the dinner table of control flow: You can certainly do without, but it's good to have it, when you do need it.
