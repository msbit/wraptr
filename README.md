## wraptr

Really simple "smart pointer" implementation to wrap raw pointers.

Basically just a chance for me to look into some C++ operator overloads and templating.

### Example

Given a `child` class defined in `child.h` like so:

```c++
#include <cstdio>

struct child {
  child() { printf("child: creating (%p)\n", this); }

  ~child() { printf("child: destroying (%p)\n", this); }

  void do_stuff() { printf("child: doing stuff (%p)\n", this); }
};
```

we can wrap the raw `child` pointer in a `wraptr` like so:

```c++
#include <cstdio>

#include "child.h"
#include "wraptr.h"

wraptr<child> get(wraptr<child> a) { return a; }

int main(int argc, char **argv) {
  printf(" main: creating and wrapping (automatic)\n");
  wraptr<child> w1;
  w1->do_stuff();

  printf(" main: creating and wrapping (explicit)\n");
  wraptr<child> w2(new child());
  w2->do_stuff();

  printf(" main: copy construct\n");
  wraptr<child> w3 = w1;
  w3->do_stuff();

  printf(" main: move construct\n");
  wraptr<child> w4 = get(w1);
  w4->do_stuff();

  printf(" main: copy assignment\n");
  wraptr<child> w5;
  w5 = w1;
  w5->do_stuff();

  printf(" main: move assignment\n");
  wraptr<child> w6;
  w6 = get(w1);
  w6->do_stuff();

  {
    printf(" main: copy construct (separate scope)\n");
    wraptr<child> w7 = w1;
    w7->do_stuff();

    printf(" main: move construct (separate scope)\n");
    wraptr<child> w8 = get(w1);
    w8->do_stuff();

    printf(" main: copy assignment (separate scope)\n");
    wraptr<child> w9;
    w9 = w1;
    w9->do_stuff();

    printf(" main: move assignment (separate scope)\n");
    wraptr<child> w10;
    w10 = get(w1);
    w10->do_stuff();
  }

  printf(" main: about to return\n");
  return 0;
}
```

This outputs something like:

```
 main: creating and wrapping (automatic)
child: creating (0x55f3ca5a1e80)
child: doing stuff (0x55f3ca5a1e80)
 main: creating and wrapping (explicit)
child: creating (0x55f3ca5a1ec0)
child: doing stuff (0x55f3ca5a1ec0)
 main: copy construct
child: doing stuff (0x55f3ca5a1e80)
 main: move construct
child: doing stuff (0x55f3ca5a1e80)
 main: copy assignment
child: creating (0x55f3ca5a1f00)
child: destroying (0x55f3ca5a1f00)
child: doing stuff (0x55f3ca5a1e80)
 main: move assignment
child: creating (0x55f3ca5a1f20)
child: destroying (0x55f3ca5a1f20)
child: doing stuff (0x55f3ca5a1e80)
 main: copy construct (separate scope)
child: doing stuff (0x55f3ca5a1e80)
 main: move construct (separate scope)
child: doing stuff (0x55f3ca5a1e80)
 main: copy assignment (separate scope)
child: creating (0x55f3ca5a1f00)
child: destroying (0x55f3ca5a1f00)
child: doing stuff (0x55f3ca5a1e80)
 main: move assignment (separate scope)
child: creating (0x55f3ca5a1f20)
child: destroying (0x55f3ca5a1f20)
child: doing stuff (0x55f3ca5a1e80)
 main: about to return
child: destroying (0x55f3ca5a1ec0)
child: destroying (0x55f3ca5a1e80)
```

showing that the `child` instances are destroyed when the wrapping `wraptr` leaves scope, and that each of the ["Rule of Five"](https://cpppatterns.com/patterns/rule-of-five.html) methods are implemented:

* destructor
* copy constructor
* move constructor
* copy assignment operator
* move assignment operator
