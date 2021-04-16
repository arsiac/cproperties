#include "../include/CProperties.h"
int main()
{
    using namespace ac;
    ac::CProperties p;

    printf("p:ArraySize -> %d\np:PropertiesMaxSize -> %d\n", p.ARRAY_SIZE, p.PROPERTY_MAX_SIZE);

    p.set("a", "arsiac");
    p.set("b", "blabla");

    printf("a: %s\nb: %s\nc: %s", p.get("a"), p.get("b"), p.get("c"));

    return 0;
}