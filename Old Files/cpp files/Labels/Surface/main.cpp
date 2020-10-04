/* This is a base application for cinolib (https://github.com/maxicino/cinolib).
 * It will show a GL canvas (and associated control panel) to interact
 * with a triangle mesh.
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

#include <GL/gl.h>
#include <GL/glu.h>
#include <cinolib/drawable_object.h>
#include <cinolib/drawable_sphere.h>
#include <cinolib/gl/draw_sphere.h>
#include <cinolib/geometry/vec3.h>
#include <cinolib/color.h>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char **argv)
{
    using namespace std;
    using namespace cinolib;

    QApplication a(argc, argv);

//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/bunny.obj";
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/cup.obj";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/blub_triangulated.obj";

    DrawableTrimesh<> m(s.c_str());

    // Rotation to obtain a NON-Manifold Verted in the Orange Cluster around the back of the head
    m.translate(m.bbox().center());
//    m.rotate(vec3d(0,1,0),0.001); // for BUNNY

    m.rotate(vec3d(1,0,0),0.39); //for CUP

    m.rotate(vec3d(1,0,0),0.35); //for blub_triangulated

    m.updateGL(); //Always update after transforms on mesh

    // Subdivision in Cluster into the 8
    std::vector<Color> colors(8);
    colors[0] = Color::GREEN();
    colors[1] = Color::PASTEL_YELLOW();
    colors[2] = Color::YELLOW();
    colors[3] = Color::BLUE();
    colors[4] = Color::PASTEL_PINK();
    colors[5] = Color::PASTEL_CYAN();
    colors[6] = Color::PASTEL_ORANGE();
    colors[7] = Color::PASTEL_VIOLET();

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

        if(x >= 0 && y >= 0 && z >= 0) m.poly_data(pid).label = 0;
        if(x >= 0 && y >= 0 && z <= 0) m.poly_data(pid).label = 1;
        if(x >= 0 && y <= 0 && z >= 0) m.poly_data(pid).label = 2;
        if(x >= 0 && y <= 0 && z <= 0) m.poly_data(pid).label = 3;
        if(x <= 0 && y >= 0 && z >= 0) m.poly_data(pid).label = 4;
        if(x <= 0 && y >= 0 && z <= 0) m.poly_data(pid).label = 5;
        if(x <= 0 && y <= 0 && z >= 0) m.poly_data(pid).label = 6;
        if(x <= 0 && y <= 0 && z <= 0) m.poly_data(pid).label = 7;
    }
    /// Test removing non wanted vertexes
    //m.polys_remove(to_remove);
    // END Subdivision in Cluster into the 8

    m.updateGL();

    //Color color = colors[5];   // Correct non-manifoldness case (BUNNY)
    //Color color = colors[2];    // Correct non-manifoldness case (CUP)

    vector<DrawableSphere> points;
    /*
    for(auto color: colors){
        for(uint vid = 0; vid < m.num_verts() ; ++vid){

            if( !m.vert_is_manifold(vid, color)){

                //Push sphere to GUI
                DrawableSphere sphere(m.vert(vid));
                points.push_back(sphere);

                // SPLITTING
                for(auto eid : m.vert_ordered_edges_star(vid)){
                    m.edge_split(eid, 0.5); // lambda 0.5 == split edge by the half point
                }
                m.updateGL();

                vector<uint> poly_star = m.vert_ordered_polys_star(vid);

                // Like KMP dublicate pid to simulate a circular space in which to look for occurences
                for(auto pid : m.vert_ordered_polys_star(vid)){
                    poly_star.push_back(pid);
                }
                vector<vector<uint>> separated_slices(poly_star.size());
                uint index = 0;
                cout << "PolyStar size: " + to_string(poly_star.size()) << endl;

                // POLY STAR RED MARKED
                cout << "PolyStar: " << endl;
                for(uint pid : poly_star){
                    cout << pid  << endl;
                    if(m.poly_data(pid).color != color){
                        separated_slices.at(index).push_back(pid);
                    }
                    if(m.poly_data(pid).color == color)
                        ++index;
                }

                //Biggest Slice -> Tunnel
                vector<uint> &tunnel = poly_star; // Like setting initial size to infinity, to be sure to get che minimum set separated
                int aux = 0;
                for(auto &vec : separated_slices){
                    cout << "Tunnel: " + to_string(aux) + " - Size: " + to_string(vec.size())<< endl;
                    if(vec.size() < tunnel.size() && vec.size() > 0){
                        cout << "Debug" << endl;
                        cout << "Vec_Size: " + to_string(vec.size()) << endl;
                        tunnel = vec;
                    }
                    ++aux;
                }
                cout << "Tunnel  TO RE color: " << endl;
                for(auto pid : tunnel){
                    cout << pid << endl;
                }

                // Recolor Tunnel
                cout << "Recoloring PIDs: " + to_string(tunnel.size()) + " polygons"<< endl;
                for(auto pid : tunnel){
                    cout << pid << endl;
                    m.poly_data(pid).color = color;
                }
            }
        }
    }
    */

    m.poly_color_wrt_label();

    for(auto label : labels){
        for(uint vid = 0; vid < m.num_verts() ; ++vid){
            if( !m.vert_is_manifold_cluster(vid, label)){
                //Push sphere to GUI
                DrawableSphere sphere(m.vert(vid));
                points.push_back(sphere);

                // SPLITTING
                for(auto eid : m.vert_ordered_edges_star(vid)){
                    m.edge_split(eid, 0.5); // lambda 0.5 == split edge by the half point
                }
                m.updateGL();

                vector<uint> poly_star = m.vert_ordered_polys_star(vid);

                // Like KMP dublicate pid to simulate a circular space in which to look for occurences
                for(auto pid : m.vert_ordered_polys_star(vid)){
                    poly_star.push_back(pid);
                }
                vector<vector<uint>> separated_slices(poly_star.size());
                uint index = 0;
                cout << "PolyStar size: " + to_string(poly_star.size()) << endl;

                // POLY STAR RED MARKED
                cout << "PolyStar: " << endl;
                for(uint pid : poly_star){
                    cout << pid  << endl;
                    if(m.poly_data(pid).label != label){
                        separated_slices.at(index).push_back(pid);
                    }
                    if(m.poly_data(pid).label == label)
                        ++index;
                }

                //Biggest Slice -> Tunnel
                vector<uint> &tunnel = poly_star; // Like setting initial size to infinity, to be sure to get che minimum set separated
                int aux = 0;
                for(auto &vec : separated_slices){
                    cout << "Tunnel: " + to_string(aux) + " - Size: " + to_string(vec.size())<< endl;
                    if(vec.size() < tunnel.size() && vec.size() > 0){
                        cout << "Debug" << endl;
                        cout << "Vec_Size: " + to_string(vec.size()) << endl;
                        tunnel = vec;
                    }
                    ++aux;
                }
                cout << "Tunnel  TO RE color: " << endl;
                for(auto pid : tunnel){
                    cout << pid << endl;
                }

                // Recolor Tunnel
                cout << "Recoloring PIDs: " + to_string(tunnel.size()) + " polygons"<< endl;
                for(auto pid : tunnel){
                    cout << pid << endl;
                    m.poly_data(pid).label = label;
                }
            }
        }
    }
    m.poly_color_wrt_label();
    m.updateGL();

    GLcanvas gui;

    // Push mesh before
    gui.push_obj(&m);

    std::cout << "Non Manifold Points: " + to_string(points.size()) << std::endl;
    // Then push other things!  ORDER MATTERS
    for(uint i = 0; i < points.size() ; ++i){
        points[i].size = 0.2;
        points[i].color = Color(1,0,0,0.5);
        gui.push_obj(&points[i]);
    }

    gui.show();
    gui.updateGL();

    // CMD+1 to show mesh controls.
    SurfaceMeshControlPanel<DrawableTrimesh<>> panel(&m, &gui);

    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
