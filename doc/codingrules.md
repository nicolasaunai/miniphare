
# Coding Rules

* [Naming conventions](#namiing-conventions)
    * [Golden naming rules] (#golden-rules)
    * [Function names] (#function-names)
    * [Variable names] (#variable-names)        
* [Code Formatting](#code-cormatting)
    * [Long Lines] (#long-lines)
    * [Spaces VS Tabs] (#spacetabs)    
	* [Blocks] (#blocks)    
	* [Function declarations and definitions] (#function-declarations-and-definitions)
	* [Conditionals] (#conditionals)    
	* [Loop and Switch statements] (#loopswitch)    
	* [Pointers] (#pointers)    
	* [Return values] (#return-values)    
	* [Preprocessor directives] (#RPreprocessor-directives) 
	* [Operators] (#operators) 	
	* [Blank lines] (#blank-lines) 	
	* [The \#define guard] (#define-guard) 	
	* [Function parameter ordering] (#Rfunction-parameter-ordering) 	
	* [Include ordering] (#include-ordering) 
	

## Naming conventions



### <a name="code-formatting"></a>  Golden naming rules


* Function names, variable names, and filenames should be descriptive.
* Give a name *as descriptive as possible*, within reason. Better clear and precise than cute.
* It is important to make your code immediately understandable by a new reader.
* Do not use abbreviations that are ambiguous or unfamiliar to readers outside your project, and do not abbreviate by deleting letters within a word.
* Ask yourself : "what other meanings could someone interpret from this name"



### <a name="function-names"></a>  Function names



####  Public Functions


####  Private functions

### <a name="variable-names"></a>   Variable names


* avoid generic names, e.g. 'tmp' or 'retval' unless for very short-lived and temporary variables
* name should help understanding the purpose or value of the variable
* name variable to avoid commenting



#### Be consistent with "units"

Don't write

```cpp
if (time == run->max_iter) // max_iter read as integer while time is float
```

Like in equations, instructions should be "homogeneous" to be more readable.



#### Respect "common sense"

Some names are usually used for specific things, and it might be difficult for the reader to use them for other means.
For instance get*() usually returns a member/attribute, hence is supposed to be fast. If used for calculating something possibly big, it can be misleading, one would in this case prefer "compute*()" or something telling the user this function is not simply an accessor.

```cpp
double getMean(); // NO
double computeMean(); // YES
```


*********
		
	
## <a name="code-formatting"></a> Code Formatting 


Those rules apply to all files, header, code, makefile etc.

### <a name="long-lines"></a> Long Lines


lines must be at most about 80 characters long.


### <a name="spacetabs"></a> Spaces VS Tabs


no , only spaces.\
1 = 4 spaces


### <a name="blocks"></a> Blocks


In general, blocks have curly braces opening and closing at the same
identation level. There are no blank lines after the opening { and
theclosing }.

If the block has many lines, typically more than a screen, write a
comment at the closing parenthese to indicate which block it is closing


```cpp
    block
    {
    // code goes here;
    } // end of block comment if the block is very long
```

### <a name="function-calls"></a> Function calls


On one line if it fits; if otherwise, wrap args at the parenthesis

```cpp
    retval = DoSomething(argument1, argument2, argument3);
```

If the arguments do not all fit on one line, they should be broken up
onto multiple lines, with each subsequent line aligned with the first
argument. Do not add spaces after the open paren or before the close
parenthesis.


```cpp
    retval = DoSomething(averyveryveryverylongargument1,
                         argument2, argument3);
```                         

If the function has many arguments, consider having one per line if this
makes the code more readable:


```cpp
    retval = DoSomething(argument1,
                         argument2,
                         argument3,
                         argument4);
```                         

### <a name="function-declarations-and-definitions"></a> Function declarations and definitions


Return type on the same line as function name, parameters on the same
line if they fit. In declarations, the parameters must be explicitely
named. In definitions, curly braces must appear at the same indentation
level as the return type.


```cpp
    RetType funcname(Type param1, Type Param2, Type Param3);

    RetType funcname(Type param1, Type Param2, Type Param3)
    {
    // code goes here;
    }
```

if the function prototype is too long to fit on one line:

```cpp
    RetType reallylongfuncname(Type longparam_name,
    Type another_long_paramname);
```
    

There is never a space between:

-   function name and parenthesis
-   parenthesis and the first parameter
-   the last parameter and the parenthesis

There is one and only one space:

-   between return type and function name
-   after each coma


### <a name="conditionals"></a> Conditionals


There is no space after ( and no space before ). Curly braces are at the
same indentation level (Block rule)\
Else is on a new line.\
There is a space between if/else and ().\
There are spaces between variables and operators.\
There are parenthesis around conditions.


```cpp
    if ((condition1) || (condition2))
    {
    // code goes here...
    }
    else if (condition3)
    {
    // another piece of code here
    }
    else
    {
    // and also here.
    }
```    

On rare occasions, when the line is very short and will very probably
not require debugging, one can use a one-line if:

```cpp
    if (condition) instruction;
```


Bad examples:


```cpp
    if(condition) instruction; // no space after if

    if (condition){ // curly braces should be at the same indent.
    instruction;
    }
```



### <a name="loopswitch"></a> Loop and Switch statements


The Block rule applies.
There is one space between the loop-type and the parenthesis.

```cpp
    switch (variable)
    {
        case 0:
            // code with 1 indent. level
        break; // break at the same level than the case.

        // one empty line
        case 1:
            // code goes here
        break;

        // empty line
        default:
            //default code.
    }



    for (iter=0; iter<3; iter++)
    {
        // code goes here
    }


    do
    {
        // code goes here;
    }
    while (condition);
```



### <a name="pointers"></a> Pointers


No spaces around period or arrows.

```cpp
    a = x.y;
    b = x->y;
```

No spaces between operators and variable:

```cpp
    a = *x;
    b = &x;
```

when declaring pointers, no space after the operator:

```cpp
    char *a; // OK
    char * a; // NO
    char* a; // NO
```

### <a name="return-values"></a> Return values


No parentheses around ‘return’ except in complex cases:

```cpp
    return result; // ok
    return (result); // no
    return (condition1 && condition2); // ok
```

### Preprocessor directives


The hash mark should always be at the begining of the line even\
if embedded in an indented block.


### <a name="operators"></a> Operators 


```cpp
    x = 0; // Assignment operators always have spaces around them.
    x = -5;
    ++x; // No spaces separating unary operators and their
    ++x; // arguments.
    if (x && !y)

    v = w * x + y / z; // Binary operators usually have spaces around them,
    v = w*x + y/z; // but it's okay to remove spaces around factors.
    v = w * (x + z); // Parentheses should have no spaces inside them.
```


### <a name="blank-lines"></a> Blank lines


There is no strict rule. Blank lines should be used to separate pieces
of code with different semantics.

```cpp
    // this code
    // calculates that stuff
    // which depends on that variable

    // (blank lines)

    // this other piece
    // of code
    // does something else
```

There should be a balance between:

-   maximize the code that fits in one screen
-   minimize the code density that makes it less readable.



### <a name="define-guard"></a> The \#define guard


All header files must have a \#define guard to prevent multiple
inclusions. The format of the symbol is FILENAME_H to
guarantee uniqueness.

```cpp 
    #ifndef HYBRIDSPECIES_H
    #define HYBRIDSPECIES_H

    // code goes here

    #endif // HYBRIDSPECIES_H
```

### <a name="function-parameter-ordering"></a> Function parameter ordering


Prefer input parameters before output ones.


### <a name="include-ordering"></a> Include ordering

Prefer this order:

-   C++ standard librarie
-   blank line
-   other libraries
-   blank line
-   your project


