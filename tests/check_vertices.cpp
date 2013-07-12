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
        btVector3 bitangent;
        btVector3 tangent;
        computeTangentSpace(vert1, vert2, vert3,
                        uv1, uv2, uv3,
                        normal, bitangent, tangent);
        assert_vector_equals(normal, btVector3(0,0,-1));
        assert_vector_equals(bitangent, btVector3(0,1,0));
        assert_vector_equals(tangent, btVector3(1,0,0));
}
END_TEST

START_TEST (test_nan)
{
        btVector3 vert1(30.5, 0, 30.5);
        btVector3 vert2(30.5, 0, 31.5);
        btVector3 vert3(31.5, 0, 30.5);
        btVector3 uv1(0.968750, 0.96875, 0);
        btVector3 uv2(0.968750, 0.984375, 0);
        btVector3 uv3(0.984375, 0.968750, 0);
        btVector3 normal;
        btVector3 bitangent;
        btVector3 tangent;
        computeTangentSpace(vert1, vert2, vert3,
                        uv1, uv2, uv3,
                        normal, bitangent, tangent);
        printf("Normal : %f % f %f\n", normal[0], normal[1], normal[2]);
        printf("bitangent : %f % f %f\n", bitangent[0], bitangent[1], bitangent[2]);
        printf("Tangent : %f % f %f\n", tangent[0], tangent[1], tangent[2]);
        assert_vector_equals(normal, btVector3(0,1,0));
        assert_vector_equals(bitangent, btVector3(0,1,0));
        assert_vector_equals(tangent, btVector3(1,0,0));
}
END_TEST

Suite *vertice_suite(void)
{
  Suite *s = suite_create ("Vertice suite");
  TCase *tc_core = tcase_create ("Vertice case case");
  tcase_add_test (tc_core, test_tangent_space);
  tcase_add_test (tc_core, test_nan);
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
