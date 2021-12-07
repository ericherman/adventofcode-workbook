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

CFLAGS_I_HEADER_DIRS=-Icommon \
 -Icommon/deque \
 -Icommon/eba \
 -Icommon/echeck \
 -Icommon/ehbigint \
 -Icommon/eherr \
 -Icommon/ehht \
 -Icommon/ehstr \
 -Icommon/md5 \
 -Icommon/permute

#CFLAGS=-ggdb -O0 -Wall -Wextra -Werror $(CFLAGS_I_HEADER_DIRS)
#CFLAGS=-ggdb -pg -O2 -Wall -Wextra -Werror -DNDEBUG $(CFLAGS_I_HEADER_DIRS)
CFLAGS=-ggdb -O2 -Wall -Wextra -Werror -DNDEBUG $(CFLAGS_I_HEADER_DIRS)

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

default: check

bindir:
	mkdir -pv bin

common/deque/deque.c: common/deque/deque.h

common/eba/eba.c: common/eba/eba.h

common/echeck/echeck.c: common/echeck/echeck.h

common/echeck/eembed.c: common/echeck/eembed.h

common/ehbigint/ehbigint.c: common/ehbigint/ehbigint.h

common/eherr/eherr.c: common/eherr/eherr.h

common/ehht/ehht.c: common/ehht/ehht.h

common/ehstr/ehstr.c: common/ehstr/ehstr.h

common/md5/md5.c: common/md5/global.h common/md5/md5.h

common/permute/permute.c: common/permute/permute.h

bin/MD5: common/md5/md5.c common/md5/mddriver.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ -DMD=5 $^

check-md5-empty: bin/MD5
	./check-answer.sh \
		'MD5 ("") = d41d8cd98f00b204e9800998ecf8427e' \
		$<  -s""

check-md5-a: bin/MD5
	./check-answer.sh \
		'MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661' \
		$<  -s"a"

check-md5-abc: bin/MD5
	./check-answer.sh \
		'MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72' \
		$<  -s"abc"

check-md5-with-space: bin/MD5
	./check-answer.sh \
		'MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0' \
		$<  -s"message digest"

check-md5-a-z: bin/MD5
	./check-answer.sh \
		'MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b' \
		$<  -s"abcdefghijklmnopqrstuvwxyz"

check-md5-A-Za-0-9: bin/MD5
	./check-answer.sh \
		'MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = d174ab98d277d9f5a5611c2c9f419d9f' \
		$<  -s"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

check-md5-80-digits: bin/MD5
	./check-answer.sh \
		'MD5 ("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = 57edf4a22be3c955ac49da2e2107b67a' \
		$<  -s"12345678901234567890123456789012345678901234567890123456789012345678901234567890"

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
bin/2015-12-01-1: 2015/12/01/day1part1.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

bin/2015-12-01-2: 2015/12/01/day1part2.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-01-1: bin/2015-12-01-1
	./check-answer.sh 74 $< 2015/12/01/input

check-2015-12-01-2: bin/2015-12-01-2
	./check-answer.sh 1795 $< 2015/12/01/input


# 2015-12-02
bin/2015-12-02-1: 2015/12/02/day2part1.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

bin/2015-12-02-2: 2015/12/02/day2part2.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-02-1: bin/2015-12-02-1
	./check-answer.sh 1586300 $< 2015/12/02/input

check-2015-12-02-2: bin/2015-12-02-2
	./check-answer.sh 3737498 $< 2015/12/02/input


# 2015-12-03
bin/2015-12-03-1: common/echeck/eembed.c \
		common/ehht/ehht.c \
		2015/12/03/day3part1.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

bin/2015-12-03-2: common/echeck/eembed.c \
		common/ehht/ehht.c \
		2015/12/03/day3part2.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-03-1: bin/2015-12-03-1
	./check-answer.sh 2572 $< 2015/12/03/input

check-2015-12-03-2: bin/2015-12-03-2
	./check-answer.sh 2631 $< 2015/12/03/input


# 2015-12-04
bin/2015-12-04: common/md5/md5.c \
		common/echeck/eembed.c \
		common/ehstr/ehstr.c \
		2015/12/04/day4.c
	mkdir -pv bin
	# GNU89 for strnlen
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-04-1: bin/2015-12-04 check-md5
	./check-answer.sh 346386 $< 5

