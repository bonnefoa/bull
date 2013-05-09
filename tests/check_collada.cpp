#include <stdlib.h>
#include <bl_collada.h>
#include <check.h>

void setup (void)
{
}

void teardown (void)
{
}

START_TEST (test_collada)
{
}
END_TEST

Suite *collata_suite (void)
{
  Suite *s = suite_create ("Colladat suite");
  TCase *tc_core = tcase_create ("Colladat case");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_collada);
  suite_add_tcase (s, tc_core);
  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = collata_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

