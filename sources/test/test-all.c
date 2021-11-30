#include <test-core.h>
#include <all-test.h>
#include <type.h>

int main(int argc, char *argv[])
{
    //test du core de test
    testCore();
    //affichage du résumé des tests
    resume();
    //getType
    testType();
    return 0;
}
