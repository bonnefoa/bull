#include <stdlib.h>
#include <check.h>
#include <common_tests.h>
#include <bl_vertice.h>

START_TEST (test_tangent_space)
{
        btVector3 vert1(0, 20, 0);
        btVector3 vert2(20, 20, 0);
        btVector3 vert3(0, 0, 0);
        btVector3 uv1(0,0,0);
        btVector3 uv2(1,0,0);
        btVector3 uv3(0,1,0);
        btVector3 normal;
        btVector3 binormal;
        btVector3 tangent;
        computeTangentSpace(vert1, vert2, vert3,
                        uv1, uv2, uv3,
                        normal, binormal, tangent);
        assert_vector_equals(normal, btVector3(0,0,-1));
        assert_vector_equals(binormal, btVector3(0,1,0));
        assert_vector_equals(tangent, btVector3(1,0,0));
}
END_TEST

Suite *vertice_suite(void)
{
  Suite *s = suite_create ("Vertice suite");
  TCase *tc_core = tcase_create ("Vertice case case");
  tcase_add_test (tc_core, test_tangent_space);
  suite_add_tcase (s, tc_core);
  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = vertice_suite();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
