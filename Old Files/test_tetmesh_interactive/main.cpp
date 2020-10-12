#include <QApplication>
#include <cinolib/meshes/meshes.h>
#include <cinolib/gui/qt/qt_gui_tools.h>
#include <cinolib/drawable_sphere.h>
#include <stack>
#include <cinolib/profiler.h>

int main(int argc, char **argv)
{
    using namespace cinolib;
    using namespace std;

    QApplication a(argc, argv);

    int axis_labels = true;

//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/sphere.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/part.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_vertex.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_vertex_seven.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_tet.mesh"; axis_labels = false;
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_edge.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/cube_tet_multiple.mesh"; axis_labels = false;
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/cube_tet_2_labels.mesh"; axis_labels = false;
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/cube_tet_2_labels_edge.mesh"; axis_labels = false;
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/four_tet.mesh"; axis_labels = false;

    // part file download link
    // https://people.sc.fsu.edu/~jburkardt/data/mesh/mesh.html

    DrawableTetmesh<> m(s.c_str());
    m.translate(-m.centroid());
    m.scale(0.2);

    m.poly_color_wrt_label();

    /// Labelling Around World Origin
    if (axis_labels){
        std::vector<int> labels(8);
        labels[0] = 0;
        labels[1] = 1;
        labels[2] = 2;
        labels[3] = 3;
        labels[4] = 4;
        labels[5] = 5;
        labels[6] = 6;
        labels[7] = 7;

        double x,y,z;
        for(uint pid=0; pid < m.num_polys(); ++pid){
            x = m.poly_centroid(pid).x();
            y = m.poly_centroid(pid).y();
            z = m.poly_centroid(pid).z();

            if(x >= 0 && y >= 0 && z >= 0) m.poly_data(pid).label = 1;
            if(x >= 0 && y >= 0 && z <= 0) m.poly_data(pid).label = 2;
            if(x >= 0 && y <= 0 && z >= 0) m.poly_data(pid).label = 3;
            if(x >= 0 && y <= 0 && z <= 0) m.poly_data(pid).label = 4;
            if(x <= 0 && y >= 0 && z >= 0) m.poly_data(pid).label = 5;
            if(x <= 0 && y >= 0 && z <= 0) m.poly_data(pid).label = 6;
            if(x <= 0 && y <= 0 && z >= 0) m.poly_data(pid).label = 7;
            if(x <= 0 && y <= 0 && z <= 0) m.poly_data(pid).label = 8;
          }
    }

    vector<DrawableSphere> points;
    uint polys = m.num_polys();
    set<uint> edges_list;
    set<uint> non_manifold_edges_vids;
    set<uint> vid_list;

    for(uint mesh_pid = 0; mesh_pid < polys ; ++mesh_pid){

        uint vid0 = m.poly_vert_id(mesh_pid, 0);
        uint vid1 = m.poly_vert_id(mesh_pid, 1);
        uint vid2 = m.poly_vert_id(mesh_pid, 2);
        uint vid3 = m.poly_vert_id(mesh_pid, 3);

        set<uint> poly_edges;
        poly_edges.insert(uint(m.edge_id(vid0,vid1)));
        poly_edges.insert(uint(m.edge_id(vid0,vid2)));
        poly_edges.insert(uint(m.edge_id(vid0,vid3)));
        poly_edges.insert(uint(m.edge_id(vid1,vid2)));
        poly_edges.insert(uint(m.edge_id(vid1,vid3)));
        poly_edges.insert(uint(m.edge_id(vid2,vid3)));

        for(auto eid : poly_edges){
            uint v0 = m.edge_vert_id(eid, 0);
            uint v1 = m.edge_vert_id(eid, 1);

            if ( !m.vert_is_manifold_cluster(v0) &&
                 !m.vert_is_manifold_cluster(v1) &&
                 DOES_NOT_CONTAIN(edges_list, eid) &&
                 DOES_NOT_CONTAIN(non_manifold_edges_vids, v0) &&
                 DOES_NOT_CONTAIN(non_manifold_edges_vids, v1)
                 ){

                edges_list.insert(eid);

                non_manifold_edges_vids.insert(v0);
                non_manifold_edges_vids.insert(v1);

            }
        }

        for(auto eid : poly_edges){
            if( DOES_NOT_CONTAIN(edges_list, eid) ){

                uint v0 = m.edge_vert_id(eid, 0);
                uint v1 = m.edge_vert_id(eid, 1);

                if ( !m.vert_is_manifold_cluster(v0) &&
                     DOES_NOT_CONTAIN(non_manifold_edges_vids, v0)){

                    vid_list.insert(v0);
                }

                if ( !m.vert_is_manifold_cluster(v1) &&
                     DOES_NOT_CONTAIN(non_manifold_edges_vids, v1)){

                    vid_list.insert(v1);
                }
            }
        }
    }

    m.update_normals();
    m.poly_color_wrt_label();
    m.updateGL();

    {
        uint verts = m.num_verts();
        for(uint vid = 0; vid < verts ; ++vid){
            if( !m.vert_is_manifold_cluster(vid)){
                //Push sphere to GUI
                DrawableSphere sphere(m.vert(vid));
                points.push_back(sphere);
            }
        }
    }
    GLcanvas gui;
    gui.push_obj(&m);

//     Then push other things!  ORDER MATTERS
    for(uint i = 0; i < points.size() ; ++i){
        points[i].size = 0.5;
        points[i].color = Color(1,0,0,0.5);
        gui.push_obj(&points[i]);
    }
    cout << "Non manifold points number: " + to_string(points.size()) << endl;
    gui.set_scene_center(m.vert(0), 0.2, false);

    // GUI pick VID -> components -> labels
    gui.push_marker(vec2i(10, gui.height()-20), "CMD + click to select a vertex", Color::BLACK(), 12, 0);
    Profiler profiler;

    gui.callback_mouse_press = [&](GLcanvas *c, QMouseEvent *e)
    {
        if (e->modifiers() == Qt::ShiftModifier)
        {
            vec3d p;
            vec2i click(e->x(), e->y());
            if (c->unproject(click, p)) // transform click in a 3d point
            {
                profiler.push("Pick Edge");
                uint eid = m.pick_edge(p);
                profiler.pop();
                cout << " ____________________________" << endl;
                cout << "EID - " << eid << endl;
                uint v0 = m.edge_vert_id(eid,0);
                uint v1 = m.edge_vert_id(eid,1);

                bool manifold0 = !m.vert_is_manifold_cluster(v0);
                bool manifold1 = !m.vert_is_manifold_cluster(v1);
                cout << ( manifold0 && manifold1 ? "FALSE" : "TRUE") << endl;

                uint new_vid = m.edge_split(eid, 0.5);

                set<uint> poly2e;

                for(auto pid : m.adj_v2p(new_vid)){
                    poly2e.insert(pid);
                }

                map<int, int> labels;
                // Calculate Unique Labels
                for(auto pid : poly2e){
                    int curr_label = m.poly_data(pid).label;
                    auto query = labels.find(curr_label);
                    if(query == labels.end()){
                        labels.insert(make_pair(curr_label, 1));
                    }else{
                        query->second++;
                    }
                }
                int most_pids_label;
                int most_pids_label_count = 9999999;
                for(auto label : labels ){
                    if(label.second < most_pids_label_count){
                        most_pids_label_count = label.second;
                        most_pids_label = label.first;
                    }
                }

                for(auto pid : m.adj_v2p(new_vid)){
                    m.poly_data(pid).label = most_pids_label;
                }

                m.update_normals();
                m.poly_color_wrt_label();
                m.updateGL();


                for(uint i = 0; i < points.size() ; ++i){
                    gui.pop(&points[i]);
                }
                points.clear();
                uint verts = m.num_verts();
                for(uint vid = 0; vid < verts ; ++vid){
                    if( !m.vert_is_manifold_cluster(vid)){
                        //Push sphere to GUI
                        DrawableSphere sphere(m.vert(vid));
                        points.push_back(sphere);
                    }
                }
                for(uint i = 0; i < points.size() ; ++i){
                    points[i].size = 0.5;
                    points[i].color = Color(1,0,0,0.5);
                    gui.push_obj(&points[i]);
                }
                cout << "Non manifold points number: " + to_string(points.size()) << endl;

                c->updateGL();
            }
        }

        if (e->modifiers() == Qt::ControlModifier )
        {
            vec3d p;
            vec2i click(e->x(), e->y());
            if (c->unproject(click, p)) // transform click in a 3d point
            {
                profiler.push("Pick Vertex");
                uint vid = m.pick_vert(p);
                profiler.pop();

                cout << " ____________________________" << endl;
                cout << "VID - " << vid << endl;
                bool manifold = !m.vert_is_manifold_cluster(vid);
                cout << ( manifold ? "FALSE" : "TRUE") << endl;

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
                }

                vector<vector<uint>> components = m.vert_poly_cc_by_labels(vid);
                unordered_map<int, int> labels;
                // Calculate Unique Labels
                for(auto comp : components){
                    for(auto pid : comp){
                        int curr_label = m.poly_data(pid).label;
                        auto query = labels.find(curr_label);
                        if(query == labels.end()){
                            labels.insert(make_pair(curr_label, 1));
                        }else{
                            query->second++;
                        }
                    }
                }

                int most_pids_label = m.poly_data(components[0].at(0)).label;
                int most_pids_label_count = 0;
                for(auto label : labels ){
                    if(label.second > most_pids_label_count){
                        most_pids_label_count = label.second;
                        most_pids_label = label.first;
                    }
                }

                for(auto pid : m.adj_v2p(vid)){
                    m.poly_data(pid).label = most_pids_label;
                }

                for(uint i = 0; i < points.size() ; ++i){
                    gui.pop(&points[i]);
                }
                points.clear();
                uint verts = m.num_verts();
                for(uint vid = 0; vid < verts ; ++vid){
                    if( !m.vert_is_manifold_cluster(vid)){
                        //Push sphere to GUI
                        DrawableSphere sphere(m.vert(vid));
                        points.push_back(sphere);
                    }
                }
                for(uint i = 0; i < points.size() ; ++i){
                    points[i].size = 0.5;
                    points[i].color = Color(1,0,0,0.5);
                    gui.push_obj(&points[i]);
                }
                cout << "Non manifold points number: " + to_string(points.size()) << endl;

                m.update_normals();
                m.poly_color_wrt_label();
                m.updateGL();
                c->updateGL();
            }
        }
    };

    gui.updateGL();
    gui.show();
    // CMD+1 to show mesh controls.
    VolumeMeshControlPanel<DrawableTetmesh<>> panel(&m, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
