#include <QApplication>
#include <cinolib/meshes/meshes.h>
#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/drawable_sphere.h>
#include <cinolib/profiler.h>
#include <stack>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char **argv)
{
    using namespace cinolib;
    using namespace std;

    QApplication a(argc, argv);

    /// https://people.sc.fsu.edu/~jburkardt/data/mesh/mesh.html
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/part.mesh";
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/sphere.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/teapot_tet.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/cyl248.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/cube_tet_multiple.mesh"; axis_labels = false;
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/cube_tet_2_labels_edge.mesh"; axis_labels = false;
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/four_tet.mesh"; axis_labels = false
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/cube_tet_2_labels.mesh"; axis_labels = false;
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/cube86.mesh";

//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/p01.mesh"; // Load Problem (pid label to big ?? )
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/korean_tet.mesh"; // Load Problem (too big ??)

    /// Testing Meshes
{
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/one_vertex.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/one_tet.mesh"; axis_labels = false;
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/one_edge.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/one_vertex_seven.mesh";
}


    DrawableTetmesh<> m(s.c_str());

    m.poly_color_wrt_label();

    vector<DrawableSphere> points;
    vector<uint> poly_2_remove;
    vector<uint> vid_non_manifold;

    uint verts = m.num_verts();
    for(uint mesh_vid = 0; mesh_vid < verts; ++mesh_vid){
//        if(mesh_vid == vid_non_manifold){
        if( !m.vert_is_manifold_cluster(mesh_vid) ){

            vid_non_manifold.push_back(mesh_vid);

            vector<vector<uint>> components = m.vert_poly_cc_by_labels(mesh_vid);
            vector<uint> poly_sphere;
            for(auto one : components) for(auto two : one) poly_sphere.push_back(two);

            for(auto pid : poly_sphere){
                if( DOES_NOT_CONTAIN_VEC(poly_2_remove,pid) ){
                    poly_2_remove.push_back(pid);
                    int pid_label = m.poly_data(pid).label;
                    uint offset;
                    for (offset = 0; offset < 4; ++offset){
                        uint vid = m.poly_vert_id(pid,offset);
                        if (vid == mesh_vid){
                            cout << "VID - " << mesh_vid << " @ poly offset = " << offset << endl;
                            break;
                        }
                    }

                    if(offset == 0){
                        uint start_vid = m.poly_vert_id(pid,offset);
                        vector<uint> edge_star;
                        for(auto eid: m.adj_v2e(start_vid)){
                            if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
                                edge_star.push_back(eid);
                            }
                        }
                        double x, y, z;
                        uint eid;

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,1));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v4(x,y,z);
                        uint id_v4 = m.vert_add(v4);

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,2));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v5(x,y,z);
                        uint id_v5 = m.vert_add(v5);

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v6(x,y,z);
                        uint id_v6 = m.vert_add(v6);

                //                uint id_v0 = m.poly_vert_id(pid,0);
                        uint id_v1 = m.poly_vert_id(pid,1);
                        uint id_v2 = m.poly_vert_id(pid,2);
                        uint id_v3 = m.poly_vert_id(pid,3);

                        uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
                        uint new_poly_2 = m.poly_add({    id_v6, id_v4, id_v5, id_v3 });
                        uint new_poly_3 = m.poly_add({    id_v4, id_v1, id_v2, id_v3 });
                        uint new_poly_4 = m.poly_add({    id_v5, id_v4, id_v2, id_v3 });

                        m.poly_data(new_poly_1).label = pid_label;
                        m.poly_data(new_poly_2).label = pid_label;
                        m.poly_data(new_poly_3).label = pid_label;
                        m.poly_data(new_poly_4).label = pid_label;

                    }

                    if(offset == 1){
                        uint start_vid = m.poly_vert_id(pid,offset);
                        vector<uint> edge_star;
                        for(auto eid: m.adj_v2e(start_vid)){
                            if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
                                edge_star.push_back(eid);
                            }
                        }
                        double x, y, z;
                        uint eid;

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,0));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v4(x,y,z);
                        uint id_v4 = m.vert_add(v4);

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,2));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v5(x,y,z);
                        uint id_v5 = m.vert_add(v5);

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v6(x,y,z);
                        uint id_v6 = m.vert_add(v6);

                        uint id_v0 = m.poly_vert_id(pid,0);
                //                uint id_v1 = m.poly_vert_id(pid,1);
                        uint id_v2 = m.poly_vert_id(pid,2);
                        uint id_v3 = m.poly_vert_id(pid,3);

                        uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
                        uint new_poly_2 = m.poly_add({    id_v0, id_v2, id_v3, id_v4 });
                        uint new_poly_3 = m.poly_add({    id_v2, id_v3, id_v4, id_v5 });
                        uint new_poly_4 = m.poly_add({    id_v3, id_v4, id_v5, id_v6});

                        m.poly_data(new_poly_1).label = pid_label;
                        m.poly_data(new_poly_2).label = pid_label;
                        m.poly_data(new_poly_3).label = pid_label;
                        m.poly_data(new_poly_4).label = pid_label;
                    }

                    if(offset == 2){
                        uint start_vid = m.poly_vert_id(pid,offset);
                        vector<uint> edge_star;
                        for(auto eid: m.adj_v2e(start_vid)){
                            if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
                                edge_star.push_back(eid);
                            }
                        }
                        double x, y, z;
                        uint eid;

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,0));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v4(x,y,z);
                        uint id_v4 = m.vert_add(v4);

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,1));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v5(x,y,z);
                        uint id_v5 = m.vert_add(v5);

                        eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
                        x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
                        y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
                        z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
                        vec3d v6(x,y,z);
                        uint id_v6 = m.vert_add(v6);

                        uint id_v0 = m.poly_vert_id(pid,0);
                        uint id_v1 = m.poly_vert_id(pid,1);
                //                uint id_v2 = m.poly_vert_id(pid,2);
                        uint id_v3 = m.poly_vert_id(pid,3);

                        uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
                        uint new_poly_2 = m.poly_add({    id_v0, id_v1, id_v4, id_v6 });
                        uint new_poly_3 = m.poly_add({    id_v1, id_v4, id_v5, id_v6 });
                        uint new_poly_4 = m.poly_add({    id_v0, id_v1, id_v3, id_v6 });

                        m.poly_data(new_poly_1).label = pid_label;
                        m.poly_data(new_poly_2).label = pid_label;
                        m.poly_data(new_poly_3).label = pid_label;
                        m.poly_data(new_poly_4).label = pid_label;
                    }

                    if(offset == 3){
                        uint start_vid = m.poly_vert_id(pid,offset);
                        vector<uint> edge_star;
                        for(auto eid: m.adj_v2e(start_vid)){
                            if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
                                edge_star.push_back(eid);
                            }
                        }
                        double x, y, z;
                        uint test;

                        test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,1));
                        x = (m.edge_verts(test)[0].x() + m.edge_verts(test)[1].x()) / 2;
                        y = (m.edge_verts(test)[0].y() + m.edge_verts(test)[1].y()) / 2;
                        z = (m.edge_verts(test)[0].z() + m.edge_verts(test)[1].z()) / 2;
                        vec3d v4(x,y,z);
                        uint id_v4 = m.vert_add(v4);

                        test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,2));
                        x = (m.edge_verts(test)[0].x() + m.edge_verts(test)[1].x()) / 2;
                        y = (m.edge_verts(test)[0].y() + m.edge_verts(test)[1].y()) / 2;
                        z = (m.edge_verts(test)[0].z() + m.edge_verts(test)[1].z()) / 2;
                        vec3d v5(x,y,z);
                        uint id_v5 = m.vert_add(v5);

                        test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,0));
                        x = (m.edge_verts(test)[0].x() + m.edge_verts(test)[1].x()) / 2;
                        y = (m.edge_verts(test)[0].y() + m.edge_verts(test)[1].y()) / 2;
                        z = (m.edge_verts(test)[0].z() + m.edge_verts(test)[1].z()) / 2;
                        vec3d v6(x,y,z);
                        uint id_v6 = m.vert_add(v6);

                        uint id_v0 = m.poly_vert_id(pid,0);
                        uint id_v1 = m.poly_vert_id(pid,1);
                        uint id_v2 = m.poly_vert_id(pid,2);
                //                uint id_v3 = m.poly_vert_id(pid,3);

                        uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
                        uint new_poly_2 = m.poly_add({    id_v0, id_v1, id_v2, id_v4 });
                        uint new_poly_3 = m.poly_add({    id_v0, id_v2, id_v4, id_v5 });
                        uint new_poly_4 = m.poly_add({    id_v0, id_v4, id_v5, id_v6 });

                        m.poly_data(new_poly_1).label = pid_label;
                        m.poly_data(new_poly_2).label = pid_label;
                        m.poly_data(new_poly_3).label = pid_label;
                        m.poly_data(new_poly_4).label = pid_label;
                    }
                }
            }
            m.poly_color_wrt_label();
            m.updateGL();
        }
    }

    m.polys_remove(poly_2_remove);
    m.poly_color_wrt_label();
    m.updateGL();

    for(auto vid : vid_non_manifold){
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

//        // Pretty print for Connected Componets
//        {
//            int i = 0;
//            std::cout << "======================================" << std::endl;
//            std::cout << "vid: " + std::__cxx11::to_string(vid)  +
//                         "  -  Comp #: " + std::__cxx11::to_string(components.size()) +
//                         "  -  Labels #: " + std::__cxx11::to_string(labels.size()) << std::endl;

//            for (auto comp : components){
//                std::cout << "Component[" + std::__cxx11::to_string(i) + "] pids: " << std::endl;
//                for(auto pid : comp){
//                    std::cout << std::__cxx11::to_string(pid) + "\t -> Label: " + std::__cxx11::to_string(m.poly_data(pid).label) << std::endl;
//                }
//                ++i;
//            }
//        }

//        cout << "-----------------" << endl;
//        cout << "Label '" << searched_label << "' occurs " << countMax << " times " << endl;

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

//        cout << "Starting Component Index = " << searched_start_comp << endl;
//        cout << "CountMax = " << countMax << endl;
//        cout << "======================================" << endl;

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

//        // PIDs adjacent to starting component
//        cout << "-----------------" << endl;
//        cout << "PIDs adjacent to starting component" << endl;
//        for(auto pid : expand){
//            cout << pid << endl;
//        }

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

//        cout << "-----------------" << endl;
//        cout << "Compo to Relabel" << endl;
//        for(auto pid : to_relabel){
//            cout << pid << endl;
//        }

        index = 0;
        for(auto ind : to_relabel){
            for( auto pid : components[ind]){
                m.poly_data(pid).label = searched_label;
            }
        }

        components.clear();
    }

    m.update_normals();
    m.poly_color_wrt_label();
    m.updateGL();

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

    for(uint i = 0; i < points.size() ; ++i){
        points[i].size = 0.8;
        points[i].color = Color(1,0,0,0.5);
        gui.push_obj(&points[i]);
    }

    std::cout << "\nTotal:\nNon Manifold Points: " + to_string(points.size()) << std::endl;

    // GUI pick VID -> components -> labels
    {
        gui.push_marker(vec2i(10, gui.height()-20), "CMD + click to select a vertex", Color::BLACK(), 12, 0);
        Profiler profiler;

        gui.callback_mouse_press = [&](GLcanvas *c, QMouseEvent *e)
        {
            if (e->modifiers() == Qt::ControlModifier)
            {
                vec3d p;
                vec2i click(e->x(), e->y());
                if (c->unproject(click, p)) // transform click in a 3d point
                {
                    profiler.push("Pick Vertex");
                    uint vid = m.pick_vert(p);
                    profiler.pop();
                    m.vert_data(vid).color = Color::RED();
                    m.updateGL();

                    vector<vector<uint>> components = m.vert_poly_cc_by_labels(vid);
                    unordered_map<int, int> labels;

                    // Pretty print for Connected Componets
                    {
                        int i = 0;
                        std::cout << "======================================" << std::endl;
                        string manifold = !m.vert_is_manifold_cluster(vid) ? "FALSE" : "TRUE" ;
                        std::cout << "MANIFOLD - " << manifold << std::endl;

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

                    c->updateGL();
                }
            }
        };
    }

    gui.updateGL();
    gui.show();
    // CMD+1 to show mesh controls.
    VolumeMeshControlPanel<DrawableTetmesh<>> panel(&m, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});
    return a.exec();
}
