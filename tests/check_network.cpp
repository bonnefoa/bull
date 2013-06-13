#include <stdlib.h>
#include <check.h>
#include <common_tests.h>
#include <bl_network.h>

void setup (void)
{
}

void teardown (void)
{
}

START_TEST (test_network)
{
        BlNetwork blNetwork = BlNetwork("localhost", 6666);
        blNetwork.init();
        blNetwork.connect();
        unsigned char *text = (unsigned char *)"test\n";
        blNetwork.sendText(text, sizeof(text));
}
END_TEST

Suite *network_suite (void)
{
  Suite *s = suite_create ("Network suite");
  TCase *tc_core = tcase_create ("Network case");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_network);
  suite_add_tcase (s, tc_core);
  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = network_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
