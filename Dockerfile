FROM debian:stretch

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y \
  build-essential clang cmake git curl wget unzip \
  ruby2.3 ruby-dev libreadline-dev \
  python2.7 nodejs default-jre \
  mingw-w64 \
  libwxgtk3.0-dev libwxgtk-webview3.0-dev libglew-dev libxcursor-dev \
  libxrandr-dev libxinerama-dev  libxml2-dev zlib1g-dev libssl-dev openssl \
  libgdbm-dev uuid-dev  \
  libvorbis-dev libvorbisfile3 libopenal-dev

RUN gem install bundler

# emscripten
ENV EMSCRIPTEN_VERSION=sdk-1.38.12-64bit

RUN git clone https://github.com/juj/emsdk.git /emsdk
RUN cd /emsdk && ./emsdk install $EMSCRIPTEN_VERSION && ./emsdk activate $EMSCRIPTEN_VERSION

# android
ENV ANDROID_HOME /opt/android-sdk
ENV PATH ${PATH}:${ANDROID_HOME}/tools/bin:${ANDROID_HOME}/platform-tools:${PATH}:${ANDROID_HOME}/tools
ENV ANDROID_NDK /opt/android-ndk
ENV ANDROID_NDK_HOME /opt/android-ndk

RUN mkdir -p /opt/android-sdk && mkdir -p ~/.android && touch ~/.android/repositories.cfg
WORKDIR /opt

RUN cd /opt/android-sdk && \
  wget -q --output-document=sdk-tools.zip https://dl.google.com/android/repository/sdk-tools-linux-3859397.zip && \
  unzip sdk-tools.zip && \
  rm -f sdk-tools.zip && \
  echo y | sdkmanager "build-tools;27.0.3" "platforms;android-27" && \
  echo y | sdkmanager "extras;android;m2repository" "extras;google;m2repository" "extras;google;google_play_services" && \
  sdkmanager "cmake;3.6.4111459"

RUN wget -q --output-document=android-ndk.zip https://dl.google.com/android/repository/android-ndk-r17-linux-x86_64.zip && \
  unzip android-ndk.zip && \
  rm -f android-ndk.zip && \
  mv android-ndk-r17 android-ndk

# build system
WORKDIR /blitz3d-ng
# ADD ./Gemfile ./Gemfile
# ADD ./Gemfile.lock ./Gemfile.lock
# RUN bundle install

# ADD ./ ./

ENV blitzpath=/blitz3d-ng/_release

CMD /bin/bash
