C89=-std=c89 -Wpedantic
GNU89=-std=gnu89 -Wpedantic
CFLAGS=-ggdb -O2 -Wall -Wextra -Werror

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0


default: check


# 2015-12-01
2015-12-01-1: 2015/12/01/day1part1.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-01-1 2015/12/01/day1part1.c

2015-12-01-2: 2015/12/01/day1part2.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-01-2 2015/12/01/day1part2.c

check-2015-12-01-1: 2015-12-01-1
	./check-answer.sh 74 bin/2015-12-01-1 2015/12/01/input

check-2015-12-01-2: 2015-12-01-2
	./check-answer.sh 1795 bin/2015-12-01-2 2015/12/01/input


# 2015-12-02
2015-12-02-1: 2015/12/02/day2part1.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-02-1 2015/12/02/day2part1.c

2015-12-02-2: 2015/12/02/day2part2.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-02-2 2015/12/02/day2part2.c

check-2015-12-02-1: 2015-12-02-1
	./check-answer.sh 1586300 bin/2015-12-02-1 2015/12/02/input

check-2015-12-02-2: 2015-12-02-2
	./check-answer.sh 3737498 bin/2015-12-02-2 2015/12/02/input


# 2015-12-03
2015-12-03-1: 2015/12/03/day3part1.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-03-1 \
		-Icommon/ehht common/ehht/*.c \
		2015/12/03/day3part1.c

2015-12-03-2: 2015/12/03/day3part2.c
	gcc $(C99) $(CFLAGS) -o bin/2015-12-03-2 \
		-Icommon/ehht common/ehht/*.c \
		2015/12/03/day3part2.c

check-2015-12-03-1: 2015-12-03-1
	./check-answer.sh 2572 bin/2015-12-03-1 2015/12/03/input

check-2015-12-03-2: 2015-12-03-2
	./check-answer.sh 2631 bin/2015-12-03-2 2015/12/03/input


# 2015-12-04
2015-12-04: 2015/12/04/day4.c
	# GNU89 for strnlen
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-04 2015/12/04/day4.c -lcrypto

check-2015-12-04-1: 2015-12-04
	./check-answer.sh 346386 bin/2015-12-04 5

check-2015-12-04-2: 2015-12-04
	./check-answer.sh 9958218 bin/2015-12-04 6


# 2015-12-05
2015-12-05-1: 2015/12/05/day5part1.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-05-1 2015/12/05/day5part1.c

2015-12-05-2: 2015/12/05/day5part2.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-05-2 2015/12/05/day5part2.c

check-2015-12-05-1: 2015-12-05-1
	./check-answer.sh 238 bin/2015-12-05-1 2015/12/05/input

check-2015-12-05-2: 2015-12-05-2
	./check-answer.sh 69 bin/2015-12-05-2 2015/12/05/input


# 2015-12-06
2015-12-06-1: 2015/12/06/day6part1.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-06-1 2015/12/06/day6part1.c

2015-12-06-2: 2015/12/06/day6part2.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-06-2 2015/12/06/day6part2.c

check-2015-12-06-1: 2015-12-06-1
	./check-answer.sh 377891 bin/2015-12-06-1 2015/12/06/input

check-2015-12-06-2: 2015-12-06-2
	./check-answer.sh 14110788 bin/2015-12-06-2 2015/12/06/input


# 2015-12-07
2015-12-07: 2015/12/07/day7.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-07 \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/07/day7.c

check-2015-12-07-1: 2015-12-07
	./check-answer.sh 16076 bin/2015-12-07 2015/12/07/input

check-2015-12-07-2: 2015-12-07
	./check-answer.sh 2797 bin/2015-12-07 2015/12/07/input 16076


# 2015-12-08
2015-12-08: 2015/12/08/day8.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-08 2015/12/08/day8.c

check-2015-12-08-1: 2015-12-08
	./check-answer.sh 1350 bin/2015-12-08 2015/12/08/input

check-2015-12-08-2: 2015-12-08
	./check-answer.sh 2085 bin/2015-12-08 2015/12/08/input 1


# 2015-12-09
2015-12-09: 2015/12/09/day9.c
	gcc $(c89) $(CFLAGS) -o bin/2015-12-09 \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		-Icommon/misc common/misc/*.c \
		2015/12/09/day9.c

check-2015-12-09-1: 2015-12-09
	./check-answer.sh 251 bin/2015-12-09 2015/12/09/input

check-2015-12-09-2: 2015-12-09
	./check-answer.sh 898 bin/2015-12-09 2015/12/09/input 1


# 2015-12-10
2015-12-10: 2015/12/10/day10.c
	# GNU89 for snprintf
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-10 2015/12/10/day10.c

check-2015-12-10-1: 2015-12-10
	./check-answer.sh 252594 bin/2015-12-10 40 1113222113

check-2015-12-10-2: 2015-12-10
	./check-answer.sh 3579328 bin/2015-12-10 50 1113222113


# 2015-12-11
2015-12-11: 2015/12/11/day11.c
	# GNU89 for snprintf
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-11 2015/12/11/day11.c

check-2015-12-11-1: 2015-12-11
	./check-answer.sh cqjxxyzz bin/2015-12-11 cqjxjnds

check-2015-12-11-2: 2015-12-11
	./check-answer.sh cqkaabcc bin/2015-12-11 cqjxxyzz


# 2015-12-12
2015-12-12-1: 2015/12/12/day12part1.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-12-1 \
		-Icommon/deque common/deque/*.c \
		2015/12/12/day12part1.c

2015-12-12-2: 2015/12/12/day12part2.c
	gcc $(C99) $(CFLAGS) -o bin/2015-12-12-2 \
		-Icommon/deque common/deque/*.c \
		2015/12/12/day12part2.c

check-2015-12-12-1: 2015-12-12-1
	./check-answer.sh 119433 bin/2015-12-12-1 2015/12/12/input

check-2015-12-12-2: 2015-12-12-2
	./check-answer.sh 68466 bin/2015-12-12-2 2015/12/12/input


# 2015-12-13
2015-12-13: 2015/12/13/day13.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-13 \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		-Icommon/misc common/misc/*.c \
		2015/12/13/day13.c

check-2015-12-13-1: 2015-12-13
	./check-answer.sh 664 bin/2015-12-13 2015/12/13/input

check-2015-12-13-2: 2015-12-13
	./check-answer.sh 640 bin/2015-12-13 2015/12/13/input 1


# 2015-12-14
2015-12-14: 2015/12/14/day14.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-14 2015/12/14/day14.c

check-2015-12-14-1: 2015-12-14
	./check-answer.sh 2640 bin/2015-12-14

check-2015-12-14-2: 2015-12-14
	./check-answer.sh 1102 bin/2015-12-14 1


# 2015-12-15
2015-12-15: 2015/12/15/day15.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-15 2015/12/15/day15.c

check-2015-12-15-1: 2015-12-15
	./check-answer.sh 13882464 bin/2015-12-15

check-2015-12-15-2: 2015-12-15
	./check-answer.sh 11171160 bin/2015-12-15 500


# 2015-12-16
2015-12-16: 2015/12/16/day16.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-16 2015/12/16/day16.c

check-2015-12-16-1: 2015-12-16
	./check-answer.sh 373 bin/2015-12-16 2015/12/16/input

check-2015-12-16-2: 2015-12-16
	./check-answer.sh 260 bin/2015-12-16 2015/12/16/input 1


# 2015-12-17
2015-12-17: 2015/12/17/day17.c
	gcc $(c89) $(CFLAGS) -o bin/2015-12-17 \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/17/day17.c

check-2015-12-17-1: 2015-12-17
	./check-answer.sh 1638 bin/2015-12-17 2015/12/17/input

check-2015-12-17-2: 2015-12-17
	./check-answer.sh 17 bin/2015-12-17 2015/12/17/input 1


# 2015-12-18
2015-12-18: 2015/12/18/day18.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-18 2015/12/18/day18.c

check-2015-12-18-1: 2015-12-18
	./check-answer.sh 768 bin/2015-12-18 2015/12/18/input

check-2015-12-18-2: 2015-12-18
	./check-answer.sh 781 bin/2015-12-18 2015/12/18/input 1


# 2015-12-19
2015-12-19: 2015/12/19/day19.c
	gcc $(c89) $(CFLAGS) -o bin/2015-12-19 \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/19/day19.c

check-2015-12-19-1: 2015-12-19
	./check-answer.sh 576 bin/2015-12-19 2015/12/19/input

check-2015-12-19-2: 2015-12-19
	./check-answer.sh 207 bin/2015-12-19 2015/12/19/input 1


# 2015-12-20
2015-12-20: 2015/12/20/day20.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-20 2015/12/20/day20.c

check-2015-12-20-1: 2015-12-20
	./check-answer.sh 776160 bin/2015-12-20 2015/12/20/input

check-2015-12-20-2: 2015-12-20
	./check-answer.sh 786240 bin/2015-12-20 2015/12/20/input 1


# 2015-12-21
2015-12-21: 2015/12/21/day21.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-21 2015/12/21/day21.c

check-2015-12-21-1: 2015-12-21
	./check-answer.sh 91 bin/2015-12-21 2015/12/21/input

check-2015-12-21-2: 2015-12-21
	./check-answer.sh 158 bin/2015-12-21 2015/12/21/input 1


# 2015-12-22
2015-12-22: 2015/12/22/day22.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-22 2015/12/22/day22.c

check-2015-12-22-1: 2015-12-22
	./check-answer.sh 1824 bin/2015-12-22 2015/12/22/input

check-2015-12-22-2: 2015-12-22
	./check-answer.sh 1937 bin/2015-12-22 2015/12/22/input 1


# 2015-12-23
2015-12-23: 2015/12/23/day23.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-23 2015/12/23/day23.c

check-2015-12-23-1: 2015-12-23
	./check-answer.sh 255 bin/2015-12-23 2015/12/23/input

check-2015-12-23-2: 2015-12-23
	./check-answer.sh 334 bin/2015-12-23 2015/12/23/input 1


# 2015-12-24
2015-12-24: 2015/12/24/day24.c
	gcc $(c89) $(CFLAGS) -o bin/2015-12-24 \
		-Icommon/eba common/eba/*.c \
		-Icommon/ehbigint common/ehbigint/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/24/day24.c

check-2015-12-24-1: 2015-12-24
	./check-answer.sh 11846773891 bin/2015-12-24 2015/12/24/input

check-2015-12-24-2: 2015-12-24
	./check-answer.sh 80393059 bin/2015-12-24 2015/12/24/input 1


# 2015-12-25
2015-12-25: 2015/12/25/day25.c
	gcc $(c89) $(CFLAGS) -o bin/2015-12-25 \
		-Icommon/eba common/eba/*.c \
		-Icommon/ehbigint common/ehbigint/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/25/day25.c

check-2015-12-25: 2015-12-25
	./check-answer.sh 8997277 bin/2015-12-25 3010 3019



# 2016-12-01
2016-12-01-1: 2016/12/01/day1part1.c
	gcc $(GNU89) $(CFLAGS) -o bin/2016-12-01-1 \
		-Icommon/ehht common/ehht/*.c \
		2016/12/01/day1part1.c

2016-12-01-2: 2016/12/01/day1part2.c
	gcc $(C99) $(CFLAGS) -o bin/2016-12-01-2 \
		-Icommon/ehht common/ehht/*.c \
		2016/12/01/day1part2.c

check-2016-12-01-1: 2016-12-01-1
	./check-answer.sh 209 bin/2016-12-01-1 2016/12/01/input

check-2016-12-01-2: 2016-12-01-2
	./check-answer.sh 136 bin/2016-12-01-2 2016/12/01/input



check: \
	check-2015-12-01-1 check-2015-12-01-2 \
	check-2015-12-02-1 check-2015-12-02-2 \
	check-2015-12-03-1 check-2015-12-03-2 \
	check-2015-12-04-1 check-2015-12-04-2 \
	check-2015-12-05-1 check-2015-12-05-2 \
	check-2015-12-06-1 check-2015-12-06-2 \
	check-2015-12-07-1 check-2015-12-07-2 \
	check-2015-12-08-1 check-2015-12-08-2 \
	check-2015-12-09-1 check-2015-12-09-2 \
	check-2015-12-10-1 check-2015-12-10-2 \
	check-2015-12-11-1 check-2015-12-11-2 \
	check-2015-12-12-1 check-2015-12-12-2 \
	check-2015-12-13-1 check-2015-12-13-2 \
	check-2015-12-14-1 check-2015-12-14-2 \
	check-2015-12-15-1 check-2015-12-15-2 \
	check-2015-12-16-1 check-2015-12-16-2 \
	check-2015-12-17-1 check-2015-12-17-2 \
	check-2015-12-18-1 check-2015-12-18-2 \
	check-2015-12-19-1 check-2015-12-19-2 \
	check-2015-12-20-1 check-2015-12-20-2 \
	check-2015-12-21-1 check-2015-12-21-2 \
	check-2015-12-22-1 check-2015-12-22-2 \
	check-2015-12-23-1 check-2015-12-23-2 \
	check-2015-12-24-1 check-2015-12-24-2 \
	check-2015-12-25 \
	check-2016-12-01-1 check-2016-12-01-2 \


tidy:
	$(LINDENT) \
		-T FILE \
		-T size_t -T ssize_t \
		-T int8_t -T int16_t -T int32_t -T int64_t \
		-T uint8_t -T uint16_t -T uint32_t -T uint64_t \
		-T eba_s \
		-T ehht_s -T ehht_key_s -T ehht_keys_s \
		-T ehbigint \
                `find 2015 2016 -name '*.c'`


clean:
	rm -fv bin/* `find . -name '*~'`