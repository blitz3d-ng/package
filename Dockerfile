FROM debian:buster

ENV DEBIAN_FRONTEND noninteractive

# basic deps
RUN apt-get update && apt-get install -y \
  build-essential clang git curl wget unzip \
  ruby2.5 ruby-dev libreadline-dev \
  python2.7 nodejs default-jre \
  mingw-w64 \
  libwxgtk3.0-dev libwxgtk-webview3.0-gtk3-dev libglew-dev libxcursor-dev \
  libxrandr-dev libxinerama-dev libxml2-dev zlib1g-dev libssl-dev openssl \
  libgdbm-dev uuid-dev  \
  libvorbis-dev libvorbisfile3 libopenal-dev

RUN gem install bundler

# cmake
RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential wget pkg-config automake libltdl-dev clang mingw-w64 zlib1g-dev libcunit1-dev default-jre xxd && \
    wget https://github.com/Kitware/CMake/releases/download/v3.16.2/cmake-3.16.2-Linux-x86_64.sh && \
    bash cmake-3.16.2-Linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.16.2-Linux-x86_64.sh

# emscripten
ENV EMSCRIPTEN_VERSION=sdk-1.39.0-64bit

RUN git clone https://github.com/juj/emsdk.git /emsdk
RUN cd /emsdk && ./emsdk install $EMSCRIPTEN_VERSION && ./emsdk activate $EMSCRIPTEN_VERSION && source ./emsdk_env.sh

# android
ENV ANDROID_HOME /opt/android-sdk
ENV PATH ${PATH}:${ANDROID_HOME}/tools/bin:${ANDROID_HOME}/platform-tools:${PATH}:${ANDROID_HOME}/tools
ENV ANDROID_NDK /opt/android-ndk
ENV ANDROID_NDK_HOME /opt/android-ndk

RUN mkdir -p /opt/android-sdk && mkdir -p ~/.android && touch ~/.android/repositories.cfg
WORKDIR /opt

RUN cd /opt/android-sdk && \
  wget -q --output-document=sdk-tools.zip https://dl.google.com/android/repository/commandlinetools-linux-6200805_latest.zip && \
  unzip sdk-tools.zip && \
  rm -f sdk-tools.zip && \
  yes | sdkmanager --sdk_root=${ANDROID_HOME} "ndk-bundle" && \
  yes | sdkmanager --sdk_root=${ANDROID_HOME} "build-tools;27.0.3" "platforms;android-27" && \
  yes | sdkmanager --sdk_root=${ANDROID_HOME} "extras;android;m2repository" "extras;google;m2repository" "extras;google;google_play_services"

# build system
WORKDIR /blitz3d-ng
ADD ./Gemfile ./Gemfile
ADD ./Gemfile.lock ./Gemfile.lock
RUN bundle install

ADD ./ ./

RUN bin/blitz3d config && make
RUN bin/blitz3d help --build

# for historical purposes, hopefully we can get away from this
ENV blitzpath=/blitz3d-ng/_release

CMD /bin/bash
