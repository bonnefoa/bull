#include <stdlib.h>
#include <check.h>
#include <bl_model.h>
#include <bl_shape.h>
#include <bl_loader.h>
#include <common_tests.h>

START_TEST (test_shape)
{
        btVector3 position = btVector3(-1.0f, -1.0f, -1.0f);
        BlModel *model = loadModelFile("tests/cube.dae", position, 0.0f)[0];
        btBoxShape *shape = (btBoxShape *)guessShape(model);
        fail_unless(shape->isConvex() == true);
        btVector3 he = shape->getHalfExtentsWithMargin();
        printf("%f %f %f\n", he[0], he[1], he[2]);
        assert_float_equals(he[0], 1.0f);
        assert_float_equals(he[1], 1.0f);
        assert_float_equals(he[2], 1.0f);
}
END_TEST

Suite *shape_suite (void)
{
  Suite *s = suite_create ("Shape suite");
  TCase *tc_core = tcase_create ("Shape case");
  tcase_add_test (tc_core, test_shape);
  suite_add_tcase (s, tc_core);
  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = shape_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

