#include "s21_decimal.h"

s21_decimal s21_mul(s21_decimal x, s21_decimal y) {
    s21_decimal res;
    if (x.value_type == s21_NORMAL_VALUE && y.value_type == s21_NORMAL_VALUE) {
        int sign = 0;
        if (get_sign(x) ^ get_sign(y)) sign = 1;
        s21_big_decimal x_big, y_big, res_big;
        init_big_dec(&x_big);
        init_big_dec(&y_big);
        init_big_dec(&res_big);
        init_dec(&res);
        convert_dec_to_big(x, &x_big);
        convert_dec_to_big(y, &y_big);
        int x_scale = get_big_scale(x_big);
        int y_scale = get_big_scale(y_big);
        int res_scale = x_scale + y_scale;
        for (int i = 0; i < 192; i++) {
            if (i) {
                left_shift_big_decimal(&x_big, 1);
            }
            if (get_big_bit(y_big, i)) {
                res_big = sum_big_wo_scale(res_big, x_big);
            }
        }
        set_big_scale(&res_big, res_scale);
        convert_big_to_dec(res_big, &res);
        if (sign) {
            if (res.value_type == s21_NORMAL_VALUE) {
                res = set_sign(res);
            } else {
                res.value_type = s21_NEGATIVE_INFINITY;
            }
        }
    } else if (x.value_type == s21_NAN || y.value_type == s21_NAN) {
        res.value_type = s21_NAN;
    } else if ((x.value_type == s21_NEGATIVE_INFINITY) ^
               (y.value_type == s21_NEGATIVE_INFINITY)) {
        res.value_type = s21_NEGATIVE_INFINITY;
    } else {
        res.value_type = s21_NEGATIVE_INFINITY;
    }
    return res;
}

s21_decimal s21_add(s21_decimal x, s21_decimal y) {
    s21_decimal ret = {{0x0, 0x0, 0x0, 0x0}, 0x0};
    if (incorrect_values_add(x, y, &ret) == TRUE) {
        normalize(&x, &y);
        ret = sum_wo_scale(x, y);
        int scales = 2 * get_sign(x) + get_sign(y);
        if (scales == 3 && ret.value_type == s21_NORMAL_VALUE) {
            ret = set_sign(ret);
        }
        if (scales == 2) {
            if (s21_is_less_or_equal(dec_abs(x), dec_abs(y)) == TRUE) {
                ret = simple_subtraction(y, x);
                if (s21_is_equal(dec_abs(x), dec_abs(y)) == TRUE) {
                    ret = rm_sign(ret);
                }
            } else {
                ret = simple_subtraction(x, y);
                ret = set_sign(ret);
            }
        }
        if (scales == 1) {
            if (s21_is_less_or_equal(dec_abs(x), dec_abs(y)) == TRUE) {
                ret = simple_subtraction(y, x);
                ret = set_sign(ret);
                if (s21_is_equal(dec_abs(x), dec_abs(y)) == TRUE) {
                    ret = rm_sign(ret);
                }
            } else {
                ret = simple_subtraction(x, y);
            }
        }
    }
    return ret;
}

s21_decimal s21_sub(s21_decimal x, s21_decimal y) {
    y = s21_negate(y);
    return s21_add(x, y);
}

s21_decimal s21_div(s21_decimal x, s21_decimal y) {
    s21_decimal tmp;
    return div_mod_core(x, y, &tmp);
}

s21_decimal s21_mod(s21_decimal x, s21_decimal y) {
    s21_decimal mod = {{0x0, 0x0, 0x0, 0x0}, 0x0};
    div_mod_core(x, y, &mod);
    if (get_sign(x)) {
        mod = set_sign(mod);
    }
    return mod;
}
