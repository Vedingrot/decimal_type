.PHONY: clean all linter cppcheck test style_check gcov_report
CC = gcc
CFLAGS= -c -Wall -Werror -Wextra
LDFLAGS= -lcheck -L. -ls21_decimal
GFLAGS= -fprofile-arcs -ftest-coverage
HEADERS=s21_decimal.h
SOURCES=another.c arithmetics.c big_decimal_core.c converters.c \
	decimal_core.c equal.c help_functions.c  
VPATH = src/

OBJECTS=$(SOURCES:.c=.o)
OS := $(shell uname)

ifeq ($(OS), Linux)
	LDFLAGS += -lrt -lm -lpthread -lsubunit
endif

all: s21_decimal.a

$(OBJECTS): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $^

s21_decimal.a: $(OBJECTS)
	ar rc $@ $(OBJECTS)
	cp $@ lib$@

clean:
	rm -rf $(OBJECTS) *.a *.out *.gcda *.gcno *.info *.cfg gcov_report

test: test.c s21_decimal.a
	$(CC) $? -o test.out $(LDFLAGS)
	./test.out

gcov_report: $(SOURCES)
	$(CC) $(CFLAGS) $^ $(GFLAGS)
	ar rcs libs21_decimal.a $(OBJECTS)
	$(CC) -o gcov.out test.c $(LDFLAGS) -lgcov
	rm -f *.o
	./gcov.out
	lcov -c -d . -o coverage.info
	genhtml coverage.info -o gcov_report
	open gcov_report/index.html

.IGNORE: linter
linter:
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h
	rm -rf CPPLINT.cfg

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=shiftTooManyBitsSigned \
						  --suppress=invalidPointerCast --suppress=integerOverflow *.c *.h

style_check: linter cppcheck

leaks:
	CK_FORK=no leaks --atExit -- ./test.out
