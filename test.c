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

    printfProperties(properties);
    const char *age = cPropertiesGet(properties, "age");
    puts(age);
    const char *weight = cPropertiesGet(properties, "weight");
    puts(weight);
    return 0;
}