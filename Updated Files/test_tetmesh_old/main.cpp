/* This is a base application for cinolib (https://github.com/maxicino/cinolib).
 * It will show a GL canvas (and associated control panel) to interact
 * with a tetrahedral mesh.
 *
 * In case you don't need a GUI, you can drop the "Drawable" prefix from the mesh data type.
 * What you will get is a lighter yet fully operational mesh data structure, just
 * without the burden of OpenGL code necessary for rendering!
 *
 * Enjoy!
*/

#include <QApplication>
#include <cinolib/meshes/meshes.h>
#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/drawable_sphere.h>
#include <stack>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char **argv)
{
    using namespace cinolib;
    using namespace std;

    QApplication a(argc, argv);

//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/sphere.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/part.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_vertex.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_vertex_seven.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_edge.mesh";
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/cube_tet.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_tet.mesh";

    // part file download link
    // https://people.sc.fsu.edu/~jburkardt/data/mesh/mesh.html

    DrawableTetmesh<> m(s.c_str());
    m.translate(-m.centroid());
//    m.scale(3);

    m.scale(0.2);
//    m.translate(m.bbox().center());

//    m.rotate(vec3d(1,0,0),0.35);
    m.updateGL(); //Always update after transforms on mesh

    /// Labelling Around World Origin

    {
//        std::vector<int> labels(8);
//        labels[0] = 0;
//        labels[1] = 1;
//        labels[2] = 2;
//        labels[3] = 3;
//        labels[4] = 4;
//        labels[5] = 5;
//        labels[6] = 6;
//        labels[7] = 7;

//        double x,y,z;
//        for(uint pid=0; pid < m.num_polys(); ++pid){
//            x = m.poly_centroid(pid).x();
//            y = m.poly_centroid(pid).y();
//            z = m.poly_centroid(pid).z();

//            if(x >= 0 && y >= 0 && z >= 0) m.poly_data(pid).label = 0;
//            if(x >= 0 && y >= 0 && z <= 0) m.poly_data(pid).label = 1;
//            if(x >= 0 && y <= 0 && z >= 0) m.poly_data(pid).label = 2;
//            if(x >= 0 && y <= 0 && z <= 0) m.poly_data(pid).label = 3;
//            if(x <= 0 && y >= 0 && z >= 0) m.poly_data(pid).label = 4;
//            if(x <= 0 && y >= 0 && z <= 0) m.poly_data(pid).label = 5;
//            if(x <= 0 && y <= 0 && z >= 0) m.poly_data(pid).label = 6;
//            if(x <= 0 && y <= 0 && z <= 0) m.poly_data(pid).label = 7;

//        }
//        m.updateGL(); //Always update after transforms on mesh
    }

    /// Costume Labelling for Cube_Tet.mesh
    // 2 non manifold vertex
//    {
//        //Front
//        m.poly_data(0).label = 1;
//        m.poly_data(1).label = 1;
//        //Right
//        m.poly_data(2).label = 2;
//        m.poly_data(3).label = 2;
//        //Back
//        m.poly_data(4).label = 3;
//        m.poly_data(5).label = 3;
//        //Left
//        m.poly_data(6).label = 1;
//        m.poly_data(7).label = 4;
//        //Down
//        m.poly_data(8).label = 1;
//        m.poly_data(9).label = 5;
//        //Up
//        m.poly_data(10).label = 6;
//        m.poly_data(11).label = 6;
//    }

    /// Costume Labelling for Cube_Tet.mesh
    // 1 non manifold vertex
//    {
    m.poly_data(0).label = 1;
    m.poly_data(1).label = 1;
    //Right
    m.poly_data(2).label = 1;
    m.poly_data(3).label = 1;
    //Back
    m.poly_data(4).label = 1;
    m.poly_data(5).label = 1;
    //Left
    m.poly_data(6).label = 1;
    m.poly_data(7).label = 1;
    //Down
    m.poly_data(8).label = 2;
    m.poly_data(9).label = 1;
    //Up
    m.poly_data(10).label = 2;
    m.poly_data(11).label = 1;
//    }

    m.updateGL();
    m.poly_color_wrt_label();

    vector<DrawableSphere> points;
    uint verts = m.num_verts();


    for(uint vid = 0; vid < verts ; ++vid){
        if( !m.vert_is_manifold_cluster(vid)){

            /// Cut without Order !! WRONG NORMALS
            {
                std::vector<uint> e_link = m.adj_v2e(vid);
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

                    for(uint nbr : m.adj_e2e(curr))
                    {
                        // still in the link of vid, but not visited yet
                        if(CONTAINS(edge_set, nbr) && !CONTAINS(visited, nbr))
                        {
                            visited.insert(nbr);
                            m.edge_split(nbr, 0.5);
                            q.push(nbr);
                        }
                    }
                }
                m.edge_split(e_link.front(), 0.5);
                m.updateGL();
            }

            vector<vector<uint>> components = m.vert_poly_cc_by_labels(vid);
            unordered_map<int, int> labels;

            // Calculate Unique Labels
            for(auto comp : components){
                int curr_label = m.poly_data(comp[0]).label;
                auto query = labels.find(curr_label);
                if(query == labels.end()){
                    labels.insert(make_pair(curr_label, 1));
                }else{
                    query->second++;
                }
            }

            // Calculate number of poly of each label
            map<int, int> count_label;
            for (auto comp: components){
                for(auto pid: comp){
                    int curr_label = m.poly_data(pid).label;
                    auto query = count_label.find(curr_label);
                    if(query == count_label.end()){
                        count_label.insert(make_pair(curr_label, 1));
                    }else{
                        query->second++;
                    }
                }
            }

            // Count number of components for each label
            map<int, int> count_comp_label;
            for (auto comp: components){
                int curr_label = m.poly_data(comp[0]).label;
                auto query = count_comp_label.find(curr_label);
                if(query == count_comp_label.end()){
                    count_comp_label.insert(make_pair(curr_label, 1));
                }else{
                    query->second++;
                }
            }

            // Calculate Tunnel Start. CCs index and Label to begin
            int countMax = 0;
            int searched_label = 0;
            for(auto it = count_label.cbegin(); it != count_label.cend(); ++it ) {
                auto query = count_comp_label.find(it->first);
                auto cc_count = query->second;
                if (it->second > countMax && cc_count >= 2){
                    searched_label = it->first;
                    countMax = it->second;
                }
            }

            // Pretty print for Connected Componets
            {
                int i = 0;
                std::cout << "======================================" << std::endl;
                std::cout << "vid: " + std::__cxx11::to_string(vid)  +
                             "  -  Comp #: " + std::__cxx11::to_string(components.size()) +
                             "  -  Labels #: " + std::__cxx11::to_string(labels.size()) << std::endl;

                for (auto comp : components){
                    std::cout << "Component[" + std::__cxx11::to_string(i) + "] pids: " << std::endl;
                    for(auto pid : comp){
                        std::cout << std::__cxx11::to_string(pid) + "\t -> Label: " + std::__cxx11::to_string(m.poly_data(pid).label) << std::endl;
                    }
                    ++i;
                }
            }

            cout << "-----------------" << endl;
            cout << "Label '" << searched_label << "' occurs " << countMax << " times " << endl;

            int max_pid_count=0;
            int index = 0;
            int searched_start_comp = 0;
            for (auto comp: components ){

                if (int(comp.size()) > max_pid_count && m.poly_data(comp[0]).label == searched_label){
                    searched_start_comp = index;
                    max_pid_count = int(comp.size());
                }
                index++;
            }

            cout << "Starting Component Index = " << searched_start_comp << endl;
            cout << "CountMax = " << countMax << endl;
            cout << "======================================" << endl;

            // Exoands around vid searching for shortest route
            vector<uint> start_comp = components[uint(searched_start_comp)];
            vector<uint> contour;
            vector<uint> expand;
            for (auto pid : start_comp){
                for (auto pid2 : m.adj_p2p(pid)){
                    contour.push_back(pid2);
                }
            }
            for (auto pid : contour){
                if (CONTAINS_VEC(m.adj_v2p(vid), pid) && DOES_NOT_CONTAIN_VEC(start_comp, pid)){
                    expand.push_back(pid);
                }
            }

            // PIDs adjacent to starting component
            cout << "-----------------" << endl;
            cout << "PIDs adjacent to starting component" << endl;
            for(auto pid : expand){
                cout << pid << endl;
            }

            stack<vector<uint>> q;
            q.push(components[uint(searched_start_comp)]);

            //Visited contains all pid of poly connected to the first in the cluster
            vector<uint> to_relabel;
            vector<uint> visited;

            uint indexes_path;
            bool found = false;
            while(!found){
                auto curr = q.top();
                q.pop();
                indexes_path = 0;
                for (auto nbr : components){
                    if(nbr != curr){
                        if (m.poly_data(nbr[0]).label != searched_label){
                            q.push(nbr);
                            to_relabel.push_back(indexes_path);
                        }
                        else{
                            found = true;
                        }
                    }
                    indexes_path++;
                }
            }

            cout << "-----------------" << endl;
            cout << "Compo to Relabel" << endl;
            for(auto pid : to_relabel){
                cout << pid << endl;
            }

            index = 0;
            for(auto ind : to_relabel){
                for( auto pid : components[ind]){
                    m.poly_data(pid).label = searched_label;
                }
            }

            components.clear();
        }
//        m.updateGL();
//        m.poly_color_wrt_label();

    }

    m.updateGL();
    m.poly_color_wrt_label();

    verts = m.num_verts();
    for(uint vid = 0; vid < verts ; ++vid){
        if( !m.vert_is_manifold_cluster(vid)){

            //Push sphere to GUI
            DrawableSphere sphere(m.vert(vid));
            points.push_back(sphere);
        }
    }

    GLcanvas gui;
    gui.push_obj(&m);

    // Then push other things!  ORDER MATTERS
    for(uint i = 0; i < points.size() ; ++i){
        points[i].size = 0.5;
        points[i].color = Color(1,0,0,0.5);
        gui.push_obj(&points[i]);
    }

    gui.set_scene_center(m.vert(0), 0.2, false);
    gui.updateGL();

    cout << "Non manifold points number: " + to_string(points.size()) << endl;

    gui.show();
    // CMD+1 to show mesh controls.
    VolumeMeshControlPanel<DrawableTetmesh<>> panel(&m, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