check-2015-12-04-2: bin/2015-12-04 check-md5
	./check-answer.sh 9958218 $< 6


# 2015-12-05
bin/2015-12-05-1: 2015/12/05/day5part1.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

bin/2015-12-05-2: 2015/12/05/day5part2.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-05-1: bin/2015-12-05-1
	./check-answer.sh 238 $< 2015/12/05/input

check-2015-12-05-2: bin/2015-12-05-2
	./check-answer.sh 69 $< 2015/12/05/input


# 2015-12-06
bin/2015-12-06-1: 2015/12/06/day6part1.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

bin/2015-12-06-2: 2015/12/06/day6part2.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-06-1: bin/2015-12-06-1
	./check-answer.sh 377891 $< 2015/12/06/input

check-2015-12-06-2: bin/2015-12-06-2
	./check-answer.sh 14110788 $< 2015/12/06/input


# 2015-12-07
bin/2015-12-07: common/echeck/eembed.c \
		common/ehht/ehht.c \
		common/ehstr/ehstr.c \
		2015/12/07/day7.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-07-1: bin/2015-12-07
	./check-answer.sh 16076 $< 2015/12/07/input

check-2015-12-07-2: bin/2015-12-07
	./check-answer.sh 2797 $< 2015/12/07/input 16076


# 2015-12-08
bin/2015-12-08: 2015/12/08/day8.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-08-1: bin/2015-12-08
	./check-answer.sh 1350 $< 2015/12/08/input

check-2015-12-08-2: bin/2015-12-08
	./check-answer.sh 2085 $< 2015/12/08/input 1


# 2015-12-09
bin/2015-12-09: common/echeck/eembed.c \
		common/ehht/ehht.c \
		common/ehstr/ehstr.c \
		common/permute/permute.c \
		2015/12/09/day9.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-09-1: bin/2015-12-09
	./check-answer.sh 251 $< 2015/12/09/input

check-2015-12-09-2: bin/2015-12-09
	./check-answer.sh 898 $< 2015/12/09/input 1


# 2015-12-10
bin/2015-12-10: 2015/12/10/day10.c
	# GNU89 for snprintf
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-10-1: bin/2015-12-10
	./check-answer.sh 252594 $< 40 1113222113

check-2015-12-10-2: bin/2015-12-10
	./check-answer.sh 3579328 $< 50 1113222113


# 2015-12-11
bin/2015-12-11: 2015/12/11/day11.c
	# GNU89 for snprintf
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-11-1: bin/2015-12-11
	./check-answer.sh cqjxxyzz $< cqjxjnds

check-2015-12-11-2: bin/2015-12-11
	./check-answer.sh cqkaabcc $< cqjxxyzz


# 2015-12-12
bin/2015-12-12-1: common/echeck/eembed.c \
		common/deque/deque.c \
		2015/12/12/day12part1.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

bin/2015-12-12-2: common/echeck/eembed.c \
		common/deque/deque.c \
		2015/12/12/day12part2.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-12-1: bin/2015-12-12-1
	./check-answer.sh 119433 $< 2015/12/12/input

check-2015-12-12-2: bin/2015-12-12-2
	./check-answer.sh 68466 $< 2015/12/12/input


# 2015-12-13
bin/2015-12-13: common/echeck/eembed.c \
		common/ehht/ehht.c \
		common/ehstr/ehstr.c \
		common/permute/permute.c \
		2015/12/13/day13.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-13-1: bin/2015-12-13
	./check-answer.sh 664 $< 2015/12/13/input

check-2015-12-13-2: bin/2015-12-13
	./check-answer.sh 640 $< 2015/12/13/input 1


# 2015-12-14
bin/2015-12-14: 2015/12/14/day14.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-14-1: bin/2015-12-14
	./check-answer.sh 2640 $<

check-2015-12-14-2: bin/2015-12-14
	./check-answer.sh 1102 $< 1


# 2015-12-15
bin/2015-12-15: 2015/12/15/day15.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-15-1: bin/2015-12-15
	./check-answer.sh 13882464 $<

