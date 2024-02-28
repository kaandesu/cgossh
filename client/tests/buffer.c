#include "../ring_buffer.h"
#include <criterion/criterion.h>

ring *r = NULL;

void suitesetup(void) { r = init_ring(3, 3); }

void suiteteardown(void) { ring_destroy(r); }

TestSuite(buffer, .init = suitesetup, .fini = suiteteardown);

Test(buffer, create) {
  cr_expect(r != NULL, "init_ring should return not NULL");
}

Test(buffer, adding_elements) {
  cr_expect(ring_empty(r), "ring should be empty");
  cr_expect(add_ring(r, 1), "ring should accept elements");
  cr_expect_not(ring_empty(r), "ring should not be empty");
}

Test(buffer, adding_elements_until_full) {
  cr_expect(add_ring(r, 1), "expected adding an element to succeed");
  cr_expect(add_ring(r, 1), "expected adding an element to succeed");
  cr_expect(add_ring(r, 1), "expected adding an element to succeed");

  cr_expect(ring_full(r), "expected ring to be full");
  cr_expect(add_ring(r, 1) == false, "expected adding an element to fail");
  cr_expect(ring_full(r), "expected ring to be full");
}
