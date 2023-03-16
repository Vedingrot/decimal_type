# s21_decimal 

## Information

An interesting project about the implementation high precision floating
point datatype. Arithmetic decimal operators realized by bitwise operators,
which gives insign how computing systems works.

The Decimal value type represents decimal numbers ranging from 7.9e+29 to
-7.9e+29  The Decimal value type is appropriate for financial calculations
that require large numbers of significant integral and fractional digits
and no round-off errors.  The Decimal type does not eliminate the need
for rounding. Rather, it minimizes errors due to rounding.

The concept of a decimal number is implemented through a structure that
stores an array of 4 elements of the int type and one element of the
value_type_t type, indicating what kind of value it is.

A decimal number is a floating-point value that consists of a sign, a numeric
value where each digit in the value ranges from 0 to 9, and a scaling factor
that indicates the position of a floating decimal point that separates the
integral and fractional parts of the numeric value.

The scaling factor also can preserve any trailing zeros in a Decimal number.
Trailing zeros do not affect the value of a Decimal number in arithmetic
or comparison operations. 

## Build

Just call make to build library.

Requirements: gcc, make, libcheck (for tests).

## Decimal functions and operators

### Arithmetic Operators

| Operator name | Operators  | Function | 
| ------ | ------ | ------ |
| Addition | + | s21_decimal s21_add(s21_decimal, s21_decimal) |
| Subtraction | - | s21_decimal s21_sub(s21_decimal, s21_decimal) |
| Multiplication | * | s21_decimal s21_mul(s21_decimal, s21_decimal) | 
| Division | / | s21_decimal s21_div(s21_decimal, s21_decimal) |
| Modulo | Mod | s21_decimal s21_mod(s21_decimal, s21_decimal) |

### Comparison Operators

| Operator name | Operators  | Function | 
| ------ | ------ | ------ |
| Less than | < | int s21_is_less(s21_decimal, s21_decimal) |
| Less than or equal to | <= | int s21_is_less_or_equal(s21_decimal, s21_decimal) | 
| Greater than | > |  int s21_is_greater(s21_decimal, s21_decimal) |
| Greater than or equal to | >= | int s21_is_greater_or_equal(s21_decimal, s21_decimal) | 
| Equal to | == |  int s21_is_equal(s21_decimal, s21_decimal) |
| Not equal to | != |  int s21_is_not_equal(s21_decimal, s21_decimal) |

Return value:
- 0 - TRUE
- 1 - FALSE

### Convertors and parsers

| Convertor/parser | Function | 
| ------ | ------ |
| From int  | int s21_from_int_to_decimal(int src, s21_decimal *dst) |
| From float  | int s21_from_float_to_decimal(float src, s21_decimal *dst) |
| To int  | int s21_from_decimal_to_int(s21_decimal src, int *dst) |
| To float  | int s21_from_decimal_to_float(s21_decimal src, float *dst) |

Return value - code error:
- 0 - SUCCESS
- 1 - CONVERTING ERROR

### Another functions

| Description | Function | 
| ------ | ------ |
| Rounds a specified Decimal number to the closest integer toward negative infinity. | s21_decimal s21_floor(s21_decimal) |	
| Rounds a decimal value to the nearest integer. | s21_decimal s21_round(s21_decimal) |
| Returns the integral digits of the specified Decimal; any fractional digits are discarded, including trailing zeroes. | s21_decimal s21_truncate(s21_decimal) |
| Returns the result of multiplying the specified Decimal value by negative one. | s21_decimal s21_negate(s21_decimal) |

Written with Savin Anton(sav1nbrave4code).

