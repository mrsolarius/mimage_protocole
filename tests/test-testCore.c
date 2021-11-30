#include "all-test.h"
#include "test-core.h"

//Test test_core
void testCore(){
    printTitle("tests-core");
    passTest("passTest", "should say success", true);
    //passTest("passTest", "should say error", false);
}