#include "s21_decimal.h"

int s21_is_less(s21_decimal x, s21_decimal y) {
    int ret = FALSE;
    if (comparer(x, y) == -1) {
        ret = TRUE;
    }
    return ret;
}

int s21_is_equal(s21_decimal x, s21_decimal y) {
    int ret = FALSE;
    if (comparer(x, y) == 0) {
        ret = TRUE;
    }
    return ret;
}

int s21_is_greater(s21_decimal x, s21_decimal y) {
    int ret = FALSE;
    if (comparer(x, y) == 1) {
        ret = TRUE;
    }
    return ret;
}

int s21_is_not_equal(s21_decimal x, s21_decimal y) {
    return !s21_is_equal(x, y);
}

int s21_is_less_or_equal(s21_decimal x, s21_decimal y) {
    int ret = FALSE;
    if (comparer(x, y) == -1 || comparer(x, y) == 0) {
        ret = TRUE;
    }
    return ret;
}

int s21_is_greater_or_equal(s21_decimal x, s21_decimal y) {
    int ret = FALSE;
    if (comparer(x, y) == 1 || comparer(x, y) == 0) {
        ret = TRUE;
    }
    return ret;
}

int comparer(s21_decimal x, s21_decimal y) {
    int res = -2;
    if (x.value_type == s21_NORMAL_VALUE && y.value_type == s21_NORMAL_VALUE) {
        int sign = 2 * get_sign(x) + get_sign(y);
        normalize(&x, &y);
        if (sign == 3) {
            res = -1 * first_different_bit(x, y);
        }
        if (sign == 2) {
            res = -1;
        }
        if (sign == 1) {
            res = 1;
        }
        if (sign == 0) {
            res = first_different_bit(x, y);
        }
    } else {
        if (x.value_type == s21_NEGATIVE_INFINITY &&
            y.value_type == s21_INFINITY) {
            res = -1;
        }
        if (x.value_type == s21_INFINITY &&
            y.value_type == s21_NEGATIVE_INFINITY) {
            res = 1;
        }
    }
    return res;
}
