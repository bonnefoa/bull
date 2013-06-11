#define EPSILON 0.01f
#define assert_float_equals(res, expected) \
        fail_unless(fabs(res - expected) < EPSILON\
                        , "Expected %f, got %f", expected, res)
#define assert_vector_equals(first, second) \
        assert_float_equals(first[0],second[0]); \
        assert_float_equals(first[1],second[1]); \
        assert_float_equals(first[2],second[2]);
