#include "s21_decimal.h"

void convert_dec_to_big(s21_decimal dec, s21_big_decimal* dec_big) {
    init_big_dec(dec_big);
    dec_big->bits[B_LOW] = dec.bits[LOW];
    dec_big->bits[B_MID] = dec.bits[MID];
    dec_big->bits[B_HIGH] = dec.bits[HIGH];
    dec_big->bits[B_SCALE] = dec.bits[SCALE];
    dec_big->value_type = dec.value_type;
}

int get_big_bit(s21_big_decimal val, int i) {
    int mask = 1 << (i % 32);
    val.bits[i / 32] &= mask;
    return val.bits[i / 32];
}

s21_big_decimal set_big_bit(s21_big_decimal val, int i) {
    int mask = 1 << (i % 32);
    val.bits[i / 32] |= mask;
    return val;
}

int get_big_sign(s21_big_decimal val) {
    int mask = 1 << 31;
    val.bits[B_SCALE] &= mask;
    return val.bits[B_SCALE];
}

s21_big_decimal set_big_sign(s21_big_decimal val) {
    int mask = 1 << 31;
    val.bits[B_SCALE] |= mask;
    return val;
}

s21_big_decimal rm_big_sign(s21_big_decimal val) {
    int mask = ~(1 << 31);
    val.bits[B_SCALE] &= mask;
    return val;
}

int get_big_scale(s21_big_decimal val) {
    val = rm_big_sign(val);
    return val.bits[B_SCALE] >> 16;
}

void set_big_scale(s21_big_decimal* val, int scale) {
    rm_big_scale(val);
    scale = scale << 16;
    val->bits[B_SCALE] |= scale;
}

void rm_big_scale(s21_big_decimal* val) {
    int mask = 1 << 31;
    val->bits[B_SCALE] &= mask;
}

void init_big_dec(s21_big_decimal* big_dec) {
    big_dec->bits[B_LOW] = 0;
    big_dec->bits[B_MID] = 0;
    big_dec->bits[B_HIGH] = 0;
    big_dec->bits[B_UP_LOW] = 0;
    big_dec->bits[B_UP_MID] = 0;
    big_dec->bits[B_UP_HIGH] = 0;
    big_dec->bits[B_SCALE] = 0;
    big_dec->value_type = s21_NORMAL_VALUE;
}

void left_shift_big_decimal(s21_big_decimal* val, int shift) {
    int i;
    s21_big_decimal res;
    init_big_dec(&res);
    for (i = 0; i + shift < 192; i++) {
        if (get_big_bit(*val, i)) res = set_big_bit(res, i + shift);
    }
    *val = res;
}

void ten_big_mult(s21_big_decimal* val) {
    s21_big_decimal buf_1, buf_3;
    buf_1 = *val;
    buf_3 = *val;
    left_shift_big_decimal(&buf_1, 1);
    left_shift_big_decimal(&buf_3, 3);
    *val = sum_big_wo_scale(buf_1, buf_3);
}

void convert_big_to_dec(s21_big_decimal big_dec, s21_decimal* dec) {
    int big_scale = get_big_scale(big_dec);
    int mod = 0;
    while (((big_dec.bits[B_UP_HIGH] != 0 || big_dec.bits[B_UP_MID] != 0 ||
             big_dec.bits[B_UP_LOW] != 0) &&
            big_scale) ||
           big_scale > 28) {
        big_scale--;
        big_dec = ten_big_div_mod_core(big_dec, &mod);
    }
    bank_round(&big_dec, mod);
    if (big_dec.bits[B_UP_HIGH] != 0 || big_dec.bits[B_UP_MID] != 0 ||
        big_dec.bits[B_UP_LOW] != 0) {
        init_dec(dec);
        if (get_big_sign(big_dec)) {
            dec->value_type = s21_NEGATIVE_INFINITY;
        } else {
            dec->value_type = s21_INFINITY;
        }
    } else {
        dec->bits[LOW] = big_dec.bits[B_LOW];
        dec->bits[MID] = big_dec.bits[B_MID];
        dec->bits[HIGH] = big_dec.bits[B_HIGH];
        dec->value_type = big_dec.value_type;
        set_scale(dec, big_scale);
    }
}

int first_big_bit(s21_big_decimal val) {
    int index = 0;
    int i;
    for (i = 191; i >= 0; i--)
        if (get_big_bit(val, i) && !index) index = i;
    if (!index && !get_big_bit(val, 0)) {
        index = -1;
    }
    return index;
}

s21_big_decimal big_dop_code(s21_big_decimal dec) {
    s21_big_decimal one = {{0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
    int i;
    for (i = B_UP_HIGH; i >= B_LOW; i--) {
        dec.bits[i] = ~dec.bits[i];
    }
    dec.value_type = s21_DOP_CODE;
    return sum_big_wo_scale(dec, one);
}

s21_big_decimal sum_big_wo_scale(s21_big_decimal x, s21_big_decimal y) {
    int mind = 0;
    int i;
    s21_big_decimal res;
    init_big_dec(&res);
    for (i = 0; i < 192; i++) {
        if (get_big_bit(x, i) && get_big_bit(y, i)) {
            if (mind) res = set_big_bit(res, i);
            mind = 1;
        } else if (!get_big_bit(x, i) && !get_big_bit(y, i)) {
            if (mind) {
                res = set_big_bit(res, i);
                mind = 0;
            }
        } else {
            if (mind) {
                mind = 1;
            } else {
                res = set_big_bit(res, i);
            }
        }
    }
    if (mind) {
        if (y.value_type != s21_DOP_CODE) {
            init_big_dec(&res);
            res.value_type = s21_INFINITY;
        } else {
            res.value_type = s21_NORMAL_VALUE;
        }
    }
    if (x.value_type == s21_DOP_CODE) {
        res.value_type = s21_DOP_CODE;
    }
    set_big_scale(&res, get_big_scale(x));
    return res;
}

s21_big_decimal ten_big_div_mod_core(s21_big_decimal dec, int* mod) {
    s21_big_decimal tmp_dec = {{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
    s21_big_decimal quotient = {{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
    s21_big_decimal one = {{0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
    int i;
    for (i = 191; i >= 0; i--) {
        left_shift_big_decimal(&tmp_dec, 1);
        if (get_big_bit(dec, i)) {
            tmp_dec = set_big_bit(tmp_dec, 0);
        }
        left_shift_big_decimal(&quotient, 1);
        if (tmp_dec.bits[LOW] >= 0xA) {
            tmp_dec.bits[LOW] -= 0xA;
            quotient = sum_big_wo_scale(quotient, one);
        }
    }
    *mod = tmp_dec.bits[LOW];
    if (get_big_sign(dec)) {
        quotient = set_big_sign(quotient);
    }
    return quotient;
}

void bank_round(s21_big_decimal* val, int mod) {
    s21_big_decimal one = {{0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0x0};
    if (mod > 5) {
        *val = sum_big_wo_scale(*val, one);
    } else if (mod == 5) {
        if (get_big_bit(*val, 0)) {
            *val = sum_big_wo_scale(*val, one);
        }
    }
}

int first_different_big_bit(s21_big_decimal x, s21_big_decimal y) {
    int ret = 0;
    int i;
    for (i = 191; i >= 0 && ret == 0; i--) {
        ret = get_big_bit(x, i) - get_big_bit(y, i);
    }
    return ret;
}
