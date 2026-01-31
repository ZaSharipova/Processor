FROM ubuntu:noble

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow


RUN apt update && apt install -y \
    g++ \
    make \
    valgrind \
    graphviz \
    libncurses-dev


WORKDIR /langroot

COPY . .

RUN make clean && make proc

CMD ["./proc.out", "--log", "logfile.txt", "--in", "output.txt", "--out", "stdout"]