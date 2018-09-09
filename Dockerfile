FROM debian:stretch

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y curl build-essential mingw-w64 python2.7 cmake git clang ruby2.3 ruby-dev nodejs default-jre libwxgtk3.0-dev libwxgtk-webview3.0-dev libglew-dev libxcursor-dev libxrandr-dev libxinerama-dev cmake libxml2-dev zlib1g-dev libssl-dev openssl libreadline-dev libgdbm-dev openssl uuid-dev libvorbis-dev libvorbisfile3 libopenal-dev
RUN gem install bundler

RUN git clone https://github.com/juj/emsdk.git /emsdk
ENV EMSCRIPTEN_VERSION=sdk-1.38.12-64bit
RUN cd /emsdk && ./emsdk install $EMSCRIPTEN_VERSION && ./emsdk activate $EMSCRIPTEN_VERSION

WORKDIR /blitz3d-ng
ADD ./Gemfile ./Gemfile
ADD ./Gemfile.lock ./Gemfile.lock
RUN bundle install

ADD ./ ./

ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++

ENV blitzpath=/blitz3d-ng/_release

CMD /bin/bash