check-2015-12-15-2: bin/2015-12-15
	./check-answer.sh 11171160 $< 500


# 2015-12-16
bin/2015-12-16: 2015/12/16/day16.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-16-1: bin/2015-12-16
	./check-answer.sh 373 $< 2015/12/16/input

check-2015-12-16-2: bin/2015-12-16
	./check-answer.sh 260 $< 2015/12/16/input 1


# 2015-12-17
bin/2015-12-17: common/echeck/eembed.c \
		common/ehht/ehht.c \
		common/ehstr/ehstr.c \
		2015/12/17/day17.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-17-1: bin/2015-12-17
	./check-answer.sh 1638 $< 2015/12/17/input

check-2015-12-17-2: bin/2015-12-17
	./check-answer.sh 17 $< 2015/12/17/input 1


# 2015-12-18
bin/2015-12-18: 2015/12/18/day18.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-18-1: bin/2015-12-18
	./check-answer.sh 768 $< 2015/12/18/input

check-2015-12-18-2: bin/2015-12-18
	./check-answer.sh 781 $< 2015/12/18/input 1


# 2015-12-19
bin/2015-12-19: common/echeck/eembed.c \
		common/ehht/ehht.c \
		common/ehstr/ehstr.c \
		2015/12/19/day19.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-19-1: bin/2015-12-19
	./check-answer.sh 576 $< 2015/12/19/input

check-2015-12-19-2: bin/2015-12-19
	./check-answer.sh 207 $< 2015/12/19/input 1


# 2015-12-20
bin/2015-12-20: 2015/12/20/day20.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-20-1: bin/2015-12-20
	./check-answer.sh 776160 $< 2015/12/20/input

check-2015-12-20-2: bin/2015-12-20
	./check-answer.sh 786240 $< 2015/12/20/input 1


# 2015-12-21
bin/2015-12-21: 2015/12/21/day21.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-21-1: bin/2015-12-21
	./check-answer.sh 91 $< 2015/12/21/input

check-2015-12-21-2: bin/2015-12-21
	./check-answer.sh 158 $< 2015/12/21/input 1


# 2015-12-22
bin/2015-12-22: 2015/12/22/day22.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-22-1: bin/2015-12-22
	./check-answer.sh 1824 $< 2015/12/22/input

check-2015-12-22-2: bin/2015-12-22
	./check-answer.sh 1937 $< 2015/12/22/input 1


# 2015-12-23
bin/2015-12-23: 2015/12/23/day23.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2015-12-23-1: bin/2015-12-23
	./check-answer.sh 255 $< 2015/12/23/input

check-2015-12-23-2: bin/2015-12-23
	./check-answer.sh 334 $< 2015/12/23/input 1


# 2015-12-24
bin/2015-12-24: common/echeck/eembed.c \
		common/eba/eba.c \
		common/ehbigint/ehbigint.c \
		common/ehstr/ehstr.c \
		2015/12/24/day24.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-24-1: bin/2015-12-24
	./check-answer.sh 11846773891 $< 2015/12/24/input

check-2015-12-24-2: bin/2015-12-24
	./check-answer.sh 80393059 $< 2015/12/24/input 1


# 2015-12-25
bin/2015-12-25: common/echeck/eembed.c \
		common/eba/eba.c \
		common/ehbigint/ehbigint.c \
		common/ehstr/ehstr.c \
		2015/12/25/day25.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2015-12-25: bin/2015-12-25
	./check-answer.sh 8997277 $< 3010 3019



# 2016-12-01
bin/2016-12-01-1: common/echeck/eembed.c \
		common/ehht/ehht.c \
		2016/12/01/day1part1.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

bin/2016-12-01-2: common/echeck/eembed.c \
		common/ehht/ehht.c \
		2016/12/01/day1part2.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2016-12-01-1: bin/2016-12-01-1
	./check-answer.sh 209 $< 2016/12/01/input

check-2016-12-01-2: bin/2016-12-01-2
	./check-answer.sh 136 $< 2016/12/01/input


2019/intcode-computer.c: 2019/intcode-computer.h

CFLAGS_INTCODE_2019=-I ./2019

