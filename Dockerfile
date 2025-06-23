FROM archlinux:latest

ENV LANG=en_US.UTF-8

RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm \
    cmake clang llvm  lcov \
    kcoreaddons ki18n kxmlgui kconfigwidgets kwidgetsaddons kio \
    libkdegames base-devel extra-cmake-modules \
    qt6-svg \
    && pacman -Scc --noconfirm

WORKDIR /usr/src/app

COPY . .

RUN cmake -S . -B build \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_INSTALL_PREFIX=/usr/local/kde/usr \
      -DQT_MAJOR_VERSION=6 \
      -DCMAKE_BUILD_TYPE=Debug \
      -DBUILD_TESTS=ON \
      -DCOVERAGE=ON

RUN cmake --build build --target coverage --parallel $(nproc)

CMD ["/bin/bash"]
