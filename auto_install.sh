#!/bin/bash

# get in home directory
cd 

echo "What do you want to install?"
echo "1) - base programs (git, htop, telegram...)"
echo "2) - cinolib-internship repository"
echo "3) - qt-creator + necessary libraries"
echo "4) - VS-code"
echo "5) - Graphite3 1.7.3"

read command
if [ $command -eq 1 ]; then
	echo $'\n Installing git \n'
	sudo apt install -y git

	echo $'\n Installing gnome-tweak-tool \n'
	sudo apt install -y gnome-tweak-tool

	echo $'\n Installing build-essential \n'
	sudo apt install -y build-essential

	echo $'\n Installing synaptic \n'
	sudo apt install -y synaptic

	echo $'\n Installing htop \n'
	sudo apt install -y htop

	echo $'\n Installing telegram-desktop \n'
	sudo snap install telegram-desktop

	echo $'\n Setting git to save credentials \n'
	git config --global credential.helper store
	exit 1
fi

if [ $command -eq 2 ]; then
	echo $'\n Cloning cinolib \n'
	git clone --recursive https://github.com/mlivesu/cinolib.git

	cd internship_volume_mesh
	echo $'\n Moving and copying files into cinolib \n'
	cp -r internship_examples ../cinolib/examples/
	mv ../cinolib/examples/internship_examples/cpp_files/fix_manifold.h ../cinolib/include/cinolib/
	
	echo "####################################################"
	echo "#                                                  #"
	echo "#  Add function declarations to relative .h files  #"
	echo "#                                                  #"
	echo "####################################################"
	echo ""
	exit 1
fi

if [ $command -eq 3 ]; then
	echo $'\n Installing necessary libraries \n'
	sudo apt install -y libxcb-randr0-dev libxcb-xtest0-dev libxcb-xinerama0-dev libxcb-shape0-dev libxcb-xkb-dev
	sudo apt install -y libgl-dev
	sudo apt install -y mesa-common-dev
	sudo apt install -y libglu1-mesa-dev freeglut3-dev mesa-common-dev libgl1-mesa-dev
	sudo apt install -y libxi-dev build-essential libdbus-1-dev libfontconfig1-dev libfreetype6-dev libx11-dev
	sudo apt install -y libqt4-dev zlib1g-dev libqt4-opengl-dev
	echo $'\n Downloadding Qt-Creator OpenSource \n'
	wget https://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run
	echo $'\n Select Desktop Gcc binaries \n'
	chmod +x qt-unified-linux-x64-online.run
	./qt-unified-linux-x64-online.run
	exit 1
fi

if [ $command -eq 4 ]; then
	echo $'\n Downloading VS-Code \n'
	wget https://az764295.vo.msecnd.net/stable/d2e414d9e4239a252d1ab117bd7067f125afd80a/code_1.50.1-1602600906_amd64.deb
	sudo dpkg -i code_1.50.1-1602600906_amd64.deb
	exit 1
fi

if [ $command -eq 5 ]; then
	echo $'\n Downloading Graphite3 1.7.3 \n'
	wget https://gforge.inria.fr/frs/download.php/file/38234/graphite3_1.7.3.zip
	unzip graphite3_1.7.3.zip
	rm graphite3_1.7.3.zip
	cd graphite3_1.7.3
	sudo apt install -y xorg-dev libglu1-mesa-dev
	echo $'\n Building Graphite3 1.7.3 \n'
	./make_it.sh
	exit 1
fi
