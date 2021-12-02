set -e

apt update
apt install -y sudo git curl wget apt-transport-https patchelf software-properties-common lsb-release pkg-config libgl1-mesa-dev libegl1-mesa
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
add-apt-repository -y ppa:deadsnakes/ppa
add-apt-repository -y ppa:ubuntu-toolchain-r/test
apt update
apt install -y cmake python3.9 python3.9-distutils g++-8
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 60 --slave /usr/bin/g++ g++ /usr/bin/g++-8
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
python3.9 get-pip.py
pip install conan
conan profile new --detect default
ln -s /usr/bin/python3.9 /usr/bin/python
