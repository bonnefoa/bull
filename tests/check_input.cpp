#include <stdlib.h>
#include <check.h>
#include <bl_input.h>
#include <bl_util.h>
#include <common_tests.h>

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
        btTransform projection = input.computeProjection(fov, aspect, zNear, zFar);
        btVector3 point = btVector3(4.0f, 3.0f, -15.0f);
        btVector3 res = projection * point;
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
        btTransform projection = input.computeProjection(fov, aspect, zNear, zFar);
        btVector3 point = btVector3(4.0f, 3.0f, -15.0f);
        btVector3 res = projection * point;
        assert_float_equals(3.0f / sqrt(3), res[0]);
        assert_float_equals(3.0f / sqrt(3), res[1]);
        assert_float_equals(15.0f, res[2]);
}
END_TEST

START_TEST (test_view)
{
        BlInput input = BlInput();
        input.handleMovement();
        assert_float_equals(input.view.getBasis()[0][0], 1.0f);
        assert_float_equals(input.view.getBasis()[1][1], 1.0f);
        assert_float_equals(input.view.getBasis()[2][2], 1.0f);

        input.theta = M_PI_2;
        input.phi = 0;
        input.handleMovement();
        assert_float_equals(input.view.getBasis()[0][0], 1.0f);
        assert_float_equals(input.view.getBasis()[1][1], 1.0f);
        assert_float_equals(input.view.getBasis()[2][2], 1.0f);

        btVector3 position = btVector3(0, 0, 5);
        btVector3 direction = btVector3(0, 0, -1);
        btVector3 up = btVector3(0, 1, 0);
        btVector3 right = btVector3(-1, 0, 0);
        btTransform view = input.computeView(right, up, direction, position);

        btVector3 center = view * btVector3(0, 0, 0);
        assert_float_equals(0.0f, center[0]);
        assert_float_equals(0.0f, center[1]);
        assert_float_equals(5.0f, center[2]);

        btVector3 upPoint = view * btVector3(0, 1, 5);
        assert_float_equals(0.0f, upPoint[0]);
        assert_float_equals(1.0f, upPoint[1]);
        assert_float_equals(10.0f, upPoint[2]);

        btVector3 rightPoint = view * btVector3(1, 1, 0);
        assert_float_equals(-1.0f, rightPoint[0]);
        assert_float_equals(1.0f, rightPoint[1]);
        assert_float_equals(5.0f, rightPoint[2]);
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

