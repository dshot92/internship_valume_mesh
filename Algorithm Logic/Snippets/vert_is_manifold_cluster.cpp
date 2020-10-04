//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractMesh<M,V,E,P>::vert_is_manifold_cluster(const uint vid) const
{
    std::vector<uint> p_link = this->adj_v2p(vid);
    std::unordered_set<uint> poly_set(p_link.begin(), p_link.end());

    uint index = 0;
    std::vector<std::vector<uint>> components;
    std::unordered_map<int, int> labels;

    // If all poly in p_link have the same label, vid is completely int eh cluster and cannot be non-manifold

    int first = this->poly_data(p_link.at(0)).label;
    bool inside_cluster = true;
    for(auto pid : p_link){
        if(this->poly_data(pid).label != first)
            inside_cluster = false;
    }

    if(inside_cluster){
        return true;
    }
    else{
        components = this->vert_poly_cc_by_labels(vid);


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

        /// Pretty print for Connected Componets

        uint i = 0;

        if ( components.size() > labels.size() ){
            std::cout << "--------------------------------------" << std::endl;
            std::cout << "vid: " + std::__cxx11::to_string(vid)  + "  -  Comp #: " + std::__cxx11::to_string(index) + "  -  Colors #: " + std::__cxx11::to_string(labels.size()) << std::endl;
            i = 0;

            std::vector<uint> tunnel(components.at(0));

            for (auto comp : components){
                if(comp.size() < tunnel.size() ) tunnel = comp;

                std::cout << "Component[" + std::__cxx11::to_string(i) + "] pids: " << std::endl;
                for(auto pid : comp){
                    std::cout << std::__cxx11::to_string(pid) + "\t -> Label: " + std::__cxx11::to_string(this->poly_data(pid).label) << std::endl;
                }
                ++i;
            }
        }



        return !( components.size() > labels.size() );
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::