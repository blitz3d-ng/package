FROM debian:jessie

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update
RUN apt-get install -y curl build-essential libwxgtk3.0-dev libwxgtk-webview3.0-dev

RUN curl -L https://github.com/premake/premake-core/releases/download/v5.0.0-alpha10/premake-5.0.0-alpha10-linux.tar.gz | tar xz && mv ./premake5 /usr/local/bin

WORKDIR /usr/local/opt/blitz3d-ng
ADD ./ ./
