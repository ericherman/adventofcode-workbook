# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2015-2021 Eric Herman <eric@freesa.org>

# $@ : target label
# $< : the first prerequisite after the colon
# $^ : all of the prerequisite files
# $* : wildcard matched part

C89=-std=c89 -Wpedantic
GNU89=-std=gnu89 -Wpedantic
GNU11=-std=gnu11 -Wpedantic
C11=-std=c11 -Wpedantic
#CFLAGS=-ggdb -O0 -Wall -Wextra -Werror -Icommon
#CFLAGS=-ggdb -pg -O2 -Wall -Wextra -Werror -DNDEBUG -Icommon
CFLAGS=-ggdb -O2 -Wall -Wextra -Werror -DNDEBUG -Icommon

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

default: check

bindir:
	mkdir -pv bin

MD5: bindir common/md5/global.h common/md5/md5.h \
		common/md5/md5.c common/md5/mddriver.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/MD5 \
		-DMD=5 \
		-Icommon/md5 \
		common/md5/md5.c common/md5/mddriver.c

check-md5-empty: MD5
	./check-answer.sh 'MD5 ("") = d41d8cd98f00b204e9800998ecf8427e' bin/MD5 -s""

check-md5-a: MD5
	./check-answer.sh 'MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661' bin/MD5 -s"a"

check-md5-abc: MD5
	./check-answer.sh 'MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72' bin/MD5 -s"abc"

check-md5-with-space: MD5
	./check-answer.sh 'MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0' bin/MD5 -s"message digest"

check-md5-a-z: MD5
	./check-answer.sh 'MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b' bin/MD5 -s"abcdefghijklmnopqrstuvwxyz"

check-md5-A-Za-0-9: MD5
	./check-answer.sh 'MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = d174ab98d277d9f5a5611c2c9f419d9f' bin/MD5 -s"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

check-md5-80-digits: MD5
	./check-answer.sh 'MD5 ("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = 57edf4a22be3c955ac49da2e2107b67a' bin/MD5 -s"12345678901234567890123456789012345678901234567890123456789012345678901234567890"

#check-md5-with-space \

check-md5: \
 check-md5-empty \
 check-md5-a \
 check-md5-abc \
 check-md5-a-z \
 check-md5-A-Za-0-9 \
 check-md5-80-digits
	echo "SUCCESS $@"

# 2015-12-01
2015-12-01-1: bindir 2015/12/01/day1part1.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-01-1 2015/12/01/day1part1.c

2015-12-01-2: bindir 2015/12/01/day1part2.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-01-2 2015/12/01/day1part2.c

check-2015-12-01-1: bindir 2015-12-01-1
	./check-answer.sh 74 bin/2015-12-01-1 2015/12/01/input

check-2015-12-01-2: bindir 2015-12-01-2
	./check-answer.sh 1795 bin/2015-12-01-2 2015/12/01/input


# 2015-12-02
2015-12-02-1: bindir 2015/12/02/day2part1.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-02-1 2015/12/02/day2part1.c

2015-12-02-2: bindir 2015/12/02/day2part2.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-02-2 2015/12/02/day2part2.c

check-2015-12-02-1: bindir 2015-12-02-1
	./check-answer.sh 1586300 bin/2015-12-02-1 2015/12/02/input

check-2015-12-02-2: bindir 2015-12-02-2
	./check-answer.sh 3737498 bin/2015-12-02-2 2015/12/02/input


# 2015-12-03
2015-12-03-1: bindir 2015/12/03/day3part1.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-03-1 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		2015/12/03/day3part1.c

2015-12-03-2: bindir 2015/12/03/day3part2.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-03-2 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		2015/12/03/day3part2.c

check-2015-12-03-1: bindir 2015-12-03-1
	./check-answer.sh 2572 bin/2015-12-03-1 2015/12/03/input

check-2015-12-03-2: bindir 2015-12-03-2
	./check-answer.sh 2631 bin/2015-12-03-2 2015/12/03/input


# 2015-12-04
2015-12-04: bindir 2015/12/04/day4.c
	# GNU89 for strnlen
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-04 \
		-Icommon/md5 common/md5/md5.c \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehstr common/ehstr/ehstr.c \
		2015/12/04/day4.c

check-2015-12-04-1: bindir 2015-12-04 check-md5
	./check-answer.sh 346386 bin/2015-12-04 5

check-2015-12-04-2: bindir 2015-12-04 check-md5
	./check-answer.sh 9958218 bin/2015-12-04 6


