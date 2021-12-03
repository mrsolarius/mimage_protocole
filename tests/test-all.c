#include "test-core.h"
#include "all-test.h"
#include "interface.h"

int main(int argc, char *argv[])
{
    //tests du core de tests
    testCore();
    //getType
    testType();
    //test SPP
    testSPP();
    //affichage du résumé des tests
    resume();
    return 0;
}
