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
<<<<<<< HEAD
    //my_calc.implies(my_calc.prop_begin(), my_calc.prop_back());
}
=======
    my_calc.implies(my_calc.prop_begin(), my_calc.prop_back());
}
print(a)
>>>>>>> 739fbb190b3ce84a373e3bd1c3e75d7fc3c434f4
TEST_MAIN()