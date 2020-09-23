#include <stdio.h>
#include "../cproperties.h"

int main()
{
    cProperties *properties = createPropertiesFromFile("test/data.properties");
    printStructure(properties);
    return 0;
}