// abstract_mesh.cpp

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::vert_is_manifold_cluster(const uint vid) const
{
    std::vector<uint> p_link = this->adj_v2p(vid);
    std::unordered_set<uint> poly_set(p_link.begin(), p_link.end());

    uint index = 0;
    std::vector<std::vector<uint>> components;

    for(auto pid : p_link){
        bool connected = false;
        int label = this->poly_data(pid).label;
        for (auto comp : components){
            if(CONTAINS_VEC(comp, pid)){
               connected = true;
            }
        }
        std::unordered_set<uint> start;
        start.insert(pid);
        if(!connected){
            ++index;
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

    uint i = 0;
    std::unordered_set<int> colors;
    for (auto comp : components){
        for(auto pid : comp){
            if(DOES_NOT_CONTAIN(colors, this->poly_data(pid).label)) colors.insert(this->poly_data(pid).label);
        }
        ++i;
    }

    if ( index > colors.size() ){
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "vid: " + std::__cxx11::to_string(vid)  + "  -  Comp #: " + std::__cxx11::to_string(index) + "  -  Colors #: " + std::__cxx11::to_string(colors.size()) << std::endl;
        i = 0;
        for (auto comp : components){
            std::cout << "Component[" + std::__cxx11::to_string(i) + "] pids: " << std::endl;
            for(auto pid : comp){
                std::cout << std::__cxx11::to_string(pid) + "\t -> Label: " + std::__cxx11::to_string(this->poly_data(pid).label) << std::endl;
            }
            ++i;
        }
    }

    return ( index <= colors.size() );
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
