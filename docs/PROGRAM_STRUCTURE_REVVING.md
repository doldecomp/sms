# Reverse-engineering Overall Program Structure

A compiled program consist out of symbols that represent functions and global variables spread across `.o` files.
A source code consists of classes, functions and global variables, whose declarations/definitions are spread across `.h(pp)`/`.c(pp)` files.
This document describes how to recover the high-level C or C++ file structure from a compiled program that has a linker map file.
Note that some if it is only applicable to MWCC, specifically the way symbols are mangled.

### Step 1: Definitions and Source Files

Firstestly, `.o` files are in a 1-to-1 correspondence with `.cpp` files, and the names of these `.cpp` files are explicitly available in the `.MAP` file, so it is quite obvious where the **definitions** of functions and variables should live.
What is not available from the `.MAP` file are the paths to the `.cpp` files.
Those must either be made up, or recovered from various assertions that accidentally include the full path to the file they are located in.
Sadly, SMS's version of MWCC does not do that, so the paths we have for the SDK, MSL and JSystem have been recovered from sources other than SMS.
One thing that is always worth doing is sorting `.cpp` files into folders based on the `.a` or `.rel` library they were compiled into, which is always available in the `.MAP` file.
For SMS's game code, any further structuring into folders will have to be made up.

Secondly, symbols have multiple properties. These are it's name, being function/object, being weak/strong, being local/global, and what section the symbol is in.
These properties are common for both C and C++, and represent the following high-level constructs.

 1. A name uniquely identifies the symbol.
 In C++, the names are *mangled* and contain additional information which is described latter.
 2. A function symbol corresponds to a function.
 An object symbol corresponds to a variable.
 3. A **local** symbol is one that is "local" to the TU, and so inaccessible from other TUs.
 This allows one to re-use it's name for different stuff in different TUs without risking a linking error.
 Free functions and global variables corresponding to local symbols must be marked as `static` at their definition point.
 Note that `static` in declarations of class members means a completely different thing unrelated to weak symbols.
 In C++, a local symbol's mangling might start with `@unnamed@`, which means that rather than being `static`, it is actually contained inside of an anonymous namespace, which automatically makes it `static`.
 4. A **weak** symbol is one that can "safely" have duplicates with the same name across multiple TUs.
 The linker is allowed to *assume* that all the duplicates are exactly the same and throw out all but one version of the symbol, even if this is not actually true.
 MWCC seems to not really do this very actively for function symbols though.
 What being weak corresponds to is being marked as `inline` and usually being defined inside of the header rather than the cpp file for free functions.
 Then when the header is included into multiple TUs we avoid violating the One Definition Rule because it does not apply to inline/weak things.
 For C++ member functions (aka methods), this corresponds to simply being defined not-out-of-line, i.e. the definition goes right inside of the class, where the declaration is. Note that in some extreme cases you might need to force the compiler's hand to make something local via the `__declspec(weak)` extension, but this should generally be avoided, because presumably no sane dev would have written that.
 Not-out-of-line defined methods are automatically marked as inline, and you can but really shouldn't additionally mark them as inline.
 Note that weak object symbols that correspond to inline variables are not supported by C++98.
 5. The section the symbol is in gives additional info about what it was generated from.
 Functions are always located in the `.text` section, except when explicitly overriden via the `__declspec(section ".lulz")` compiler extension.
 For variables, there are several different sections that imply different things about the variable:
    - `.bss` section represents variables which are "uninitialized", or rather, initialized to all zeros;
    - `.sbss` is same as `.bss`, but for small variables (how small is configured via compiler flags), which allows for them to be referred to more efficiently from asm;
    - `.data` section represents variables which contain actual data, i.e. are initialized at definition point with something non-zero;
    - `.sdata` is for small `.data`, same as `.sbss` is for `.bss`;
    - `.sdata2` is same as `.sdata` but read-only, i.e. ;
    - `.rodata` section represents const variables which contain data (usage of it is controlled by compiler flags);
    - `.comm` section represents weak-behaving variables which are usually typed with C++ classes that have a constructor;
    - TODO: this is incomplete.

Thirdly, when compiling C++, symbol names are *mangled*, which basically means including additional information into the symbol's name itself.
For MWCC this means including the signature of a function except for return type and it's namespace.
This is done because C++ supports function overloading and name resolution: one is allowed to define both `foo(int)`, `foo(float)` and `ns::foo(int)` and disambiguate between them on call site.
Giving all of them the same name `foo` like C does is impossible, because they are different entities and must have different symbols, and symbols are identified by name.
What this means for us is that you get a ton of information *for free* if you de-mangle the symbol.
De-mangling can be done via objdiff, dtk, or the bulk_demangle script in the root of this repo, but for some special symbols it does not work.
The following information can be extracted, be it via automatic demangling or manual analysis:

1. The function signature and namespace path.
 Plain and simple, just demangle and you'll have it.
 One tricky nuance is that you cannot generally tell apart classes and namespaces just from mangling.
 Moreover, a class with only static methods is essentially equivalent to a namespace, so guessing is the best we can do.
 Finally, you cannot tell apart regular methods from static methods, for that you need to decomp the actual function and see whether it uses `r3` as `this`.
2. Anonymous namespace are marked as `@unnamed@`.
3. Unnamed constants are referred to as `@42069`. These refer to various constants used in code: float literals (`1.0f`), string literals (`"lol"`), struct literals (`(Thing) { 1, 2, 3 }`), array literals (`int data[] = { 1, 2, 3 }`).
4. Static local variables (`static int a = 4;` inside of a function) are mangled to as `someName$42`, and if the initial value for the variable is non-zero, the flag marking them as initialized is called `init$69`.
5. Virtual tables are mangled as `__vt__*`.
6. When doing multiple or virtual inheritance, method stubs for sibling vtables are prefixed with `@42@` and the main method's symbol name follows.
7. A function that initializes global C++ variables that have constructors is called `__sinit_FileName_cpp`.
8. TODO: probably more nuances

