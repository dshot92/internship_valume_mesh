// abstract_mesh.cpp

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::vert_is_manifold_cluster(const uint vid, int lab) const
{
    std::vector<uint> p_link = this->adj_v2p(vid);
    std::unordered_set<uint> poly_set(p_link.begin(), p_link.end());

    std::unordered_set<uint> cluster;
    for(uint pid : p_link){
        if(this->poly_data(pid).label == lab){
            cluster.insert(pid); //number of wanted polys
        }
    }

    if (cluster.size() < 2) return true;
    else{
        uint index = 0;
        std::vector<std::vector<uint>> components;

        for(auto pid : cluster){
            bool connected = false;
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
                            if(this->poly_data(nbr).label == lab){
                                q.push(nbr);
                                visited.push_back(nbr);
                            }
                        }
                    }
                }
                components.push_back(visited);
            }
        }

        if ( index >= 2 ){
            std::cout << "Cluster polys around vertex(" + std::__cxx11::to_string(vid) +") : " + std::__cxx11::to_string(cluster.size()) + " - Connected Components number = " + std::__cxx11::to_string(components.size()) << std::endl;
            uint i = 0;
            for (auto comp : components){
                std::cout << "Component[" + std::__cxx11::to_string(i) + "] pids: " << std::endl;
                for(auto pid : comp){
                    std::cout << std::__cxx11::to_string(pid) << std::endl;
                }
                ++i;
            }
        }
        return ( index < 2 );
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
