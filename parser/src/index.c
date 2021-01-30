/* 
Tibor Ukropina
Student ID: 0961384

Citations:
Starter Code (Provided by prof)
http://knol2share.blogspot.com/2009/05/validate-xml-against-xsd-in-c.html
http://www.xmlsoft.org/examples/tree2.c
*/

#include <stdio.h>
#include "SVGParser.h"

int main(){

    char *string;

    string = getImgJson("./bin/Emoji_poo.svg");

    printf("%s\n", string);

    free(string);

}