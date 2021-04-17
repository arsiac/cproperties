#include "../include/CProperties.h"
int main(int argc, char *argv[])
{
    using namespace ac;
    
    if (argc == 1)
    {
        puts("testcpp <file name>");
        return 1;
    }

    CProperties p;
    p.load(argv[1]);

    printf("user.name: %s\n", p.get("user.name"));
    puts("set user.name = LiSi");

    p.set("user.name", "LiSi");

    puts("add user.phone=123");
    p.set("user.phone", "123");
    p.save(argv[1]);
    return 0;
}