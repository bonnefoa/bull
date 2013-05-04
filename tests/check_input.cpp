#include <stdlib.h>
#include <check.h>
#include <bl_input.h>
#include <bl_util.h>

#define EPSILON 0.01f
#define assert_float_equals(res, expected) \
        fail_unless(fabs(res - expected) < EPSILON\
                        , "Expected %f, got %f", expected, res)

void setup (void)
{
}

void teardown (void)
{
}

START_TEST (test_projection_simple)
{
        BlInput input = BlInput();
        btScalar fov = M_PI_2;
        btScalar zNear = 5.0f;
        btScalar zFar = 15.0f;
        btScalar aspect = 4.0f/3.0f;
        input.computeProjection(fov, aspect, zNear, zFar);
        btVector3 point = btVector3(4.0f, 3.0f, -15.0f);
        btVector3 res = input.projection * point;
        assert_float_equals(3.0f, res[0]);
        assert_float_equals(3.0f, res[1]);
        assert_float_equals(15.0f, res[2]);
}
END_TEST

START_TEST (test_projection_pi_3)
{
        BlInput input = BlInput();
        btScalar fov = 2 * M_PI / 3;
        btScalar zNear = 5.0f;
        btScalar zFar = 15.0f;
        btScalar aspect = 4.0f/3.0f;
        input.computeProjection(fov, aspect, zNear, zFar);
        btVector3 point = btVector3(4.0f, 3.0f, -15.0f);
        btVector3 res = input.projection * point;
        assert_float_equals(3.0f / sqrt(3), res[0]);
        assert_float_equals(3.0f / sqrt(3), res[1]);
        assert_float_equals(15.0f, res[2]);
}
END_TEST

START_TEST (test_view)
{
        BlInput input = BlInput();
        btVector3 lookAt = btVector3(1, 1, 1);
        btVector3 right = btVector3(1,0,0);
        btVector3 eye = btVector3(1, 1, -1);
        input.computeView(lookAt, right, eye);
        btTransform view = input.view;

        btVector3 center = view * lookAt;
        assert_float_equals(0.0f, center[0]);
        assert_float_equals(0.0f, center[1]);
        assert_float_equals(2.0f, center[2]);
}
END_TEST

Suite *model_suite (void)
{
  Suite *s = suite_create ("Input suite");
  TCase *tc_core = tcase_create ("Input case");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_view);
  tcase_add_test (tc_core, test_projection_simple);
  tcase_add_test (tc_core, test_projection_pi_3);
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

