#include <criterion/criterion.h>

Test(simple, passing) {
    cr_assert_eq(1, 1, "1 should equal 1");
}
