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

Child::Child() { printf("child: created (%p)\n", this); }

Child::~Child() { printf("child: destroying (%p)\n", this); }

void Child::DoStuff() { printf("child: doing stuff (%p)\n", this); }
```

we can wrap the raw `Child` pointer in a `wraptr` like so:

```c++
#include <stdio.h>

#include "child.h"
#include "wraptr.h"

int main(int argc, char **argv) {
  printf(" main: creating and wrapping Child automatically\n");
  wraptr<Child> w1;

  printf(" main: creating and wrapping Child explicitly\n");
  wraptr<Child> w2(new Child());

  w1->DoStuff();
  w2->DoStuff();

  {
    printf(" main: copying Child in separate scope\n");
    wraptr<Child> w3 = w1;
    w3->DoStuff();
  }

  printf(" main: about to return\n");
  return 0;
}
```

This outputs something like:

```
 main: creating and wrapping Child automatically
child: created (0x560688621280)
 main: creating and wrapping Child explicitly
child: created (0x5606886212a0)
child: doing stuff (0x560688621280)
child: doing stuff (0x5606886212a0)
 main: copying Child in separate scope
child: doing stuff (0x560688621280)
child: destroying (0x560688621280)
 main: about to return
child: destroying (0x5606886212a0)
child: destroying (0x560688621280)
```

showing that the `Child` instances are destroyed when the wrapping `wraptr` leaves scope.
