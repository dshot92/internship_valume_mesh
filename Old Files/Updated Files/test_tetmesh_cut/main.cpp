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
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/cube_tet_multiple.mesh";
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/cube_tet_2_labels.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/one_tet.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/two_tet.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/three_tet.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/four_tet.mesh";

    DrawableTetmesh<> m(s.c_str());

    uint polys = m.num_polys();
//    m.poly_data(10).label = 1;
    m.poly_color_wrt_label();

    vector<DrawableSphere> points;

    vector<uint> poly_2_remove;
    uint vid_non_manifold = 8;
    for(uint pid = 0; pid < polys; ++pid){

        if (m.poly_data(pid).label == 2){

            poly_2_remove.push_back(pid);

            int pid_label = m.poly_data(pid).label;
            cout << pid << endl;

            uint offset;
            for (offset = 0; offset< 4; ++offset){
                uint vid = m.poly_vert_id(pid,offset);
                if (vid == vid_non_manifold){
                    cout << "poly offset = " << offset << endl;
                    break;
                }
            }

            if(offset == 0){

                uint start_vid = m.poly_vert_id(pid,offset);
                vector<uint> edge_star;
                for(auto eid: m.adj_v2e(vid_non_manifold)){
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

                test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
                x = (m.edge_verts(test)[0].x() + m.edge_verts(test)[1].x()) / 2;
                y = (m.edge_verts(test)[0].y() + m.edge_verts(test)[1].y()) / 2;
                z = (m.edge_verts(test)[0].z() + m.edge_verts(test)[1].z()) / 2;
                vec3d v6(x,y,z);
                uint id_v6 = m.vert_add(v6);

//                uint id_v0 = m.poly_vert_id(pid,0);
                uint id_v1 = m.poly_vert_id(pid,1);
                uint id_v2 = m.poly_vert_id(pid,2);
                uint id_v3 = m.poly_vert_id(pid,3);

                uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
                uint new_poly_2 = m.poly_add({id_v1, id_v2, id_v3, id_v4 });
                uint new_poly_3 = m.poly_add({id_v2, id_v3, id_v4, id_v5 });
                uint new_poly_4 = m.poly_add({id_v3, id_v4, id_v5, id_v6});

                m.poly_data(new_poly_1).label = pid_label;
                m.poly_data(new_poly_2).label = pid_label;
                m.poly_data(new_poly_3).label = pid_label;
                m.poly_data(new_poly_4).label = pid_label;

            }

            if(offset == 1){
                uint start_vid = m.poly_vert_id(pid,offset);
                vector<uint> edge_star;
                for(auto eid: m.adj_v2e(vid_non_manifold)){
                    if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
                        edge_star.push_back(eid);
                    }
                }

                double x, y, z;
                uint test;

                test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,0));
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

                test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
                x = (m.edge_verts(test)[0].x() + m.edge_verts(test)[1].x()) / 2;
                y = (m.edge_verts(test)[0].y() + m.edge_verts(test)[1].y()) / 2;
                z = (m.edge_verts(test)[0].z() + m.edge_verts(test)[1].z()) / 2;
                vec3d v6(x,y,z);
                uint id_v6 = m.vert_add(v6);

                uint id_v0 = m.poly_vert_id(pid,0);
//                uint id_v1 = m.poly_vert_id(pid,1);
                uint id_v2 = m.poly_vert_id(pid,2);
                uint id_v3 = m.poly_vert_id(pid,3);

                uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
                uint new_poly_2 = m.poly_add({id_v0, id_v2, id_v3, id_v4 });
                uint new_poly_3 = m.poly_add({id_v2, id_v3, id_v4, id_v5 });
                uint new_poly_4 = m.poly_add({id_v3, id_v4, id_v5, id_v6});

                m.poly_data(new_poly_1).label = pid_label;
                m.poly_data(new_poly_2).label = pid_label;
                m.poly_data(new_poly_3).label = pid_label;
                m.poly_data(new_poly_4).label = pid_label;
            }

            if(offset == 2){
                uint start_vid = m.poly_vert_id(pid,offset);
                vector<uint> edge_star;
                for(auto eid: m.adj_v2e(vid_non_manifold)){
                    if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
                        edge_star.push_back(eid);
                    }
                }

                double x, y, z;
                uint test;

                test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,0));
                x = (m.edge_verts(test)[0].x() + m.edge_verts(test)[1].x()) / 2;
                y = (m.edge_verts(test)[0].y() + m.edge_verts(test)[1].y()) / 2;
                z = (m.edge_verts(test)[0].z() + m.edge_verts(test)[1].z()) / 2;
                vec3d v4(x,y,z);
                uint id_v4 = m.vert_add(v4);

                test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,1));
                x = (m.edge_verts(test)[0].x() + m.edge_verts(test)[1].x()) / 2;
                y = (m.edge_verts(test)[0].y() + m.edge_verts(test)[1].y()) / 2;
                z = (m.edge_verts(test)[0].z() + m.edge_verts(test)[1].z()) / 2;
                vec3d v5(x,y,z);
                uint id_v5 = m.vert_add(v5);

                test = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
                x = (m.edge_verts(test)[0].x() + m.edge_verts(test)[1].x()) / 2;
                y = (m.edge_verts(test)[0].y() + m.edge_verts(test)[1].y()) / 2;
                z = (m.edge_verts(test)[0].z() + m.edge_verts(test)[1].z()) / 2;
                vec3d v6(x,y,z);
                uint id_v6 = m.vert_add(v6);

                uint id_v0 = m.poly_vert_id(pid,0);
                uint id_v1 = m.poly_vert_id(pid,1);
//                uint id_v2 = m.poly_vert_id(pid,2);
                uint id_v3 = m.poly_vert_id(pid,3);

                uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
                uint new_poly_2 = m.poly_add({id_v0, id_v1, id_v4, id_v5 });
                uint new_poly_3 = m.poly_add({id_v1, id_v4, id_v5, id_v6 });
                uint new_poly_4 = m.poly_add({id_v0, id_v1, id_v3, id_v6});

                m.poly_data(new_poly_1).label = pid_label;
                m.poly_data(new_poly_2).label = pid_label;
                m.poly_data(new_poly_3).label = pid_label;
                m.poly_data(new_poly_4).label = pid_label;
            }

            if(offset == 3){
                uint start_vid = m.poly_vert_id(pid,offset);
                vector<uint> edge_star;
                for(auto eid: m.adj_v2e(vid_non_manifold)){
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
                uint new_poly_2 = m.poly_add({id_v0, id_v1, id_v4, id_v5 });
                uint new_poly_3 = m.poly_add({id_v0, id_v4, id_v5, id_v6 });
                uint new_poly_4 = m.poly_add({id_v0, id_v1, id_v2, id_v5});

                m.poly_data(new_poly_1).label = pid_label;
                m.poly_data(new_poly_2).label = pid_label;
                m.poly_data(new_poly_3).label = pid_label;
                m.poly_data(new_poly_4).label = pid_label;
            }
        }
    }

    for(auto pid: poly_2_remove){
        m.poly_remove(pid);
    }

//    m.update_normals();
    m.poly_color_wrt_label();
    m.updateGL();


    GLcanvas gui;
    gui.push_obj(&m);

    for(uint i = 0; i < points.size() ; ++i){
        points[i].size = 0.5;
        points[i].color = Color(1,0,0,0.5);
        gui.push_obj(&points[i]);
    }

    gui.updateGL();
    gui.show();
    // CMD+1 to show mesh controls.
    VolumeMeshControlPanel<DrawableTetmesh<>> panel(&m, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});
    return a.exec();
}
