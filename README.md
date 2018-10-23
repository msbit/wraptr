## wraptr

Really simple "smart pointer" implementation to wrap raw pointers.

Basically just a chance for me to look into some C++ operator overloads and templating.

### Example

Given a `Child` class defined in `Child.h` like so:

```c++
#include <stdio.h>

class Child {
public:
  Child();
  ~Child();
  void DoStuff();
};

Child::Child() { printf("child: creating (%p)\n", this); }

Child::~Child() { printf("child: destroying (%p)\n", this); }

void Child::DoStuff() { printf("child: doing stuff (%p)\n", this); }
```

we can wrap the raw `Child` pointer in a `wraptr` like so:

```c++
#include <stdio.h>

#include "child.h"
#include "wraptr.h"

wraptr<Child> get(wraptr<Child> a) { return a; }

int main(int argc, char **argv) {
  printf(" main: creating and wrapping (automatic)\n");
  wraptr<Child> w1;
  w1->DoStuff();

  printf(" main: creating and wrapping (explicit)\n");
  wraptr<Child> w2(new Child());
  w2->DoStuff();

  printf(" main: copy construct\n");
  wraptr<Child> w3 = w1;
  w3->DoStuff();

  printf(" main: move construct\n");
  wraptr<Child> w4 = get(w1);
  w4->DoStuff();

  printf(" main: copy assignment\n");
  wraptr<Child> w5;
  w5 = w1;
  w5->DoStuff();

  printf(" main: move assignment\n");
  wraptr<Child> w6;
  w6 = get(w1);
  w6->DoStuff();

  {
    printf(" main: copy construct (separate scope)\n");
    wraptr<Child> w7 = w1;
    w7->DoStuff();

    printf(" main: move construct (separate scope)\n");
    wraptr<Child> w8 = get(w1);
    w8->DoStuff();

    printf(" main: copy assignment (separate scope)\n");
    wraptr<Child> w9;
    w9 = w1;
    w9->DoStuff();

    printf(" main: move assignment (separate scope)\n");
    wraptr<Child> w10;
    w10 = get(w1);
    w10->DoStuff();
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

showing that the `Child` instances are destroyed when the wrapping `wraptr` leaves scope, and that each of the "Rule of Five" methods are implemented:

* destructor
* copy constructor
* move constructor
* copy assignment operator
* move assignment operator
