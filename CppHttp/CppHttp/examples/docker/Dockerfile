FROM gcc AS development

WORKDIR /server/

RUN apt-get update

RUN apt-get install -y cmake

COPY src/dependencies .

COPY . .

FROM development AS builder

RUN cmake -S . -B build
RUN cmake --build build

RUN mkdir /app
RUN cp -r build/* /app

FROM builder AS production

EXPOSE 80

CMD [ "stdbuf", "-oL", "./build/docker" ]