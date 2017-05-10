FROM debian:jessie

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update
RUN apt-get install -y curl build-essential libwxgtk3.0-dev libwxgtk-webview3.0-dev libglew-dev libxcursor-dev libxrandr-dev libxinerama-dev cmake libxml2-dev zlib1g-dev libssl-dev libreadline-dev libgdbm-dev openssl

RUN curl -L https://github.com/premake/premake-core/releases/download/v5.0.0-alpha11/premake-5.0.0-alpha11-linux.tar.gz | tar xz && mv ./premake5 /usr/local/bin

WORKDIR /usr/local/src
RUN curl -L https://cache.ruby-lang.org/pub/ruby/2.3/ruby-2.3.0.tar.gz | tar xz
RUN cd ruby-2.3.0 && ./configure && make && make install
RUN gem install bundler

WORKDIR /usr/local/opt/blitz3d-ng
ADD ./Gemfile ./Gemfile
ADD ./Gemfile.lock ./Gemfile.lock
RUN bundle install

ADD ./ ./

CMD /bin/bash -c 'bin/blitz3d config && premake5 gmake && make config=${ENV-debug}_linux'
