#include <cinolib/meshes/abstract_mesh.h>
#include <cinolib/meshes/trimesh.h>
#include <cinolib/meshes/drawable_trimesh.h>
#include <cinolib/meshes/tetmesh.h>
#include <cinolib/meshes/drawable_polyhedralmesh.h>
#include <chrono>
#include <queue>
#include <stack>

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

// Abstract_mesh.cpp
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::vert_is_manifold_cluster(const uint vid) const
{
    /* Detect Manifoldness of a VID in relation to it adjacents Polys labels */

    std::vector<std::vector<uint>> components = this->v2p_label_cc(vid); // Polys Connected Components around VID
    std::unordered_map<int, int> labels;

    std::vector<uint> p_link = this->adj_v2p(vid);
    std::unordered_set<uint> poly_set(p_link.begin(), p_link.end());

    // If all poly in p_link have the same label, vid is completely int eh cluster and cannot be non-manifold
    int first_label = this->poly_data(p_link.at(0)).label;
    bool inside_cluster = true;
    for(auto pid : p_link){
        if(this->poly_data(pid).label != first_label)
            inside_cluster = false;
    }

    if(inside_cluster){
        return true;
    }
    else{

        // Calculate number of distinct lables with a map
        for(auto comp : components){
            int curr_label = this->poly_data(comp[0]).label;

            auto query = labels.find(curr_label);

            if(query == labels.end()){
                labels.insert(std::make_pair(curr_label, 1));
            }else{
                query->second++;
            }
        }

        // VID is manifold if the number of Connected Components is as big as the number of labels
        // Return value in NEGATED, as is called checking for MANIFOLDNESS
        return !( components.size() > labels.size());
    }
}

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<std::vector<uint>> AbstractMesh<M,V,E,P>::v2p_label_cc(const uint vid) const
{
    /* Function returns Connected Componets of Polys around a VID */

    // Pids of all polys adjacent to VID
    std::vector<uint> p_link = this->adj_v2p(vid);
    std::set<uint> poly_set(p_link.begin(), p_link.end());

    std::vector<std::vector<uint>> components;
    std::map<int, int> labels;

    /* Algoritmh idea:
     * for each pid in p_link:
     *      if already connected to a components : skip it
     *      else
     *          BSF around it on poly_set pids
     *          add vector of BFS search of to components
    */
    for(auto pid : p_link){
        bool connected = false;
        int label = this->poly_data(pid).label;
        auto query = labels.find(label);
        if(query == labels.end()){
            labels.insert(std::make_pair(label, 1));
        }else{
            query->second++;
        }

        // Check if already present in some other pid connected component of poly set
        for (auto comp : components){
            if(CONTAINS_VEC(comp, pid)){
               connected = true;
            }
        }

        // set with non connected pid sto start BFS search
        std::set<uint> start;
        start.insert(pid);

        // If not in a componets, BFS around it
        if(!connected){

            // Get pid label, if new add it to label map
            int curr_label = this->poly_data(pid).label;
            auto query = labels.find(curr_label);
            if(query == labels.end()){
                labels.insert(std::make_pair(curr_label, 1));
            }else{
                query->second++;
            }

            std::queue<uint> q;
            q.push(*start.begin());

            //Visited contains all pid of poly connected to the first in the cluster
            std::vector<uint> visited;
            visited.push_back(*start.begin());

            // BFS
            while(!q.empty()){
                uint curr = q.front();
                q.pop();

                for (auto nbr : this->adj_p2p(curr)){
                    if(CONTAINS(poly_set, nbr) && DOES_NOT_CONTAIN_VEC(visited, nbr) ){
                        if(this->poly_data(nbr).label == label){
                            q.push(nbr);
                            visited.push_back(nbr);
                        }
                    }
                }
            }
            // Add BFS result of pids to Connected Components vector
            components.push_back(visited);
        }
    }

    // Return Connected Components
    return components;}

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<std::vector<uint>> AbstractMesh<M,V,E,P>::e2p_label_cc(const uint eid) const
{
    /* Function returns Connected Componets of Polys around a EID */

    // Pids of all polys adjacent to EID
    std::vector<uint> e_link = this->adj_e2p(eid);
    std::set<uint> poly_set(e_link.begin(), e_link.end());

    std::vector<std::vector<uint>> components;
    std::map<int, int> labels;

    /* Algoritmh idea:
     * for each pid in e_link:
     *      if already connected to a components : skip it
     *      else
     *          BSF around it on poly_set pids
     *          add vector of BFS search of to components
    */

    for(auto pid : e_link){

        bool connected = false;

        int label = this->poly_data(pid).label;
        auto query = labels.find(label);
        if(query == labels.end()){
            labels.insert(std::make_pair(label, 1));
        }else{
            query->second++;
        }

        // Check if already present in some other pid connected component of poly set
        for (auto comp : components){
            if(CONTAINS_VEC(comp, pid)){
               connected = true;
            }
        }

        // set with non connected pid sto start BFS search
        std::set<uint> start;
        start.insert(pid);

        // If not in a componets, BFS around it
        if(!connected){

            // Get pid label, if new add it to label map
            int curr_label = this->poly_data(pid).label;
            auto query = labels.find(curr_label);
            if(query == labels.end()){
                labels.insert(std::make_pair(curr_label, 1));
            }else{
                query->second++;
            }

            std::queue<uint> q;
            q.push(*start.begin());

            //Visited contains all pid of poly connected to the first in the cluster
            std::vector<uint> visited;
            visited.push_back(*start.begin());

            // BFS
            while(!q.empty()){
                uint curr = q.front();
                q.pop();

                for (auto nbr : this->adj_p2p(curr)){
                    if(CONTAINS(poly_set, nbr) && DOES_NOT_CONTAIN_VEC(visited, nbr) ){
                        if(this->poly_data(nbr).label == label){
                            q.push(nbr);
                            visited.push_back(nbr);
                        }
                    }
                }
            }
            // Add BFS result of pids to Connected Components vector
            components.push_back(visited);
        }
    }
    // Return Connected Components
    return components;
}


