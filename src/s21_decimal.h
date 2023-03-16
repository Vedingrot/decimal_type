#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>

typedef enum {
    s21_NORMAL_VALUE = 0,
    s21_INFINITY = 1,
    s21_NEGATIVE_INFINITY = 2,
    s21_NAN = 3,
    s21_DOP_CODE = 4
} value_type_t;

typedef struct {
    int bits[4];
    value_type_t value_type;
} s21_decimal;

typedef struct {
    int bits[7];
    value_type_t value_type;
} s21_big_decimal;

typedef enum { LOW = 0, MID = 1, HIGH = 2, SCALE = 3 } rank;

typedef enum {
    B_LOW = 0,
    B_MID = 1,
    B_HIGH = 2,
    B_UP_LOW = 3,
    B_UP_MID = 4,
    B_UP_HIGH = 5,
    B_SCALE = 6
} b_rank;

typedef enum { SUCCESS = 0, CONVERTING_ERROR = 1 } convert;

typedef enum { TRUE = 0, FALSE = 1 } compare;

s21_decimal s21_add(s21_decimal x, s21_decimal y);
s21_decimal s21_sub(s21_decimal x, s21_decimal y);
s21_decimal s21_mul(s21_decimal x, s21_decimal y);
s21_decimal s21_div(s21_decimal x, s21_decimal y);
s21_decimal s21_mod(s21_decimal x, s21_decimal y);

int s21_is_less(s21_decimal x, s21_decimal y);
int s21_is_less_or_equal(s21_decimal x, s21_decimal y);
int s21_is_greater(s21_decimal x, s21_decimal y);
int s21_is_greater_or_equal(s21_decimal x, s21_decimal y);
int s21_is_equal(s21_decimal x, s21_decimal y);
int s21_is_not_equal(s21_decimal x, s21_decimal y);

int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_float_to_decimal(float src, s21_decimal* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);

s21_decimal s21_floor(s21_decimal dec);
s21_decimal s21_round(s21_decimal dec);
s21_decimal s21_truncate(s21_decimal dec);
s21_decimal s21_negate(s21_decimal x);

int get_bit(s21_decimal val, int i);
s21_decimal set_bit(s21_decimal val, int i);
s21_decimal rm_bit(s21_decimal val, int i);

int get_big_bit(s21_big_decimal val, int i);
s21_big_decimal set_big_bit(s21_big_decimal val, int i);

int get_sign(s21_decimal val);
s21_decimal set_sign(s21_decimal val);
s21_decimal rm_sign(s21_decimal val);

int get_big_sign(s21_big_decimal val);
s21_big_decimal set_big_sign(s21_big_decimal val);
s21_big_decimal rm_big_sign(s21_big_decimal val);

int get_scale(s21_decimal val);
void set_scale(s21_decimal* val, int scale);
void rm_scale(s21_decimal* val);

int get_big_scale(s21_big_decimal val);
void set_big_scale(s21_big_decimal* val, int scale);
void rm_big_scale(s21_big_decimal* val);

int first_different_bit(s21_decimal x, s21_decimal y);
int first_different_big_bit(s21_big_decimal x, s21_big_decimal y);
int first_big_bit(s21_big_decimal val);

void init_dec(s21_decimal* dec);
void init_big_dec(s21_big_decimal* big_dec);

void show_float(float val);
void show_int(int val);
void show_decimal(s21_decimal dec);
void print_decimal_in_hex(s21_decimal dec);
void print_big_decimal_in_hex(s21_big_decimal dec);
void show_big_decimal(s21_big_decimal big_dec);
void init_big_dec(s21_big_decimal* big_dec);
s21_decimal dec_abs(s21_decimal x);

void left_shift_decimal(s21_decimal* val, int shift);
void right_shift_decimal(s21_decimal* val, int shift);

void left_shift_big_decimal(s21_big_decimal* val, int shift);
void right_shift_big_decimal(s21_big_decimal* val, int shift);

int get_bin_exp(float val);

void ten_mult(s21_decimal* val);
void ten_big_mult(s21_big_decimal* val);
s21_decimal ten_div(s21_decimal dec);
s21_decimal ten_div_mod_core(s21_decimal dec, int* mod);
s21_decimal div_mod_core(s21_decimal x, s21_decimal y, s21_decimal* mod);
s21_big_decimal ten_big_div_mod_core(s21_big_decimal dec, int* mod);
void bank_round(s21_big_decimal* val, int mod);

s21_decimal sum_wo_scale(s21_decimal x, s21_decimal y);
s21_decimal simple_subtraction(s21_decimal x, s21_decimal y);
int comparer(s21_decimal x, s21_decimal y);

s21_big_decimal sum_big_wo_scale(s21_big_decimal x, s21_big_decimal y);

void normalize(s21_decimal* x, s21_decimal* y);
s21_decimal normalize_rounding(s21_decimal x);
int overflow_control(s21_decimal dec);

void convert_dec_to_big(s21_decimal dec, s21_big_decimal* big_dec);
void convert_big_to_dec(s21_big_decimal big_dec, s21_decimal* dec);

s21_decimal dop_code(s21_decimal dec);
s21_big_decimal big_dop_code(s21_big_decimal dec);
int div_normalize(s21_big_decimal* big_x, s21_big_decimal* big_y);

int get_float_bit(float val, int i);
int incorrect_values_div(s21_decimal x, s21_decimal y, s21_decimal* res);
int incorrect_values_add(s21_decimal x, s21_decimal y, s21_decimal* res);


#endif  /* SRC_S21_DECIMAL_H_ */
