# cinolib/include/cinolib/meshes/ abstract_mesh.h

bool             vert_is_manifold_cluster   (const uint vid) const;
std::vector<std::vector<uint>> v2p_label_cc(const uint vid) const;
std::vector<std::vector<uint>> e2p_label_cc(const uint eid) const;



# cinolib/include/cinolib/meshes/ trimesh.h

void                vid_label_manifold_fix(const uint vid);



# cinolib/include/cinolib/meshes/ tetmesh.h

void              edge_label_manifold_fix(const uint eid);
void              vid_label_manifold_fix(const uint vid);