// trimesh.cpp
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void Trimesh<M,V,E,P>::vid_label_manifold_fix(const uint vid)
{
    /* Tunnel to recolor idea
     * 1 - Calculate label with highest poly count
     * 2 - Get bigger component with that label
     * 3 - From that components DFS untill component.size != label_count
    */

    // Cut edges adjacent on non manivold Vert
    std::vector<uint> e_link = this->adj_v2e(vid);
    for (auto eid : e_link){
        this->edge_split(eid, 0.5);
    }

    // Calculate Poly Connected Components around VID
    std::vector<std::vector<uint>> components = this->v2p_label_cc(vid);
    std::unordered_map<int, int> labels;

    // Calculate Unique Labels
    for(auto comp : components){
        int curr_label = this->poly_data(comp[0]).label;
        auto query = labels.find(curr_label);
        if(query == labels.end()){
            labels.insert(std::make_pair(curr_label, 1));
        }else{
            query->second++;
        }
    }

    // Calculate label occurences in Connected Components
    std::map<int, int> count_label;
    for (auto comp: components){
        for(auto pid: comp){
            int curr_label = this->poly_data(pid).label;
            auto query = count_label.find(curr_label);
            if(query == count_label.end()){
                count_label.insert(std::make_pair(curr_label, 1));
            }else{
                query->second++;
            }
        }
    }

    // Calculate higheset label count by poly
    int highest_label_count = 0;
    int highest_label = 0;
    for(auto label : labels ){
        if(label.second > highest_label_count){
            highest_label_count = label.second;
            highest_label = label.first;
        }
    }

    // Calculate biggest component with highest_label_count
    uint biggest_component_index = 0;
    for(uint i = 0; i < components.size(); ++i){
        int comp_label = this->poly_data(components[i].at(0)).label;

        if( comp_label == highest_label && components[i].size() > components[biggest_component_index].size() ){
            biggest_component_index = i;
        }
    }

    int total_label_count = count_label[highest_label];

    for(auto pid : this->adj_v2p(vid)){
        this->poly_data(pid).label = highest_label;
    }

    /* Recap:
     *           highest_label = number of highest poly with a given label
     * biggest_component_index = index of component with that label
     *       total_label_count = number of polys with that label
    */

    // Shortest route

//    // Exoands around vid searching for shortest route
//    std::vector<uint> start_comp = components[biggest_component_index];
//    std::vector<uint> contour;
//    std::vector<uint> expand;
//    for (auto pid : start_comp){
//        for (auto pid2 : this->adj_p2p(pid)){
//            contour.push_back(pid2);
//        }
//    }

//    for (auto pid : contour){
//        if (CONTAINS_VEC(this->adj_v2p(vid), pid) && DOES_NOT_CONTAIN_VEC(start_comp, pid)){
//            expand.push_back(pid);
//        }
//    }

//    std::stack<std::vector<uint>> q;
//    q.push(components[uint(biggest_component_index)]);

//    //Visited contains all pid of poly connected to the first in the cluster
//    std::vector<uint> to_relabel;
//    std::vector<uint> visited;

//    uint indexes_path;
//    bool found = false;
//    while(!found){
//        auto curr = q.top();
//        q.pop();
//        indexes_path = 0;
//        for (auto nbr : components){
//            if(nbr != curr){
//                if (this->poly_data(nbr[0]).label != highest_label){
//                    q.push(nbr);
//                    to_relabel.push_back(indexes_path);
//                }
//                else{
//                    found = true;
//                }
//            }
//            indexes_path++;
//        }
//    }

//    for(auto ind : to_relabel){
//        for( auto pid : components[ind]){
//            this->poly_data(pid).label = highest_label;
//        }
//    }
}


