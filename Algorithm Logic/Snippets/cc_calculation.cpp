//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
std::vector<std::vector<uint>> AbstractMesh<M,V,E,P>::vert_poly_cc_by_labels(const uint vid) const
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