# 2015-12-05
2015-12-05-1: bindir 2015/12/05/day5part1.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-05-1 2015/12/05/day5part1.c

2015-12-05-2: bindir 2015/12/05/day5part2.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-05-2 2015/12/05/day5part2.c

check-2015-12-05-1: bindir 2015-12-05-1
	./check-answer.sh 238 bin/2015-12-05-1 2015/12/05/input

check-2015-12-05-2: bindir 2015-12-05-2
	./check-answer.sh 69 bin/2015-12-05-2 2015/12/05/input


# 2015-12-06
2015-12-06-1: bindir 2015/12/06/day6part1.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-06-1 2015/12/06/day6part1.c

2015-12-06-2: bindir 2015/12/06/day6part2.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-06-2 2015/12/06/day6part2.c

check-2015-12-06-1: bindir 2015-12-06-1
	./check-answer.sh 377891 bin/2015-12-06-1 2015/12/06/input

check-2015-12-06-2: bindir 2015-12-06-2
	./check-answer.sh 14110788 bin/2015-12-06-2 2015/12/06/input


# 2015-12-07
2015-12-07: bindir 2015/12/07/day7.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-07 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/07/day7.c

check-2015-12-07-1: bindir 2015-12-07
	./check-answer.sh 16076 bin/2015-12-07 2015/12/07/input

check-2015-12-07-2: bindir 2015-12-07
	./check-answer.sh 2797 bin/2015-12-07 2015/12/07/input 16076


# 2015-12-08
2015-12-08: bindir 2015/12/08/day8.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-08 2015/12/08/day8.c

check-2015-12-08-1: bindir 2015-12-08
	./check-answer.sh 1350 bin/2015-12-08 2015/12/08/input

check-2015-12-08-2: bindir 2015-12-08
	./check-answer.sh 2085 bin/2015-12-08 2015/12/08/input 1


