#include <stdlib.h>
#include <bl_image.h>
#include <bl_heightmap.h>
#include <common_tests.h>
#include <check.h>
#include <vector>

BlImage *testImage;
int gridWidth = 4;
int gridLenght = 4;

void setup (void)
{
        size_t size = 16*16*3;
        unsigned char * pixels = (unsigned char *)malloc(size);
        memset(pixels, 0, size);
        pixels[8*16*3 + 8*3] = 255;
        testImage = new BlImage(16, 16, pixels, GL_RGB, RGB_CHANNEL);
}

void teardown (void)
{
        delete testImage;
}

START_TEST (test_delta_pixel)
{
        int res = getDeltaPixelSum(8, 8, gridLenght, gridWidth, testImage);
        fail_unless(res != 0);
}
END_TEST

START_TEST (test_off_grid)
{
        std::vector<btVector3> vertices =
                generateOffGrid(testImage, gridWidth, gridLenght);
        for (std::vector<btVector3>::iterator it = vertices.begin();
                                        it != vertices.end(); ++it) {
                printf("%f %f %f\n", (*it)[0], (*it)[1], (*it)[2]);
        }
}
END_TEST

START_TEST (test_gravity)
{
        float **gravity = extractGravityPoints(testImage, 4, 4);
        assert_float_equals(0.0f, gravity[0][0]);
        for(int i = 0; i < 2; i++) {
                for(int j = 0; j < 2; j++) {
                        assert_float_equals(gravity[i][j], 0.0f);
                }
        }
        fail_unless(gravity[2][2] > 0.0f);
}
END_TEST

Suite *heightmap_suite (void)
{
  Suite *s = suite_create ("Heightmap suite");
  TCase *tc_core = tcase_create("Heightmap case");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_gravity);
  tcase_add_test (tc_core, test_delta_pixel);
  tcase_add_test (tc_core, test_off_grid);
  suite_add_tcase (s, tc_core);
  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = heightmap_suite();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
