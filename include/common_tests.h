#define EPSILON 0.01f
#define assert_float_equals(res, expected) \
        fail_unless(fabs(res - expected) < EPSILON\
                        , "Expected %f, got %f", expected, res)

#define assert_vector_equals(first, second) \
        fail_unless(fabs(first[0] - second[0]) < EPSILON \
                        && fabs(first[1] - second[1]) < EPSILON \
                        && fabs(first[2] - second[2]) < EPSILON \
                        , "Expected [%f,%f,%f], got [%f,%f,%f]", \
                        first[0], first[1], first[2], \
                        second[0], second[1], second[2])


#define assert_quaternion_equals(first, second) \
        fail_unless(fabs(first[0] - second[0]) < EPSILON \
                        && fabs(first[1] - second[1]) < EPSILON \
                        && fabs(first[2] - second[2]) < EPSILON \
                        && fabs(first[3] - second[3]) < EPSILON \
                        , "Expected [%f,%f,%f,%f], got [%f,%f,%f,%f]", \
                        first[0], first[1], first[2], first[3], \
                        second[0], second[1], second[2], second[3])
