#include "s21_decimal.h"

int get_bit(s21_decimal val, int i) {
    int mask = 1 << (i % 32);
    return !!(val.bits[i / 32] & mask);
}

s21_decimal rm_bit(s21_decimal val, int i) {
    int mask = ~(1 << (i % 32));
    val.bits[i / 32] &= mask;
    return val;
}

s21_decimal set_bit(s21_decimal val, int i) {
    int mask = 1 << (i % 32);
    val.bits[i / 32] |= mask;
    return val;
}

int get_sign(s21_decimal val) {
    int mask = 1 << 31;
    val.bits[SCALE] &= mask;
    return !!val.bits[SCALE];
}

s21_decimal set_sign(s21_decimal val) {
    int mask = 1 << 31;
    val.bits[SCALE] |= mask;
    return val;
}

s21_decimal rm_sign(s21_decimal val) {
    int mask = ~(1 << 31);
    val.bits[SCALE] &= mask;
    return val;
}

int get_scale(s21_decimal val) {
    val = rm_sign(val);
    return val.bits[SCALE] >> 16;
}

void set_scale(s21_decimal* val, int scale) {
    rm_scale(val);
    scale = scale << 16;
    val->bits[SCALE] |= scale;
}

void rm_scale(s21_decimal* val) {
    int mask = 1 << 31;
    val->bits[SCALE] &= mask;
}

void init_dec(s21_decimal* dec) {
    dec->bits[LOW] = 0;
    dec->bits[MID] = 0;
    dec->bits[HIGH] = 0;
    dec->bits[SCALE] = 0;
    dec->value_type = s21_NORMAL_VALUE;
}

void left_shift_decimal(s21_decimal* val, int shift) {
    s21_decimal res;
    init_dec(&res);
    for (int i = 0; i + shift < 96; i++) {
        if (get_bit(*val, i)) res = set_bit(res, i + shift);
    }
    *val = res;
}

void right_shift_decimal(s21_decimal* val, int shift) {
    s21_decimal res;
    init_dec(&res);
    for (int i = 95; i - shift >= 0; i--) {
        if (get_bit(*val, i)) res = set_bit(res, i - shift);
    }
    *val = res;
}

s21_decimal dop_code(s21_decimal dec) {
    dec.bits[LOW] = ~dec.bits[LOW];
    dec.bits[MID] = ~dec.bits[MID];
    dec.bits[HIGH] = ~dec.bits[HIGH];
    dec.value_type = s21_DOP_CODE;
    s21_decimal one = {{0x1, 0x0, 0x0, 0x0}, s21_DOP_CODE};
    return sum_wo_scale(dec, one);
}

void ten_mult(s21_decimal* val) {
    s21_decimal buf_1, buf_3;
    buf_1 = *val;
    buf_3 = *val;
    left_shift_decimal(&buf_1, 1);
    left_shift_decimal(&buf_3, 3);
    *val = sum_wo_scale(buf_1, buf_3);
}

s21_decimal sum_wo_scale(s21_decimal x, s21_decimal y) {
    int mind = 0;
    s21_decimal res;
    init_dec(&res);
    for (int i = 0; i < 96; i++) {
        if (get_bit(x, i) && get_bit(y, i)) {
            if (mind) res = set_bit(res, i);
            mind = 1;
        } else if (!get_bit(x, i) && !get_bit(y, i)) {
            if (mind) {
                res = set_bit(res, i);
                mind = 0;
            }
        } else {
            if (mind) {
                mind = 1;
            } else {
                res = set_bit(res, i);
            }
        }
        set_scale(&res, get_scale(x));
    }
    if (mind) {
        if (y.value_type != s21_DOP_CODE) {
            init_dec(&res);
            if (!get_sign(x)) {
                res.value_type = s21_INFINITY;
            } else {
                res.value_type = s21_NEGATIVE_INFINITY;
            }
        } else {
            res.value_type = s21_NORMAL_VALUE;
        }
    }
    if (x.value_type == s21_DOP_CODE) {
        res.value_type = s21_DOP_CODE;
    }
    return res;
}

s21_decimal ten_div(s21_decimal dec) {
    int mod = 0;
    return ten_div_mod_core(dec, &mod);
}

s21_decimal ten_div_mod_core(s21_decimal dec, int* mod) {
    s21_decimal tmp_dec = {{0x0, 0x0, 0x0, 0x0}, 0x0};
    s21_decimal quotient = {{0x0, 0x0, 0x0, 0x0}, 0x0};
    for (int i = 95; i >= 0; i--) {
        left_shift_decimal(&tmp_dec, 1);
        if (get_bit(dec, i)) {
            tmp_dec = set_bit(tmp_dec, 0);
        }
        left_shift_decimal(&quotient, 1);
        if (tmp_dec.bits[LOW] >= 0xA) {
            tmp_dec.bits[LOW] -= 0xA;
            s21_decimal one = {{0x1, 0x0, 0x0, 0x0}, 0x0};
            quotient = s21_add(quotient, one);
        }
    }
    *mod = tmp_dec.bits[LOW];
    if (get_sign(dec)) {
        quotient = set_sign(quotient);
    }
    return quotient;
}

s21_decimal simple_subtraction(s21_decimal x, s21_decimal y) {
    y = dop_code(y);
    return sum_wo_scale(x, y);
}

s21_decimal dec_abs(s21_decimal x) {
    x = rm_sign(x);
    return x;
}

int first_different_bit(s21_decimal x, s21_decimal y) {
    int ret = 0;
    for (int i = 95; i >= 0 && ret == 0; i--) {
        ret = get_bit(x, i) - get_bit(y, i);
    }
    return ret;
}
