#include <stdlib.h>
#include <check.h>
#include <bl_model.h>
#include <bl_loader.h>

START_TEST (test_model)
{
        BlModel *blModel = loadAssetFile("tests/simple.dae")[0];
        fail_unless(blModel->uvs == NULL);
}
END_TEST

Suite *model_suite (void)
{
  Suite *s = suite_create ("Model suite");
  TCase *tc_core = tcase_create ("Model case");
  tcase_add_test (tc_core, test_model);
  suite_add_tcase (s, tc_core);
  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = model_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
