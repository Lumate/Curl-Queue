# James

FROM debian:latest

RUN apt-get update && \
    apt-get install -y build-essential supervisor libcurl4-gnutls-dev libprotobuf-dev libzmq-dev libboost-all-dev && \
    apt-get clean

COPY supervisord.conf /etc/supervisor/conf.d/

ENV APP_ROOT /project/Curl-Queue

RUN mkdir -p $APP_ROOT/build \
    cd $APP_ROOT/build \
    cmake .. \
    make

COPY . $APP_ROOT/

EXPOSE 5569

CMD ["/usr/bin/supervisord"]
