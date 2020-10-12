# internship_volume_mesh

Diary of internship at CG3HCI Team

# Cinolib install

```
git clone --recursive https://github.com/mlivesu/cinolib.git
```

Download Repository

```
git clone https://github.com/dshot92/internship_volume_mesh.git
cd internship_volume_mesh
cp -r internship_examples ../cinolib/examples/
```



------



# Geogram 1.7.6 install

```
wget https://gforge.inria.fr/frs/download.php/file/38362/geogram_1.7.6.zip
unzip geogram_1.7.6.zip
cd geogram_1.7.6
sudo apt-get install xorg-dev libglu1-mesa-dev
./configure.sh
cd build/Linux64-gcc-dynamic-Release
make
```

# Geogram 1.7.6 Usage

```
cd {mesh folder path}
geobox {mesh_file}
```

