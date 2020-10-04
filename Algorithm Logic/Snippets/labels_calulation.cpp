std::vector<std::vector<uint>> components = m.vert_poly_cc_by_labels(vid);
std::unordered_map<int, int> labels;

for(auto comp : components){
    int curr_label = m.poly_data(comp[0]).label;
    auto query = labels.find(curr_label);
    if(query == labels.end()){
        labels.insert(std::make_pair(curr_label, 1));
    }else{
        query->second++;
    }
}