// tetmesh.cpp
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void Tetmesh<M,V,E,F,P>::edge_label_manifold_fix(const uint eid)
{
    // Get Components
    std::vector<std::vector<uint>> components = this->e2p_label_cc(eid);
    std::unordered_map<int, int> labels;

    // Calculate Unique Labels
    for(auto comp : components){
        for(auto pid : comp){
            int curr_label = this->poly_data(pid).label;
            auto query = labels.find(curr_label);
            if(query == labels.end()){
                labels.insert(std::make_pair(curr_label, 1));
            }else{
                query->second++;
            }
        }
    }

    // Get most common label
    int most_pids_label_count = 0;
    int most_pids_label = this->poly_data(components[0].at(0)).label;
    most_pids_label_count = 0;
    for(auto label : labels ){
        if(label.second > most_pids_label_count){
            most_pids_label_count = label.second;
            most_pids_label = label.first;
        }
    }

    // Cut around EID
    auto new_vid = this->edge_split(eid, 0.5);

    // Apply New labels
    for(auto pid : this->adj_v2p(new_vid)){
        this->poly_data(pid).label = most_pids_label;
    }
}

template<class M, class V, class E, class F, class P>
CINO_INLINE
void Tetmesh<M,V,E,F,P>::vid_label_manifold_fix(const uint vid)
{
    // Get connected Compoents
    std::vector<std::vector<uint>> components = this->v2p_label_cc(vid);
    std::unordered_map<int, int> labels;

    // Calculate Unique Labels
    for(auto comp : components){
        for(auto pid : comp){
            int curr_label = this->poly_data(pid).label;
            auto query = labels.find(curr_label);
            if(query == labels.end()){
                labels.insert(std::make_pair(curr_label, 1));
            }else{
                query->second++;
            }
        }
    }

    // Get most common label
    int most_pids_label_count = 0;
    int most_pids_label = this->poly_data(components[0].at(0)).label;
    most_pids_label_count = 0;
    for(auto label : labels ){
        if(label.second > most_pids_label_count){
            most_pids_label_count = label.second;
            most_pids_label = label.first;
        }
    }

    // Cut Around vid
    std::vector<uint> e_link = this->adj_v2e(vid);
    std::unordered_set<uint> edge_set(e_link.begin(), e_link.end());

    std::queue<uint> q;
    q.push(e_link.front());

    std::unordered_set<uint> visited;
    visited.insert(e_link.front());

    std::unordered_set<uint> new_vids;
    while(!q.empty())
    {
        uint curr = q.front();
        q.pop();

        assert(CONTAINS(visited, curr));

        for(uint nbr : this->adj_e2e(curr))
        {
            // still in the link of vid, but not visited yet
            if(CONTAINS(edge_set, nbr) && !CONTAINS(visited, nbr))
            {
                visited.insert(nbr);
                new_vids.insert(this->edge_split(nbr, 0.5));
                q.push(nbr);
            }
        }
    }

    // Recolor pids around cutted edges
    for(auto v : new_vids){
        for(auto pid : this->adj_v2p(v)){
            this->poly_data(pid).label = most_pids_label;
        }
    }

}


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
