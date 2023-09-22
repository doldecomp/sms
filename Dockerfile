FROM devkitpro/toolchain-base

WORKDIR /sms
ADD . /sms

RUN dkp-pacman -Syyu --noconfirm gamecube-dev && \
    dkp-pacman -S --needed --noconfirm ppc-portlibs gamecube-portlibs && \
    dkp-pacman -S --needed --noconfirm devkitARM && \
    yes | dkp-pacman -Scc

RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install --no-install-recommends --assume-yes wine wine32
ENV DISPLAY :0
RUN apt-get install -y gcc build-essential

ENV DEVKITPPC=${DEVKITPRO}/devkitPPC
ENV DEVKITARM=/opt/devkitpro/devkitARM

ENTRYPOINT [ "tail -f /dev/null" ]
