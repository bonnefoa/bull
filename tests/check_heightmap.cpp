#include <stdlib.h>
#include <bl_image.h>
#include <bl_heightmap.h>
#include <common_tests.h>
#include <check.h>

BlImage *testImage;

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
        int gridWidth = 4;
        int gridLenght = 4;
        int res = getDeltaPixelSum(8, 8, gridLenght, gridWidth, testImage);
        fail_unless(res != 0);
}
END_TEST

START_TEST (test_heightmap)
{
        BlImage *normal = generateNormalMapFromHeightmap(testImage);
        normal->writeImage("/tmp/toto.png");
}
END_TEST

Suite *heightmap_suite (void)
{
  Suite *s = suite_create ("Heightmap suite");
  TCase *tc_core = tcase_create("Heightmap case");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_heightmap);
  tcase_add_test (tc_core, test_delta_pixel);
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