# 2019-12-01
bin/2019-12-01-1: 2019/12/01/01-1.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

bin/2019-12-01-2: 2019/12/01/01-2.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2019-12-01-1: bin/2019-12-01-1
	./check-answer.sh 3465154 $< 2019/12/01/input

check-2019-12-01-2: bin/2019-12-01-2
	./check-answer.sh 5194864 $< 2019/12/01/input

# 2019-12-02
bin/2019-12-02-1: 2019/12/02/02-1.c 2019/intcode-computer.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) -o $@ $^

bin/2019-12-02-2: 2019/12/02/02-2.c 2019/intcode-computer.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) -o $@ $^


check-2019-12-02-1: bin/2019-12-02-1
	./check-answer.sh 4138687 $< 2019/12/02/input

check-2019-12-02-2: bin/2019-12-02-2
	./check-answer.sh 6635 $< 2019/12/02/input

# 2019-12-03
bin/2019-12-03-1: common/echeck/eembed.c \
		common/ehht/ehht.c \
		2019/12/03/03-1.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2019-12-03-1: bin/2019-12-03-1
	./check-answer.sh 5357 $< 2019/12/03/input

bin/2019-12-03-2: common/echeck/eembed.c \
		common/ehht/ehht.c \
		2019/12/03/03-2.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2019-12-03-2: bin/2019-12-03-2
	./check-answer.sh 101956 $< 2019/12/03/input

# 2019-12-04
bin/2019-12-04-1: 2019/12/04/04-1.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

bin/2019-12-04-2: 2019/12/04/04-2.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2019-12-04-1: bin/2019-12-04-1
	./check-answer.sh 454 $<

check-2019-12-04-2: bin/2019-12-04-2
	./check-answer.sh 288 $<

# 2019-12-05
bin/2019-12-05-1: 2019/12/05/05-1.c 2019/intcode-computer.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) -o $@ $^

check-2019-12-05-1: bin/2019-12-05-1
	./check-answer.sh 6069343 $< 2019/12/05/input

bin/2019-12-05-2: 2019/12/05/05-2.c 2019/intcode-computer.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) -o $@ $^

check-2019-12-05-2: bin/2019-12-05-2
	./check-answer.sh 3188550 $< 2019/12/05/input

# 2019-12-06
bin/2019-12-06-1: common/echeck/eembed.c \
		common/ehht/ehht.c \
                common/deque/deque.c \
		2019/12/06/06-1.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2019-12-06-1: bin/2019-12-06-1
	./check-answer.sh 270768 $< 2019/12/06/input

bin/2019-12-06-2: common/echeck/eembed.c \
		common/ehht/ehht.c \
		2019/12/06/06-2.c
	mkdir -pv bin
	gcc $(GNU89) $(CFLAGS) -o $@ $^

check-2019-12-06-2: bin/2019-12-06-2
	./check-answer.sh 451 $< 2019/12/06/input

# 2019-12-07
bin/2019-12-07-1: common/permute/permute.c \
		2019/intcode-computer.c \
		2019/12/07/07-1.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) -o $@ $^

check-2019-12-07-1: bin/2019-12-07-1
	./check-answer.sh 70597 $< 2019/12/07/input

bin/2019-12-07-2: 2019/intcode-computer.c \
		common/permute/permute.c \
		2019/12/07/07-2.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) -o $@ $^ -pthread

check-2019-12-07-2: bin/2019-12-07-2
	./check-answer.sh 30872528 $< 2019/12/07/input


# 2019-12-08
bin/2019-12-08-1: 2019/12/08/08-1.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

bin/2019-12-08-2: 2019/12/08/08-2.c
	mkdir -pv bin
	gcc $(C89) $(CFLAGS) -o $@ $^

check-2019-12-08-1: bin/2019-12-08-1
	./check-answer.sh 828 $< 2019/12/08/input

check-2019-12-08-2: bin/2019-12-08-2
	./check-answer.sh \
		111101000011100001101111000010100001001000010100000010010000111000001011100010001000010010000101000010000100001001010010100001111011110111000110010000 \
		$< 2019/12/08/input

