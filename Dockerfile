FROM unit_test_gcov

COPY . /hello

WORKDIR /hello

RUN make s21_string.a
RUN make test

CMD [ "./unit_test" ]