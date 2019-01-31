#include "calculator.h"
#include "unit_test_framework.h"

using namespace std;
TEST (test_distribution) {
    Calculator my_calc;
    my_calc.take_input();
    my_calc.test_distribution();
}

TEST (test_implies) {
    Calculator my_calc;
    my_calc.take_input();
    cout << my_calc;
    //my_calc.implies(my_calc.prop_begin(), my_calc.prop_back());
}
TEST_MAIN()