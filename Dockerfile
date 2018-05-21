FROM debian:stretch

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y curl build-essential clang ruby2.3 ruby-dev libwxgtk3.0-dev libwxgtk-webview3.0-dev libglew-dev libxcursor-dev libxrandr-dev libxinerama-dev cmake libxml2-dev zlib1g-dev libssl-dev openssl libreadline-dev libgdbm-dev openssl uuid-dev
RUN gem install bundler

WORKDIR /usr/local/opt/blitz3d-ng
ADD ./Gemfile ./Gemfile
ADD ./Gemfile.lock ./Gemfile.lock
RUN bundle install

ADD ./ ./

ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
