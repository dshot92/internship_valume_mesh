# internship_volume_mesh

Diary of internship at CG3HCI Team

# Setup

```
git clone https://github.com/dshot92/internship_volume_mesh.git
cd internship_volume_mesh
chmod +x auto_install.sh
./auto_install.sh
```



------



# After installing

Add function definition to relative .h files

```c++
# cinolib/include/cinolib/meshes/ abstract_mesh.h

bool             vert_is_manifold_cluster   (const uint vid) const;
std::vector<std::vector<uint>> v2p_label_cc(const uint vid) const;
std::vector<std::vector<uint>> e2p_label_cc(const uint eid) const;
```



```c++
# cinolib/include/cinolib/meshes/ trimesh.h

void                vid_label_manifold_fix(const uint vid);
```

```c++
# cinolib/include/cinolib/meshes/ tetmesh.h

void              edge_label_manifold_fix(const uint eid);
void              vid_label_manifold_fix(const uint vid);
```

