#include "all-test.h"
#include "test-core.h"

void testCore(){
    printTitle("test-core");
    passTest("passTest", "should say success", true);
    //passTest("passTest", "should say error", false);
}