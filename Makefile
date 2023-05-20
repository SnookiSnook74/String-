CC=gcc
OS=$(shell uname)
FLAGS=-Wall -Werror -Wextra
ifeq ($(OS), Linux)
  FLAGS_LCHECK = -lcheck -pthread -lsubunit -lrt -lm
  OPEN=xdg-open
else
  FLAGS_LCHECK = -lcheck
  OPEN=open
endif
FLAGS_GCOV = -coverage -fprofile-arcs -ftest-coverage
FILE_NAME = s21_string
FILE_NAME_SPRINTF = s21_sprintf
FILE_NAME_SSCANF = s21_sscanf
FILE_TEST = s21_unit_test
EXECUTABLE = final

all: test gcov_report clean_no_all

.PHONY: test
test: s21_string.a
	$(CC) -c $(FILE_TEST).c
	$(CC) $(FILE_TEST).o -o unit_test s21_string.a $(FLAGS) $(FLAGS_LCHECK)

.PHONY: gcov_report
gcov_report: s21_string.a
	$(CC) $(FLAGS) $(FILE_TEST).c $(FLAGS_LCHECK) $(FLAGS_GCOV) $(FILE_NAME).c $(FILE_NAME_SPRINTF).c $(FILE_NAME_SSCANF).c -o info
	./info
	lcov -t "info" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	$(OPEN) report/index.html

.PHONY: s21_string.a
s21_string.a:
	$(CC) -c $(FILE_NAME).c $(FILE_NAME_SPRINTF).c $(FILE_NAME_SSCANF).c
	ar -rcs $@ $(FILE_NAME).o $(FILE_NAME_SPRINTF).o $(FILE_NAME_SSCANF).o
	ranlib $@

.PHONY: docker
docker:
	docker rmi -f string && docker build -t string . && docker run --name unit string
	# docker cp unit:/hello/report .
	docker rm unit

leaks:

.PHONY: cppcheck
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

style:
	clang-format -i *.c
	clang-format -i *.h

.PHONY: git
git: style
	rm -rf report
	git add .
	git commit -m "commit"
	git push

clean_no_all:
	rm -rf *.o *.gcda *.gcno *.info
	rm -f info

clean:
	rm -rf *.o *.gcda *.gcno *.info
	rm -f info
	rm -f *.a
	rm -rf report
	rm -f unit_test
