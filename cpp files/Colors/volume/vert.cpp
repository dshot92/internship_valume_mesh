//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class C>
CINO_INLINE
bool AbstractPolyhedralMesh<M,V,E,F,C>::vert_is_manifold(const uint vid, Color c) const
{
    std::unordered_set<uint> reds;
    std::vector<uint> incident_polys = this->adj_v2p(vid);

    for(uint pid : incident_polys ) {
        if( this->poly_data(pid).color == c ){
            reds.insert(pid);
        }
    }

//    if a red is adjacent to another red, is connected
    // cycle over each red, if another red is adjacent then connected

    if(reds.size() > 1){

        std::unordered_set<uint> connected;
        connected.insert(*reds.begin());
        std::queue<uint> q;
        q.push(*reds.begin());

//        for(uint red : reds )
//        {
//            for(uint pid: reds){
//                std::unordered_set<uint> adjacent;
//                adjacent.insert(*this->adj_p2p(pid).begin());

//                if(CONTAINS(adjacent, red) && DOES_NOT_CONTAIN(connected, red) && pid != red && this->poly_data(pid)){
//                    connected.insert(red);
//                }
//            }
//        }

        while(!q.empty())
        {
            uint id = q.front();
            q.pop();

            for(uint nbr : this->adj_p2p(id) )
            {
                if(CONTAINS(reds,nbr) && DOES_NOT_CONTAIN(connected,nbr) )
                {
                    connected.insert(nbr);
                    q.push(nbr);
                }
            }
        }
        std::cout << "Reds: " + std::__cxx11::to_string(reds.size()) + " - Connected: " + std::__cxx11::to_string(connected.size()) << std::endl;
        return !(reds.size() > connected.size());
    }
    else {
        // Vertex was not near wanted cluster polys, must be manifold
        return true;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
