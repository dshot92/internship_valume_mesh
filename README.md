# internship_volume_mesh

Diary of internship at CG3HCI Team

# Setup

### Cinolib

```bash
git clone --recursive https://github.com/mlivesu/cinolib.git
```

##### Repository files

```bash
git clone https://github.com/dshot92/internship_volume_mesh.git
cd internship_volume_mesh
cp -r internship_examples ../cinolib/examples/
```

### Graphite3 1.7.3 Install

```bash
wget https://gforge.inria.fr/frs/download.php/file/38234/graphite3_1.7.3.zip
unzip geogram_1.7.6.zip
cd graphite3_1.7.3
sudo apt-get install xorg-dev libglu1-mesa-dev
./configure.sh
cd build/Linux-gcc-dynamic-release
make
```

##### Extra - Create Icon for Launch-bar

```bash
sudo -i gedit /usr/share/applications/name.desktop
```

Input paths:

```bash
[Desktop Entry]
Type=Application
Terminal=true
Name=Graphite
Icon=//home/dshot/Downloads/graphite3_1.7.3/GraphiteThree/lib/icons/logos/graphite-window-icon.xpm
Exec=/home/dshot/Downloads/graphite3_1.7.3/bin/graphite.sh
```

Icon is created and can be set as favorite.



------



# Add functions into files 

File path:

```
/cinolib/include/cinolib/meshes/
```

#### abstract_mesh.h

```c++
bool             vert_is_manifold_cluster   (const uint vid) const;
std::vector<std::vector<uint>> v2p_label_cc(const uint vid) const;
std::vector<std::vector<uint>> e2p_label_cc(const uint eid) const;
```

#### abstract_mesh.cpp

```c++
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::vert_is_manifold_cluster(const uint vid) const
{
    std::vector<uint> p_link = this->adj_v2p(vid);
    std::unordered_set<uint> poly_set(p_link.begin(), p_link.end());

    std::vector<std::vector<uint>> components;
    std::unordered_map<int, int> labels;

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

        components = this->v2p_label_cc(vid);

        /// Calculate number of distinct lables
        for(auto comp : components){
            int curr_label = this->poly_data(comp[0]).label;

            auto query = labels.find(curr_label);

            if(query == labels.end()){
                labels.insert(std::make_pair(curr_label, 1));
            }else{
                query->second++;
            }
        }

        return !( components.size() > labels.size());
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<std::vector<uint>> AbstractMesh<M,V,E,P>::v2p_label_cc(const uint vid) const
{
    std::vector<uint> p_link = this->adj_v2p(vid);
    std::unordered_set<uint> poly_set(p_link.begin(), p_link.end());

    uint index = 0;
    std::vector<std::vector<uint>> components;
    std::unordered_map<int, int> labels;

    for(auto pid : p_link){
        bool connected = false;
        int label = this->poly_data(pid).label;
        auto query = labels.find(label);
        if(query == labels.end()){
            labels.insert(std::make_pair(label, 1));
        }else{
            query->second++;
        }
        for (auto comp : components){
            if(CONTAINS_VEC(comp, pid)){
               connected = true;
            }
        }
        std::unordered_set<uint> start;
        start.insert(pid);
        if(!connected){
            ++index;
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
            components.push_back(visited);
        }
    }

    return components;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<std::vector<uint>> AbstractMesh<M,V,E,P>::e2p_label_cc(const uint eid) const
{
    std::vector<uint> p_link = this->adj_e2p(eid);
    std::unordered_set<uint> poly_set(p_link.begin(), p_link.end());

    uint index = 0;
    std::vector<std::vector<uint>> components;
    std::unordered_map<int, int> labels;

    for(auto pid : p_link){
        bool connected = false;
        int label = this->poly_data(pid).label;
        auto query = labels.find(label);
        if(query == labels.end()){
            labels.insert(std::make_pair(label, 1));
        }else{
            query->second++;
        }
        for (auto comp : components){
            if(CONTAINS_VEC(comp, pid)){
               connected = true;
            }
        }
        std::unordered_set<uint> start;
        start.insert(pid);
        if(!connected){
            ++index;
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
            components.push_back(visited);
        }
    }

    return components;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
```



------



#### tetmesh.h

```c++
void              edge_label_manifold_fix(const uint eid);
void              vid_label_manifold_fix(const uint vid);
```

#### tetmesh.cpp

```c++
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
std::vector<uint> Tetmesh<M,V,E,F,P>::poly_faces_opposite_to(const uint pid, const uint eid) const
{
    assert(this->poly_contains_edge(pid, eid));
    return this->poly_e2f(pid, this->poly_edge_opposite_to(pid,eid));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void Tetmesh<M,V,E,F,P>::edge_label_manifold_fix(const uint eid)
{
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

    int most_pids_label = this->poly_data(components[0].at(0)).label;
    int most_pids_label_count = 0;

    // Calculate Unique Labels
    for(auto pid : this->adj_e2p(eid)){
        int curr_label = this->poly_data(pid).label;
        auto query = labels.find(curr_label);
        if(query == labels.end()){
            labels.insert(std::make_pair(curr_label, 1));
        }else{
            query->second++;
        }
    }

    auto new_vid = this->edge_split(eid, 0.5);

    for(auto pid : this->adj_v2p(new_vid)){
        this->poly_data(pid).label = most_pids_label;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void Tetmesh<M,V,E,F,P>::vid_label_manifold_fix(const uint vid)
{
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

    int most_pids_label = this->poly_data(components[0].at(0)).label;
    int most_pids_label_count = 0;
    for(auto label : labels ){
        if(label.second > most_pids_label_count){
            most_pids_label_count = label.second;
            most_pids_label = label.first;
        }
    }

    for(auto pid : this->adj_v2p(vid)){
        this->poly_data(pid).label = most_pids_label;
    }

    //Cutting
    std::vector<uint> e_link = this->adj_v2e(vid);
    std::unordered_set<uint> edge_set(e_link.begin(), e_link.end());

    std::queue<uint> q;
    q.push(e_link.front());

    std::unordered_set<uint> visited;
    visited.insert(e_link.front());

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
                this->edge_split(nbr, 0.5);
                q.push(nbr);
            }
        }
    }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
```



------

