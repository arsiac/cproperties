#include <stdio.h>
#include "cproperties.h"

int main()
{
    cProperties *properties = newProperties();

    // cp size
    printf("cp->size: %d\n", properties->size);

    cPropertiesSet(properties, "name", "ZhangSan");
    cPropertiesSet(properties, "age", "22");
    cPropertiesSet(properties, "sex", "man");
    cPropertiesSet(properties, "phone", "1234567890");

    printfProperties(properties);
    puts("delete age");
    cPropertiesDelete(properties, "age");
    printfProperties(properties);
    return 0;
}