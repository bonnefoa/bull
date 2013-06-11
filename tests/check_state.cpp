#include <stdlib.h>
#include <check.h>
#include <common_tests.h>
#include <bullet.h>
#include <bl_matrix.h>
#include <bl_state.h>

void setup (void)
{
}

void teardown (void)
{
}

void printQuaternion(btQuaternion &quat)
{
        printf("Quat %f %f %f %f\n", quat.getX(), quat.getY(),
                        quat.getZ(), quat.getW());
}

void printBtVector(const btVector3 &vect)
{
        printf("%f %f %f\n", vect[0], vect[1], vect[2]);
}

void moveAndPrint(const btVector3 &origin, const btTransform &trans)
{
        btVector3 moved = trans(origin);
        printf("Moved %f %f %f\n", moved[0], moved[1], moved[2]);
}

START_TEST (test_angles)
{
        btQuaternion quatX = btQuaternion(btVector3(1,0,0), M_PI_2);
        btTransform transX = btTransform(quatX);

        btQuaternion quatY = btQuaternion(btVector3(0,1,0), M_PI_2);
        btTransform transY = btTransform(quatY);

        btQuaternion quatXY = quatY * quatX;

        printQuaternion(quatX);
        printQuaternion(quatY);
        printQuaternion(quatXY);
        btTransform transXY = btTransform(quatXY);
        printBtTransform(transXY);

        btVector3 origin = btVector3(1, 2, 3);
        moveAndPrint(origin, transX);
        moveAndPrint(origin, transY);
        moveAndPrint(origin, transXY);
}
END_TEST

START_TEST (test_direction)
{
        BlState blState = BlState(btVector3(0,0,0), NULL, NULL);
        float deltaUp = 0.0f;
        float deltaRight = 0.0f;
        blState.computeDirectionFromDelta(deltaUp, deltaRight);
        printBtVector(blState.direction);
        assert_vector_equals(blState.direction, btVector3(0,0,-1));

        deltaUp = M_PI_2;
        deltaRight = 0.0f;
        blState.computeDirectionFromDelta(deltaUp, deltaRight);
        printBtVector(blState.direction);
        assert_vector_equals(blState.direction, btVector3(0,1,0));

        deltaUp = -M_PI_2;
        deltaRight = M_PI_2;
        blState.computeDirectionFromDelta(deltaUp, deltaRight);
        printBtVector(blState.direction);
        assert_vector_equals(blState.direction, btVector3(1,0,0));
}
END_TEST


Suite *state_suite (void)
{
  Suite *s = suite_create ("State suite");
  TCase *tc_core = tcase_create ("State case");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test(tc_core, test_angles);
  tcase_add_test(tc_core, test_direction);
  suite_add_tcase(s, tc_core);
  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = state_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