# 2019-12-09
bin/2019-12-09-1: 2019/12/09/09-1.c 2019/intcode-computer.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) -o $@ $^

check-2019-12-09-1: bin/2019-12-09-1
	./check-answer.sh '2682107844' $< 2019/12/09/input

bin/2019-12-09-2: 2019/12/09/09-2.c 2019/intcode-computer.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) $(CFLAGS_INTCODE_2019) -o $@ $^

check-2019-12-09-2: bin/2019-12-09-2
	./check-answer.sh '34738' $< 2019/12/09/input


# 2021-12-01
bin/2021-12-01-1: common/eherr/eherr.c \
		2021/12/01/01-1.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

bin/2021-12-01-2: common/eherr/eherr.c \
		2021/12/01/01-2.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

check-2021-12-01-1: bin/2021-12-01-1
	./check-answer.sh 1616 $< 2021/12/01/input

check-2021-12-01-2: bin/2021-12-01-2
	./check-answer.sh 1645 $< 2021/12/01/input


# 2021-12-02
bin/2021-12-02-1: common/eherr/eherr.c \
		2021/12/02/02-1.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

bin/2021-12-02-2: common/eherr/eherr.c \
		2021/12/02/02-2.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

check-2021-12-02-1: bin/2021-12-02-1
	./check-answer.sh 1648020 $< 2021/12/02/input

check-2021-12-02-2: bin/2021-12-02-2
	./check-answer.sh 1759818555 $< 2021/12/02/input


# 2021-12-03
bin/2021-12-03-1: common/eherr/eherr.c \
		common/echeck/eembed.c \
		common/deque/deque.c \
		2021/12/03/03-1.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

bin/2021-12-03-2: common/eherr/eherr.c \
		common/echeck/eembed.c \
		common/deque/deque.c \
		2021/12/03/03-2.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

check-2021-12-03-1: bin/2021-12-03-1
	./check-answer.sh 2967914 $< 2021/12/03/input

check-2021-12-03-2: bin/2021-12-03-2
	./check-answer.sh 7041258 $< 2021/12/03/input


# 2021-12-04
bin/2021-12-04-1: common/echeck/eembed.c \
		common/deque/deque.c \
		common/eherr/eherr.c \
		common/ehstr/ehstr.c \
		2021/12/04/04-1.c
	mkdir -pv bin
	# GNU11 strnlen is POSIX, not C11
	gcc $(GNU11) $(CFLAGS) -o $@ $^

bin/2021-12-04-2: common/echeck/eembed.c \
		common/deque/deque.c \
		common/eherr/eherr.c \
		common/ehstr/ehstr.c \
		2021/12/04/04-2.c
	mkdir -pv bin
	gcc $(GNU11) $(CFLAGS) -o $@ $^

check-2021-12-04-1: bin/2021-12-04-1
	./check-answer.sh 10680 $< 2021/12/04/input

check-2021-12-04-2: bin/2021-12-04-2
	./check-answer.sh 31892 $< 2021/12/04/input


# 2021-12-05
bin/2021-12-05: common/eherr/eherr.c \
		2021/12/05/05.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

check-2021-12-05-1: bin/2021-12-05
	./check-answer.sh 4421 $< 2021/12/05/input

check-2021-12-05-2: bin/2021-12-05
	./check-answer.sh 18674 $< 2021/12/05/input 1

# 2021-12-06
bin/2021-12-06: common/eherr/eherr.c \
		2021/12/06/06.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

check-2021-12-06-1: bin/2021-12-06
	./check-answer.sh 349549  $< 2021/12/06/input

check-2021-12-06-2: bin/2021-12-06
	./check-answer.sh 1589590444365 $< 2021/12/06/input 256

# 2021-12-07
bin/2021-12-07: common/eherr/eherr.c \
		2021/12/07/07.c
	mkdir -pv bin
	gcc $(C11) $(CFLAGS) -o $@ $^

check-2021-12-07-1: bin/2021-12-07
	./check-answer.sh 347449 $< 2021/12/07/input

check-2021-12-07-2: bin/2021-12-07
	./check-answer.sh -1 $< 2021/12/07/input 1

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
	check-2021-12-07-1 check-2021-12-07-2 \

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
