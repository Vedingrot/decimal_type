#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
    int normal = SUCCESS;
    if (dst == NULL) {
        normal = CONVERTING_ERROR;
    } else {
        init_dec(dst);
        if (src < 0) {
            *dst = set_sign(*dst);
            src *= -1;
        }
        dst->bits[LOW] |= src;
        dst->value_type = s21_NORMAL_VALUE;
    }
    return normal;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
    int normal = SUCCESS;
    if (dst == NULL) {
        normal = CONVERTING_ERROR;
    } else {
        init_dec(dst);
        if (src == src && src != INFINITY && src != -INFINITY) {
            if (src == 0.F) {
                normal = SUCCESS;
            } else {
                int sign = 0;
                if (src < 0.F) {
                    sign = 1;
                    src *= -1;
                }
                double tmp = fabs(src);
                int exp = get_bin_exp(src);
                float fl_buf = 0.;
                int scale = 0;
                if (exp < 95) {
                    while (scale < 28 && ((int)tmp / 2097152) == 0) {
                        tmp *= 10;
                        scale++;
                    }
                }
                tmp = round(tmp);
                if (scale <= 28 && (exp >= -94 && exp < 96)) {
                    tmp = (float)tmp;
                    while (fmod(tmp, 10) == 0 && scale > 0) {
                        tmp /= 10;
                        scale--;
                    }
                }
                fl_buf = tmp;
                exp = get_bin_exp(fl_buf);
                *dst = set_bit(*dst, exp);
                for (int i = 23; i >= 0; i--) {
                    if (get_float_bit(fl_buf, i)) {
                        *dst = set_bit(*dst, exp);
                    }
                    exp--;
                }
                if (exp < -94 || scale > 28) {
                    normal = CONVERTING_ERROR;
                    init_dec(dst);
                } else {
                    set_scale(dst, scale);
                    if (sign) *dst = set_sign(*dst);
                }
            }
        } else {
            normal = CONVERTING_ERROR;
            if (src != src) {
                dst->value_type = s21_NAN;
            } else if (src == INFINITY) {
                dst->value_type = s21_INFINITY;
            } else {
                dst->value_type = s21_NEGATIVE_INFINITY;
            }
        }
    }
    return normal;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
    int normal = SUCCESS;
    int sign = get_sign(src);
    if (dst == NULL) {
        normal = CONVERTING_ERROR;
    } else {
        src = s21_truncate(src);
        for (int i = 95; i >= 0; i--) {
            if (get_bit(src, i) && i > 31) {
                normal = CONVERTING_ERROR;
                break;
            } else if (get_bit(src, i) && i <= 31) {
                *dst += pow(2, i);
            }
        }
        if (sign) {
            *dst *= -1;
        }
    }
    return normal;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
    int normal = SUCCESS;
    if (dst == NULL) {
        normal = CONVERTING_ERROR;
    } else {
        if (src.value_type == s21_NORMAL_VALUE) {
            *dst = 0;
            int scale = get_scale(src);
            int sign = get_sign(src);
            for (int i = 0; i < 96; i++) {
                if (get_bit(src, i)) *dst += pow(2, i);
            }
            *dst *= pow(-1, sign) * pow(10, -scale);
        } else if (src.value_type == s21_INFINITY) {
            *dst = INFINITY;
        } else if (src.value_type == s21_NEGATIVE_INFINITY) {
            *dst = -INFINITY;
        } else if (src.value_type == s21_NAN) {
            *dst = NAN;
        }
    }
    return normal;
}
