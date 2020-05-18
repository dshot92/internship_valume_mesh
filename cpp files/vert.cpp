//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
bool AbstractPolygonMesh<M,V,E,P>::vert_is_manifold(const uint vid, Color c) const
{
    for(uint eid : this->adj_v2e(vid))
    {
        if(!this->edge_is_manifold(eid)) return false;
    }

    std::vector<uint> p_star = this->vert_ordered_polys_star(vid);
    std::unordered_set<uint> edge_set(p_star.begin(), p_star.end());

    std::queue<uint> q;
    q.push(p_star.front());

    std::unordered_set<uint> visited;
    visited.insert(p_star.front());

    Color curr_color = this->poly_data(q.front()).color;

    bool change1 = false;
    bool change2 = false;
    bool change3 = false;
    bool found_color = false;

    for(auto pid : p_star){
        if(this->poly_data(pid).color == c){
            found_color = true;     // Cycle over p_star untill first poly of selected cluster is found
        }
        if(found_color){
            if(change2){
                if(this->poly_data(pid).color != c){
                    change3 = true;
                }
            }
            if(change1){
                if(this->poly_data(pid).color == c){
                    //curr_color = this->poly_data(pid).color;
                    change2 = true;
                }
            }
            if(this->poly_data(pid).color != c){
                //curr_color = this->poly_data(pid).color;
                change1 = true;
            }
        }
    }
    return (!change3);
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
