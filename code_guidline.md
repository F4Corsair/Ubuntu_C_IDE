# Naming Convention
- macro : ALL_CAPS_WITH_UNDERBAR
- struct : UpperCase
- global / local variable : snake_case\
(Local Variable Must declared in global.h)
- function(method) : cammelCase
- filename : cammelCase.ext
## Recommendation
when you define header, try to use macro block like

```c
#ifndef __HeaderNameOrSomething
#define __HeaderNameOrSomething
/* BIG CATEGORY */
// small category
extern int global_variable; // original global variable MUST declare at global.h - other uses extern
void funcDef(int);
#endif
```
# Makefile(for demo only)
follwing is sample Makefile code (don't revise src/Makefile)
```makefile
CFLAGS = -g -lcurses

code.out : code.c
	cc -o $@ $< $(CFLAGS)
```
if you try to compile with header files : for example, main.c sub.c sub.h
```makefile
CFLAGS = -g -lcurses

main.out : main.c sub.c
	cc -o $@ $< $(CFLAGS)
```
and call
```
$ make
```
will automatically builds file under its dependency