FROM ubuntu:latest

RUN apt-get update
RUN apt-get install -y g++
RUN apt-get install -y build-essential
RUN apt-get install -y iputils-ping

WORKDIR /workspace

COPY . .

CMD ["bash"]