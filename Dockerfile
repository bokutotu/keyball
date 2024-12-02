# ベースイメージとしてUbuntuを使用
FROM ubuntu:22.04

# 環境変数を設定
ENV DEBIAN_FRONTEND=noninteractive
ENV HOME=/root
ENV PATH="/usr/local/bin:${PATH}"
ENV QMK_HOME=/qmk_firmware

# 必要なパッケージをインストール
RUN apt-get update && apt-get install -y \
    git \
    python3 \
    python3-pip \
    wget \
    unzip \
    gcc \
    avr-libc \
    binutils-arm-none-eabi \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    avrdude \
    dfu-programmer \
    dfu-util \
    gcc-avr \
    binutils-avr \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリを作成
WORKDIR /workspace

# QMKファームウェアをクローン（指定バージョン）
RUN git clone https://github.com/qmk/qmk_firmware.git --depth 1 --recurse-submodules --shallow-submodules -b 0.22.14 ${QMK_HOME}

# Keyballリポジトリをクローン
RUN git clone https://github.com/Yowkees/keyball.git

# Keyballのシンボリックリンクを作成
RUN ln -s /workspace/keyball/qmk_firmware/keyboards/keyball ${QMK_HOME}/keyboards/keyball

# QMKの依存関係をインストール
WORKDIR ${QMK_HOME}
RUN python3 -m pip install -r requirements.txt
RUN python3 -m pip install qmk

# コンテナ起動時のデフォルトコマンド
WORKDIR ${QMK_HOME}
CMD ["/bin/bash"]
