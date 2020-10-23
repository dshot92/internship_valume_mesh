#include <cinolib/meshes/trimesh.h>
#include <cinolib/meshes/tetmesh.h>
#include <chrono>

inline static std::chrono::time_point<std::chrono::system_clock> startChrono()
{
    return std::chrono::system_clock::now();
}

inline double stopChrono(std::chrono::time_point<std::chrono::system_clock> &start)
{
    auto time = std::chrono::system_clock::now() - start;
    return std::chrono::duration <double, std::milli> (time).count() / 1000;
}

namespace cinolib
{
template<class M, class V, class E, class P>
CINO_INLINE
void fix_non_manifold_verts(Trimesh<M,V,E,P> &mesh){

    uint verts = mesh.num_verts();
    auto t = startChrono();
    // Check every vert of mesh
    for(uint vid = 0; vid < verts ; ++vid){
        if( !mesh.vert_is_manifold_cluster(vid)){

            // Fix Non manifold vertss
            mesh.vid_label_manifold_fix(vid);

        }

        // Update number of mesh vertices
        verts = mesh.num_verts();
    }
    double time = stopChrono(t);
    std::cout << "Fixing Time: " << time << std::endl;
}

template<class M, class V, class E, class P>
CINO_INLINE
void fix_non_manifold_verts(Tetmesh<M,V,E,P> &mesh){

    uint v0, v1;
    std::set<uint> poly_edges;
    std::set<uint> fixed_vids;
    std::set<uint> fixed_edges;

    uint verts = mesh.num_verts();

    // Check every vert of mesh
    // for each vertex get edges and check edge vertices
    for(uint mesh_vid = 0; mesh_vid < verts ; ++mesh_vid){

        // Get edges incident to vid and cycle over them
        poly_edges.clear();
        for(auto v : mesh.adj_v2e(mesh_vid)){
            poly_edges.insert(v);
        }

        for(auto eid : poly_edges){

            // Verts of eid
            v0 = mesh.edge_vert_id(eid, 0);
            v1 = mesh.edge_vert_id(eid, 1);

            // Case EDGE non manifold
            if ( !mesh.vert_is_manifold_cluster(v0) &&
                 !mesh.vert_is_manifold_cluster(v1) &&
                 DOES_NOT_CONTAIN(fixed_edges, eid)
                 ){

                // add edge to set of fixed edges
                fixed_edges.insert(eid);

                // fix non manifold eid
                mesh.edge_label_manifold_fix(eid);
            }
            else{

                // Cases for single Vertex of the edge non manifold
                if ( !mesh.vert_is_manifold_cluster(v0) &&
                     DOES_NOT_CONTAIN(fixed_vids, v0)){

                    // add it set of fixed vids
                    fixed_vids.insert(v0);

                    // fix non manifold vid
                    mesh.vid_label_manifold_fix(v0);
                }

                if ( !mesh.vert_is_manifold_cluster(v1) &&
                     DOES_NOT_CONTAIN(fixed_vids, v1)){

                    // add it set of fixed vids
                    fixed_vids.insert(v1);

                    // fix non manifold vid
                    mesh.vid_label_manifold_fix(v1);
                }
            }
        }

        // Update mesh to add the new vid to be checked
        verts = mesh.num_verts();
    }
}

}
