# menu.h

## Get started

```bash
wget https://raw.githubusercontent.com/IncludeOnly/menu.h/refs/heads/main/menu.h
```

## Example

```c
#define MENU_IMPLEMENTATION
#include "menu.h"

int main(){
    int selected = menu("Program", 1, menu_arrow_print_option, "Item1", "Item2", "Item3", NULL);

    printf("selected: %d\n", selected);

    return 0;
}
```

## If you really need to link

```bash
mv menu.h menu.c
cc -o libmenu.so menu.c -fPIC -DMENU_IMPLEMENTATION -shared
mv menu.c menu.h
```

## License

[MIT](./LICENSE)