Finally, when you decide to reverse a particular TU, say `lol.cpp`, what you have to do to make everyone's lives easier in the future is the following:
1. Filter out all lines containing the line `lol.cpp` our of the `.MAP` file.
2. Get rid of the "top" part of the where lines are hierarchical and start with `42]`.
3. For each symbol you see, create the respective function/variable in the source file and do it while respecting all of the above (`static`, leaving out weak symbols, etc), **even for UNUSED symbols**.
For return types, initially simply go for `void`.
Figuring out the real return types requires decomping the actual code.
4. Add appropriate header(s) according to what is written below.

Including even `UNUSED` symbols is the important part.
These are more often than not **required** to get the inlining right, and while we don't have actual asm for these symbols, we can guess their contents based on name, signature and **symbol size**.

### Step 2: Declarations and Headers

Translation units (TU) communicate through headers.
Headers are copy-pasted as plain text into source files via the `#include` directive.
The `.MAP` file contains no information about headers, so we have to make an educated guess as to where stuff was originally located.

For a class `TBlurbSlurginator`, do the following:
 1. If the `mario.MAP` file contains any non-weak symbols for methods of `TBlurbSlurginator`:
    - if they are all in file `src/B/A.cpp`, put the class in `include/B/A.hpp`;
    - if they are spread across different files AND all these files start from a common prefix, like `Blurb*.cpp`, put the class in a header named like the common prefix, as in `Blurb.hpp`;
    - if they are spread across randomly named files, put it into a new header named after the class, as in `BlurSlurginator.hpp`.
 2. If `mario.MAP` contains no symbols tying `TBlurbSlurginator` to a particular TU, again, put it in a header named after the class, `BlurbSlurginator.hpp`.

For a free function:
1. If the function's symbol is strong and is in TU `src/B/A.cpp`:
    - If the symbol is global, put the declaration into `include/B/A.hpp`.
    - If the symbol is local, put the declaration at the top of `src/B/A.cpp`, or don't create one at all, definition might be enough depending on function order.
2. If the function's symbol is weak, you have to guess what header it goes into based on common sense and naming.
3. There sometimes are conventions in different files like starting non-public functions with an underscore (`_`) and marking public ones with `SMS_`.
In this case, it makes sense to either not declare underscored functions anywhere and treat them like static ones, or put their declarations in an additional "private" header that is only accessible by the current "library" and located next to the `.cpp` files.

This basically is the "common sense" approach to picking a header file.
What goes in a header is class, variable and function declarations.
Note that variable declarations look like this: `extern Type name;`.

Next, creating a declaration for a class should be done as follows to avoid missing out on additional information/context available in the linker map.
1. Filter out lines with the `__[0-9]*ClassName` regex (or a more involved one if the class is in a namespsce).
2. Put declarations for all symbols found into the class.
3. Look through the hierarchical part of the filtered out lines.
If a function is marked as `weak` there, upgrade the declaration to a definition.
4. If the class has a vtable symbol:
    - Open up Ghidra or Ida, find the symbol and mark all functions listed in the vtable as `virtual`.
    - If any functions from a different class are mentioned in the vtable, you can be certain that your class inherits that class somehow.
    - Look at the classes ctor/dtor in Ghidra to figure out it's parents for certain.
        * If the ctor/dtor are not present in Ghidra, it was inlined and omitted.
        Look for references to the classes vtable, those will be the call points at which the ctor/dtor were inlined.
        * If a different classes ctor is called first thing in the ctor on `this`, that's a parent.
        * With multiple inheritance, you will get multiple calls with different offsets from `this`.
        * If these calls to parent ctors/dtors were inlined, you will see the vptr being written to multiple times, first with the parent's vtable, then with the child's.
        MWCC is not smart enough to optimize these superfluous writes out, which is perfect for us.
        The order of these writes is the order of inheritance.
        * When multiple inheritance is involved and ctors/dtors are inlined, you will see multiple different vptrs being written to (each parent with virtual functions gets it's own vptr which needs to be replaced with one of the child's vtables).
        You will also see method stubs, wrappers around existing methods that offset the `this` pointers and call into the real method.
    - Note that MWCC's vtables always start from two 4-byte words filled with zeros most of the time. Those are for virtual inheritance and RTTI, which SMS barely uses.
    What follows is the virtual function pointers, and sometimes those can be 0.
    This might mean that there is a pure virtual function in the class, but zeros at the end of the VT might just be padding.
    Check the vtable symbol's size to be certain.
    To figure out the pure virtual function's signature, look at derived classes where the function is actually implemented and overriden.
    - Overall, figuring out OOP hierarchies simply requires good understanding of what exactly different C++ constructs like inheritance, virtual methods, etc get compiled into, or rather, what plain C code they correspond to.
    Reading up on this on the internet is a great help.
    - Ideally, you should validate that your vtable matches the binary.
    To do this, find the source file in which the vtable is located, add the definition for at least one method of the class, and the vtable should start getting emitted into that TU.
    Now open objdiff and make sure it shows the vtable as 100% matching and delete all of your temporary changes to the source file.
    - TODO: virtual inheritance

Note that again, filtering out the `.MAP` for ALL method symbols from the get-go prevents missing out on crucial information about methods which were inlined/unused, as even their names might be useful for figuring out how a class works and what it does.
Adding stuff to a class "as you need it" risks missing out on these details, so creating a fully blown header with a complete class as soon as you need it is what you should be doing.

Following these "simple" steps, you should be able to recover most high-level structure of your program.
