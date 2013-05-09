#include <stdlib.h>
#include <bl_collada.h>
#include <check.h>

START_TEST (test_collada)
{
        const char* filename = "tests/simple.dae";
        ColladaWriter wr;
        wr.readFile(filename);

}
END_TEST

Suite *collada_suite (void)
{
  Suite *s = suite_create ("Collada suite");
  TCase *tc_core = tcase_create ("Collada case");
  tcase_add_test (tc_core, test_collada);
  suite_add_tcase (s, tc_core);
  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = collada_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

