#include <stdio.h>
#include "properties.h"

int main()
{
    cProperties *properties = createPropertiesFromFile("test/data.properties");
    printStructure(properties);
    cPropertiesWriteToFile(properties, "test/data-copy.properties");
    return 0;
}