#include <stdio.h>
#include <stdlib.h>

int main(){


    puts("Wow, you can compile C on the command line!\nUnless you're just reading this in a text editor...");


    puts("what's your favorite number?");
    int i = 0;

    scanf("%d", &i );

    if( i % 2 == 0 ) puts("an even number? what a scrub!");
    else puts("an odd number? what a scrub!");

    getchar();
    return 0;
}