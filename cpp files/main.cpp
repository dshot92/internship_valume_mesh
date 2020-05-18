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

    //std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/bunny.obj";
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/cup.obj";
    //std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/blub_triangulated.obj";

    DrawableTrimesh<> m(s.c_str());

    // Rotation to obtain a NON-Manifold Verted in the Orange Cluster around the back of the head
    m.translate(m.bbox().center());
    //m.rotate(vec3d(0,1,0),0.001); // for BUNNY

    m.rotate(vec3d(1,0,0),0.39); //for CUP

    //m.rotate(vec3d(1,0,0),0.35); //for blub_triangulated

    m.updateGL(); //Always update after transforms on mesh

    // Subdivision in Cluster into the 8
    std::vector<uint> to_remove;
    double x,y,z;
    for(uint pid=0; pid < m.num_polys(); ++pid){
        x = m.poly_centroid(pid).x();
        y = m.poly_centroid(pid).y();
        z = m.poly_centroid(pid).z();

        if(x >= 0 && y >= 0 && z >= 0) m.poly_data(pid).color = Color::GREEN();
        if(x >= 0 && y >= 0 && z <= 0) m.poly_data(pid).color = Color::PASTEL_YELLOW();
        if(x >= 0 && y <= 0 && z >= 0) m.poly_data(pid).color = Color::YELLOW();
        if(x >= 0 && y <= 0 && z <= 0) m.poly_data(pid).color = Color::BLUE();
        if(x <= 0 && y >= 0 && z >= 0) m.poly_data(pid).color = Color::PASTEL_PINK();
        if(x <= 0 && y >= 0 && z <= 0) {
            m.poly_data(pid).color = Color::PASTEL_CYAN();
        }else{
            /// push back in vector to remove them all at once
            to_remove.push_back(pid);
        }
        if(x <= 0 && y <= 0 && z >= 0) m.poly_data(pid).color = Color::PASTEL_ORANGE();
        if(x <= 0 && y <= 0 && z <= 0) m.poly_data(pid).color = Color::PASTEL_VIOLET();

    }
    /// Test removing non wanted vertexes
    //m.polys_remove(to_remove);
    // END Subdivision in Cluster into the 8

    m.updateGL();

    //Color color = cinolib::Color::PASTEL_CYAN();    // Correct non-manifoldness case (BUNNY)
    Color color = Color::YELLOW();    // Correct non-manifoldness case (CUP)

    uint non_manifold = 1;
    bool non_manifold_found = false;

    for(uint vid = 0; vid < m.num_verts() ; ++vid){

        if( !m.vert_is_manifold(vid, color)){
            non_manifold_found = true;
            non_manifold = vid;     // save vid for Drawable Sphere

            // SPLITTING
            for(auto eid : m.vert_ordered_edges_star(vid)){
                m.edge_split(eid, 0.5); // lambda 0.5 == split edge by the half point
            }

            // POLY STAR RED MARKED
            for(uint pid : m.vert_ordered_polys_star(vid)){
                //m.poly_data(pid).color = Color::RED();
            }

            vector<uint> poly_star = m.vert_ordered_polys_star(vid);

            // Like KMP dublicate pid to simulate a circular space in which to look for occurences
            for(auto pid : m.vert_ordered_polys_star(vid)){
                poly_star.push_back(pid);
            }
            vector<vector<uint>> separated_slices(poly_star.size());
            uint index = 0;

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
            vector<uint> &tunnel = separated_slices.at(0);
            for(auto &vec : separated_slices){
                cout << "Tunnel: " + to_string(tunnel.size())<< endl;
                cout << "Vec: " + to_string(vec.size())<< endl;
                if(vec.size() < tunnel.size() && vec.size() != 0){
                    tunnel = vec;
                }
            }

            // Recolor Tunnel
            cout << "Recoloring PIDs: " + to_string(tunnel.size()) + " polygons"<< endl;
            for(auto pid : tunnel){
                cout << pid << endl;
                m.poly_data(pid).color = color;
            }

        }
    }
    m.updateGL();

    /*
    // Subdivision in Cluster into the 8

    for(uint pid=0; pid < m.num_polys(); ++pid){
        x = m.poly_centroid(pid).x();
        y = m.poly_centroid(pid).y();
        z = m.poly_centroid(pid).z();

        if(x >= 0 && y >= 0 && z >= 0) {
            m.poly_data(pid).color = cinolib::Color::GREEN();
        }
        if(x >= 0 && y >= 0 && z <= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_YELLOW();
        }
        if(x >= 0 && y <= 0 && z >= 0) {
            m.poly_data(pid).color = cinolib::Color::YELLOW();
        }
        if(x >= 0 && y <= 0 && z <= 0) {
            m.poly_data(pid).color = cinolib::Color::BLUE();
        }
        if(x <= 0 && y >= 0 && z >= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_PINK();
        }
        if(x <= 0 && y >= 0 && z <= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_CYAN();
        }
        if(x <= 0 && y <= 0 && z >= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_ORANGE();
        }
        if(x <= 0 && y <= 0 && z <= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_VIOLET();
        }
    }
    m.updateGL();

    for(uint vid = 0; vid < m.num_verts() ; ++vid){

        if( !m.vert_is_manifold(vid, color)){
        //if( false ){
            non_manifold_found = true;
            non_manifold = vid;     // save vid for Drawable Sphere

            // SPLITTING
            for(auto eid : m.vert_ordered_edges_star(vid)){
                m.edge_split(eid, 0.5); // lambda 0.5 == split edge by the half point
            }

            // POLY STAR RED MARKED
            for(auto e : m.vert_ordered_polys_star(vid)){
                //m.poly_data(e).color = Color::RED();
            }
        }
    }
    m.updateGL();

    for(uint pid=0; pid < m.num_polys(); ++pid){
        x = m.poly_centroid(pid).x();
        y = m.poly_centroid(pid).y();
        z = m.poly_centroid(pid).z();

        if(x >= 0 && y >= 0 && z >= 0) {
            m.poly_data(pid).color = cinolib::Color::GREEN();
        }
        if(x >= 0 && y >= 0 && z <= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_YELLOW();
        }
        if(x >= 0 && y <= 0 && z >= 0) {
            m.poly_data(pid).color = cinolib::Color::YELLOW();
        }
        if(x >= 0 && y <= 0 && z <= 0) {
            m.poly_data(pid).color = cinolib::Color::BLUE();
        }
        if(x <= 0 && y >= 0 && z >= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_PINK();
        }
        if(x <= 0 && y >= 0 && z <= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_CYAN();
        }
        if(x <= 0 && y <= 0 && z >= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_ORANGE();
        }
        if(x <= 0 && y <= 0 && z <= 0) {
            m.poly_data(pid).color = cinolib::Color::PASTEL_VIOLET();
        }
    }
    m.updateGL();
    */

    // END Subdivision in Cluster into the 8

    DrawableSphere point(m.vert(non_manifold));
    point.size = 0.2;
    point.color = Color(1,0,0,0.5);  //IF small enough (0.25 is enough) aplha is not necessary

    GLcanvas gui;

    gui.push_obj(&m);       // Push mesh before

    gui.push_obj(&point);   // Then push other things!  ORDER MATTERS

    /*
    if( non_manifold_found ){
        DrawableSphere point(m.vert(non_manifold));
        point.size = 0.25;
        point.color = Color(1,0,0,0.5);  //IF small enough (0.25 is enough) aplha is not necessary
        gui.push_obj(&point);   // Then push other things!  ORDER MATTERS
    }
    */

    gui.show();
    gui.updateGL();

    // CMD+1 to show mesh controls.
    SurfaceMeshControlPanel<DrawableTrimesh<>> panel(&m, &gui);

    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
