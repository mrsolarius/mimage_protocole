#include "test-core.h"
#include "all-test.h"
#include "Interface.h"

int main(int argc, char *argv[])
{
    //tests du core de tests
    testCore();
    //affichage du résumé des tests
    resume();
    //getType
    testType();
    //test SPP
    testSPP();
    return 0;
}
