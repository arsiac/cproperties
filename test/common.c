#include <stdio.h>
#include "../cproperties.h"


int main()
{
    cProperties *cp = newProperties();
    for (int i=0; i<cp->size; i++)
    {
        printf("array[%d]: ", i);
        if (cp->array[i])
        {
            puts("not null");
        }
        else
        {
            puts("(null)");
        }
        
    }
}