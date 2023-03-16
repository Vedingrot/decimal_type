#include "s21_decimal.h"

s21_decimal s21_floor(s21_decimal dec) {
    dec = s21_truncate(dec);
    if (get_sign(dec)) {
        s21_decimal one = {{0x1, 0x0, 0x0, 0x0}, 0x0};
        one = set_sign(one);
        dec = s21_add(dec, one);
    }
    return dec;
}

s21_decimal s21_round(s21_decimal dec) {
    s21_decimal one = {{0x1, 0x0, 0x0, 0x0}, 0x0};
    if (get_sign(dec)) {
        one = set_sign(one);
    }
    int scale = get_scale(dec);
    int mod = 0;
    while (scale >= 1) {
        dec = ten_div_mod_core(dec, &mod);
        set_scale(&dec, --scale);
    }
    dec = s21_truncate(dec);
    if (mod > 5) {
        dec = s21_add(dec, one);
    }
    if (mod == 5 && get_bit(dec, 0)) {
        dec = s21_add(dec, one);
    }
    return dec;
}

s21_decimal s21_truncate(s21_decimal dec) {
    int scale = get_scale(dec);
    while (scale > 0) {
        dec = ten_div(dec);
        set_scale(&dec, --scale);
    }
    return dec;
}

s21_decimal s21_negate(s21_decimal x) {
    if (get_sign(x)) {
        x = rm_sign(x);
    } else {
        x = set_sign(x);
    }
    return x;
}