# 2015-12-09
2015-12-09: bindir 2015/12/09/day9.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-09 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		-Icommon/permute common/permute/*.c \
		2015/12/09/day9.c

check-2015-12-09-1: bindir 2015-12-09
	./check-answer.sh 251 bin/2015-12-09 2015/12/09/input

check-2015-12-09-2: bindir 2015-12-09
	./check-answer.sh 898 bin/2015-12-09 2015/12/09/input 1


# 2015-12-10
2015-12-10: bindir 2015/12/10/day10.c
	# GNU89 for snprintf
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-10 2015/12/10/day10.c

check-2015-12-10-1: bindir 2015-12-10
	./check-answer.sh 252594 bin/2015-12-10 40 1113222113

check-2015-12-10-2: bindir 2015-12-10
	./check-answer.sh 3579328 bin/2015-12-10 50 1113222113


# 2015-12-11
2015-12-11: bindir 2015/12/11/day11.c
	# GNU89 for snprintf
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-11 2015/12/11/day11.c

check-2015-12-11-1: bindir 2015-12-11
	./check-answer.sh cqjxxyzz bin/2015-12-11 cqjxjnds

check-2015-12-11-2: bindir 2015-12-11
	./check-answer.sh cqkaabcc bin/2015-12-11 cqjxxyzz


# 2015-12-12
2015-12-12-1: bindir 2015/12/12/day12part1.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-12-1 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/deque common/deque/*.c \
		2015/12/12/day12part1.c

2015-12-12-2: bindir 2015/12/12/day12part2.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-12-2 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/deque common/deque/*.c \
		2015/12/12/day12part2.c

check-2015-12-12-1: bindir 2015-12-12-1
	./check-answer.sh 119433 bin/2015-12-12-1 2015/12/12/input

check-2015-12-12-2: bindir 2015-12-12-2
	./check-answer.sh 68466 bin/2015-12-12-2 2015/12/12/input


# 2015-12-13
2015-12-13: bindir 2015/12/13/day13.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-13 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		-Icommon/permute common/permute/*.c \
		2015/12/13/day13.c

check-2015-12-13-1: bindir 2015-12-13
	./check-answer.sh 664 bin/2015-12-13 2015/12/13/input

check-2015-12-13-2: bindir 2015-12-13
	./check-answer.sh 640 bin/2015-12-13 2015/12/13/input 1


# 2015-12-14
2015-12-14: bindir 2015/12/14/day14.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-14 2015/12/14/day14.c

check-2015-12-14-1: bindir 2015-12-14
	./check-answer.sh 2640 bin/2015-12-14

check-2015-12-14-2: bindir 2015-12-14
	./check-answer.sh 1102 bin/2015-12-14 1


# 2015-12-15
2015-12-15: bindir 2015/12/15/day15.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-15 2015/12/15/day15.c

check-2015-12-15-1: bindir 2015-12-15
	./check-answer.sh 13882464 bin/2015-12-15

check-2015-12-15-2: bindir 2015-12-15
	./check-answer.sh 11171160 bin/2015-12-15 500


# 2015-12-16
2015-12-16: bindir 2015/12/16/day16.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-16 2015/12/16/day16.c

check-2015-12-16-1: bindir 2015-12-16
	./check-answer.sh 373 bin/2015-12-16 2015/12/16/input

check-2015-12-16-2: bindir 2015-12-16
	./check-answer.sh 260 bin/2015-12-16 2015/12/16/input 1


# 2015-12-17
2015-12-17: bindir 2015/12/17/day17.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-17 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/17/day17.c

check-2015-12-17-1: bindir 2015-12-17
	./check-answer.sh 1638 bin/2015-12-17 2015/12/17/input

check-2015-12-17-2: bindir 2015-12-17
	./check-answer.sh 17 bin/2015-12-17 2015/12/17/input 1


# 2015-12-18
2015-12-18: bindir 2015/12/18/day18.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-18 2015/12/18/day18.c

check-2015-12-18-1: bindir 2015-12-18
	./check-answer.sh 768 bin/2015-12-18 2015/12/18/input

check-2015-12-18-2: bindir 2015-12-18
	./check-answer.sh 781 bin/2015-12-18 2015/12/18/input 1


# 2015-12-19
2015-12-19: bindir 2015/12/19/day19.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2015-12-19 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/19/day19.c

check-2015-12-19-1: bindir 2015-12-19
	./check-answer.sh 576 bin/2015-12-19 2015/12/19/input

check-2015-12-19-2: bindir 2015-12-19
	./check-answer.sh 207 bin/2015-12-19 2015/12/19/input 1


# 2015-12-20
2015-12-20: bindir 2015/12/20/day20.c
	gcc $(C89) $(CFLAGS) -o bin/2015-12-20 2015/12/20/day20.c

check-2015-12-20-1: bindir 2015-12-20
	./check-answer.sh 776160 bin/2015-12-20 2015/12/20/input

check-2015-12-20-2: bindir 2015-12-20
	./check-answer.sh 786240 bin/2015-12-20 2015/12/20/input 1


# 2015-12-21
2015-12-21: bindir 2015/12/21/day21.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-21 2015/12/21/day21.c

check-2015-12-21-1: bindir 2015-12-21
	./check-answer.sh 91 bin/2015-12-21 2015/12/21/input

check-2015-12-21-2: bindir 2015-12-21
	./check-answer.sh 158 bin/2015-12-21 2015/12/21/input 1


# 2015-12-22
2015-12-22: bindir 2015/12/22/day22.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-22 2015/12/22/day22.c

check-2015-12-22-1: bindir 2015-12-22
	./check-answer.sh 1824 bin/2015-12-22 2015/12/22/input

check-2015-12-22-2: bindir 2015-12-22
	./check-answer.sh 1937 bin/2015-12-22 2015/12/22/input 1


# 2015-12-23
2015-12-23: bindir 2015/12/23/day23.c
	gcc $(GNU89) $(CFLAGS) -o bin/2015-12-23 2015/12/23/day23.c

check-2015-12-23-1: bindir 2015-12-23
	./check-answer.sh 255 bin/2015-12-23 2015/12/23/input

check-2015-12-23-2: bindir 2015-12-23
	./check-answer.sh 334 bin/2015-12-23 2015/12/23/input 1


# 2015-12-24
2015-12-24: bindir 2015/12/24/day24.c
	gcc $(C89) $(CFLAGS) \
		-o bin/2015-12-24 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/eba common/eba/*.c \
		-Icommon/ehbigint common/ehbigint/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/24/day24.c

check-2015-12-24-1: bindir 2015-12-24
	./check-answer.sh 11846773891 bin/2015-12-24 2015/12/24/input

check-2015-12-24-2: bindir 2015-12-24
	./check-answer.sh 80393059 bin/2015-12-24 2015/12/24/input 1


# 2015-12-25
2015-12-25: bindir 2015/12/25/day25.c
	gcc $(C89) $(CFLAGS) \
		-o bin/2015-12-25 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/eba common/eba/*.c \
		-Icommon/ehbigint common/ehbigint/*.c \
		-Icommon/ehstr common/ehstr/*.c \
		2015/12/25/day25.c

check-2015-12-25: bindir 2015-12-25
	./check-answer.sh 8997277 bin/2015-12-25 3010 3019



# 2016-12-01
2016-12-01-1: bindir 2016/12/01/day1part1.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2016-12-01-1 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		2016/12/01/day1part1.c

2016-12-01-2: bindir 2016/12/01/day1part2.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2016-12-01-2 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		2016/12/01/day1part2.c

check-2016-12-01-1: bindir 2016-12-01-1
	./check-answer.sh 209 bin/2016-12-01-1 2016/12/01/input

check-2016-12-01-2: bindir 2016-12-01-2
	./check-answer.sh 136 bin/2016-12-01-2 2016/12/01/input


INTCODE_2019_SRC=2019/intcode-computer.c
CFLAGS_INTCODE_2019=-I ./2019 -Wno-long-long

# 2019-12-01
2019-12-01-1: bindir 2019/12/01/01-1.c
	gcc $(C89) $(CFLAGS) -o bin/2019-12-01-1 2019/12/01/01-1.c

2019-12-01-2: bindir 2019/12/01/01-2.c
	gcc $(C89) $(CFLAGS) -o bin/2019-12-01-2 2019/12/01/01-2.c

check-2019-12-01-1: bindir 2019-12-01-1
	./check-answer.sh 3465154 bin/2019-12-01-1 2019/12/01/input

check-2019-12-01-2: bindir 2019-12-01-2
	./check-answer.sh 5194864 bin/2019-12-01-2 2019/12/01/input

# 2019-12-02
2019-12-02-1: bindir 2019/12/01/01-1.c $(INTCODE_2019_SRC)
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) \
		-o bin/2019-12-02-1 2019/12/02/02-1.c

2019-12-02-2: bindir 2019/12/01/01-2.c $(INTCODE_2019_SRC)
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) \
		-o bin/2019-12-02-2 2019/12/02/02-2.c


check-2019-12-02-1: bindir 2019-12-02-1
	./check-answer.sh 4138687 bin/2019-12-02-1 2019/12/02/input

check-2019-12-02-2: bindir 2019-12-02-2
	./check-answer.sh 6635 bin/2019-12-02-2 2019/12/02/input

# 2019-12-03
2019-12-03-1: bindir 2019/12/03/03-1.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2019-12-03-1 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		2019/12/03/03-1.c

check-2019-12-03-1: bindir 2019-12-03-1
	./check-answer.sh 5357 bin/2019-12-03-1 2019/12/03/input

2019-12-03-2: bindir 2019/12/03/03-2.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2019-12-03-2 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		2019/12/03/03-2.c

check-2019-12-03-2: bindir 2019-12-03-2
	./check-answer.sh 101956 bin/2019-12-03-2 2019/12/03/input

# 2019-12-04
2019-12-04-1: bindir 2019/12/04/04-1.c
	gcc $(C89) $(CFLAGS) -o bin/2019-12-04-1 2019/12/04/04-1.c

2019-12-04-2: bindir 2019/12/04/04-2.c
	gcc $(C89) $(CFLAGS) -o bin/2019-12-04-2 2019/12/04/04-2.c

check-2019-12-04-1: bindir 2019-12-04-1
	./check-answer.sh 454 bin/2019-12-04-1

check-2019-12-04-2: bindir 2019-12-04-2
	./check-answer.sh 288 bin/2019-12-04-2

# 2019-12-05
2019-12-05-1: bindir 2019/12/05/05-1.c $(INTCODE_2019_SRC)
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) \
		-o bin/2019-12-05-1 2019/12/05/05-1.c

check-2019-12-05-1: bindir 2019-12-05-1
	./check-answer.sh 6069343 bin/2019-12-05-1 2019/12/05/input

2019-12-05-2: bindir 2019/12/05/05-2.c $(INTCODE_2019_SRC)
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) \
		-o bin/2019-12-05-2 2019/12/05/05-2.c

check-2019-12-05-2: bindir 2019-12-05-2
	./check-answer.sh 3188550 bin/2019-12-05-2 2019/12/05/input

# 2019-12-06
2019-12-06-1: bindir 2019/12/06/06-1.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2019-12-06-1 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
                -Icommon/deque common/deque/*.c \
		2019/12/06/06-1.c

check-2019-12-06-1: bindir 2019-12-06-1
	./check-answer.sh 270768 bin/2019-12-06-1 2019/12/06/input

2019-12-06-2: bindir 2019/12/06/06-2.c
	gcc $(GNU89) $(CFLAGS) \
		-o bin/2019-12-06-2 \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/ehht common/ehht/*.c \
		2019/12/06/06-2.c

check-2019-12-06-2: bindir 2019-12-06-2
	./check-answer.sh 451 bin/2019-12-06-2 2019/12/06/input

# 2019-12-07
2019-12-07-1: bindir 2019/12/07/07-1.c $(INTCODE_2019_SRC)
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) \
		-Icommon/permute common/permute/*.c \
		-o bin/2019-12-07-1 2019/12/07/07-1.c

check-2019-12-07-1: bindir 2019-12-07-1
	./check-answer.sh 70597 bin/2019-12-07-1 2019/12/07/input

2019-12-07-2: bindir 2019/12/07/07-2.c $(INTCODE_2019_SRC)
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) \
		-Icommon/permute common/permute/*.c \
		-o bin/2019-12-07-2 2019/12/07/07-2.c \
		-pthread

check-2019-12-07-2: bindir 2019-12-07-2
	./check-answer.sh 30872528 bin/2019-12-07-2 2019/12/07/input


# 2019-12-08
2019-12-08-1: bindir 2019/12/08/08-1.c
	gcc $(C89) $(CFLAGS) -o bin/2019-12-08-1 2019/12/08/08-1.c

2019-12-08-2: bindir 2019/12/08/08-2.c
	gcc $(C89) $(CFLAGS) -o bin/2019-12-08-2 2019/12/08/08-2.c

check-2019-12-08-1: bindir 2019-12-08-1
	./check-answer.sh 828 bin/2019-12-08-1 2019/12/08/input

check-2019-12-08-2: bindir 2019-12-08-2
	./check-answer.sh \
		111101000011100001101111000010100001001000010100000010010000111000001011100010001000010010000101000010000100001001010010100001111011110111000110010000 \
		bin/2019-12-08-2 2019/12/08/input

# 2019-12-09
2019-12-09-1: bindir 2019/12/09/09-1.c $(INTCODE_2019_SRC)
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) \
		-o bin/2019-12-09-1 2019/12/09/09-1.c

check-2019-12-09-1: bindir 2019-12-09-1
	./check-answer.sh '2682107844' bin/2019-12-09-1 2019/12/09/input

2019-12-09-2: bindir 2019/12/09/09-2.c $(INTCODE_2019_SRC)
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) \
		-o bin/2019-12-09-2 2019/12/09/09-2.c

check-2019-12-09-2: bindir 2019-12-09-2
	./check-answer.sh '34738' bin/2019-12-09-2 2019/12/09/input


# 2021-12-01
2021-12-01-1: bindir 2021/12/01/01-1.c
	gcc $(C11) $(CFLAGS) -o bin/2021-12-01-1 2021/12/01/01-1.c

2021-12-01-2: bindir 2021/12/01/01-2.c
	gcc $(C11) $(CFLAGS) -o bin/2021-12-01-2 2021/12/01/01-2.c

check-2021-12-01-1: bindir 2021-12-01-1
	./check-answer.sh 1616 bin/2021-12-01-1 2021/12/01/input

check-2021-12-01-2: bindir 2021-12-01-2
	./check-answer.sh 1645 bin/2021-12-01-2 2021/12/01/input


# 2021-12-02
2021-12-02-1: bindir 2021/12/02/02-1.c
	gcc $(C11) $(CFLAGS) -o bin/2021-12-02-1 2021/12/02/02-1.c

2021-12-02-2: bindir 2021/12/02/02-2.c
	gcc $(C11) $(CFLAGS) -o bin/2021-12-02-2 2021/12/02/02-2.c

check-2021-12-02-1: bindir 2021-12-02-1
	./check-answer.sh 1648020 bin/2021-12-02-1 2021/12/02/input

check-2021-12-02-2: bindir 2021-12-02-2
	./check-answer.sh 1759818555 bin/2021-12-02-2 2021/12/02/input


# 2021-12-03
2021-12-03-1: bindir 2021/12/03/03-1.c
	gcc $(C11) $(CFLAGS) \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/deque common/deque/*.c \
		-o bin/2021-12-03-1 2021/12/03/03-1.c

2021-12-03-2: bindir 2021/12/03/03-2.c
	gcc $(C11) $(CFLAGS) \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/deque common/deque/*.c \
		-o bin/2021-12-03-2 2021/12/03/03-2.c

check-2021-12-03-1: bindir 2021-12-03-1
	./check-answer.sh 2967914 bin/2021-12-03-1 2021/12/03/input

check-2021-12-03-2: bindir 2021-12-03-2
	./check-answer.sh 7041258 bin/2021-12-03-2 2021/12/03/input


# 2021-12-04
2021-12-04-1: bindir 2021/12/04/04-1.c
	gcc $(GNU11) $(CFLAGS) \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/deque common/deque/*.c \
		-Icommon/eherr common/eherr/eherr.c \
		-Icommon/ehstr common/ehstr/ehstr.c \
		-o bin/2021-12-04-1 2021/12/04/04-1.c

2021-12-04-2: bindir 2021/12/04/04-2.c
	gcc $(GNU11) $(CFLAGS) \
		-Icommon/echeck common/echeck/eembed.c \
		-Icommon/deque common/deque/*.c \
		-Icommon/eherr common/eherr/eherr.c \
		-Icommon/ehstr common/ehstr/ehstr.c \
		-o bin/2021-12-04-2 2021/12/04/04-2.c

check-2021-12-04-1: bindir 2021-12-04-1
	./check-answer.sh 10680 bin/2021-12-04-1 2021/12/04/input

check-2021-12-04-2: bindir 2021-12-04-2
	./check-answer.sh 31892 bin/2021-12-04-2 2021/12/04/input


# 2021-12-05
2021-12-05: bindir 2021/12/05/05.c
	gcc $(GNU11) $(CFLAGS) \
		-Icommon/eherr common/eherr/eherr.c \
		-o bin/2021-12-05 2021/12/05/05.c

check-2021-12-05-1: bindir 2021-12-05
	./check-answer.sh 4421 bin/2021-12-05 2021/12/05/input

check-2021-12-05-2: bindir 2021-12-05
	./check-answer.sh 18674 bin/2021-12-05 2021/12/05/input 1

# 2021-12-06
2021-12-06: bindir 2021/12/06/06.c
	gcc $(GNU11) $(CFLAGS) \
		-Icommon/eherr common/eherr/eherr.c \
		-o bin/2021-12-06 2021/12/06/06.c

check-2021-12-06-1: bindir 2021-12-06
	./check-answer.sh 349549 bin/2021-12-06 2021/12/06/input

check-2021-12-06-2: bindir 2021-12-06
	./check-answer.sh 1589590444365 bin/2021-12-06 2021/12/06/input 256


check-2015: \
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
	check-2015-12-25
	@echo SUCCESS $@


check-2016: \
	check-2016-12-01-1 check-2016-12-01-2 \

	@echo SUCCESS $@


check-2019: \
	check-2019-12-01-1 check-2019-12-01-2 \
	check-2019-12-02-1 check-2019-12-02-2 \
	check-2019-12-03-1 check-2019-12-03-2 \
	check-2019-12-04-1 check-2019-12-04-2 \
	check-2019-12-05-1 check-2019-12-05-2 \
	check-2019-12-06-1 check-2019-12-06-2 \
	check-2019-12-07-1 check-2019-12-07-2 \
	check-2019-12-08-1 check-2019-12-08-2 \
	check-2019-12-09-1 check-2019-12-09-2 \

	@echo SUCCESS $@

check-2021: \
	check-2021-12-01-1 check-2021-12-01-2 \
	check-2021-12-02-1 check-2021-12-02-2 \
	check-2021-12-03-1 check-2021-12-03-2 \
	check-2021-12-04-1 check-2021-12-04-2 \
	check-2021-12-05-1 check-2021-12-05-2 \
	check-2021-12-06-1 check-2021-12-06-2 \

	@echo SUCCESS $@


check:  \
	check-2021 \
	check-2019 \
	check-2016 \
	check-2015
	@echo SUCCESS $@

tidy:
	$(LINDENT) \
		-T FILE \
		-T size_t -T ssize_t \
		-T int8_t -T int16_t -T int32_t -T int64_t \
		-T uint8_t -T uint16_t -T uint32_t -T uint64_t \
		-T deque -T deque_s \
		-T eba \
		-T ehht -T ehht_key -T ehht_keys \
		-T ehbigint \
		-T intcode_cpu_s -T intcode_op \
                `find 2015 2016 2019 2021 -name '*.c'`


clean:
	rm -fv bin/* `find . -name '*~'`
