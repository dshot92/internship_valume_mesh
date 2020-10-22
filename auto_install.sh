#!/bin/bash

echo "What do you want to install?"
echo "1) - base programs (git, htop, telegram...)"
echo "2) - cinolib-internship repository"
echo "3) - qt-creator + necessary libraries"
echo "4) - VS-code"
echo "5) - Graphite3 1.7.3"

read command
if [ $command -eq 1 ]; then
	echo $'\n Installing base programs \n'
	sudo apt install -y git
	sudo apt install -y gnome-tweak-tool
	sudo apt install -y build-essential
	sudo apt install -y synaptic
	sudo apt install -y htop
	sudo snap install telegram-desktop
	git config --global credential.helper store
	exit 1
fi

if [ $command -eq 2 ]; then
	echo $'\n Installing cinolib-internship repository \n'
	git clone --recursive https://github.com/mlivesu/cinolib.git
	git clone https://github.com/dshot92/internship_volume_mesh.git
	cd internship_volume_mesh
	cp -r internship_examples ../cinolib/examples/
	exit 1
fi

if [ $command -eq 3 ]; then
	echo $'\n Installing qt-creator + necessary libraries \n'
	sudo apt install -y libxcb-randr0-dev libxcb-xtest0-dev libxcb-xinerama0-dev libxcb-shape0-dev libxcb-xkb-dev
	sudo apt install -y libgl-dev
	sudo apt install -y mesa-common-dev
	sudo apt install -y libglu1-mesa-dev freeglut3-dev mesa-common-dev libgl1-mesa-dev
	sudo apt install -y libxi-dev build-essential libdbus-1-dev libfontconfig1-dev libfreetype6-dev libx11-dev
	sudo apt install -y libqt4-dev zlib1g-dev libqt4-opengl-dev
	wget https://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run
	chmod +x qt-unified-linux-x64-online.run
	./qt-unified-linux-x64-online.run
	exit 1
fi

if [ $command -eq 4 ]; then
	echo $'\n Installing VS-code \n'
	wget https://az764295.vo.msecnd.net/stable/d2e414d9e4239a252d1ab117bd7067f125afd80a/code_1.50.1-1602600906_amd64.deb
	sudo dpkg -i code_1.50.1-1602600906_amd64.deb
	exit 1
fi

if [ $command -eq 5 ]; then
	echo $'\n Installing Graphite3 1.7.3 \n'
	wget https://gforge.inria.fr/frs/download.php/file/38234/graphite3_1.7.3.zip
	unzip graphite3_1.7.3.zip
	rm graphite3_1.7.3.zip
	cd graphite3_1.7.3
	sudo apt install -y xorg-dev libglu1-mesa-dev
	./make_it.sh
	exit 1
fi
