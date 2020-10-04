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

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char **argv)
{
    using namespace cinolib;
    using namespace std;

    QApplication a(argc, argv);

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/sphere.mesh";
//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/part.mesh";
    // part file download link
    // https://people.sc.fsu.edu/~jburkardt/data/mesh/mesh.html

    DrawableTetmesh<> m(s.c_str());

    m.translate(m.bbox().center());

    m.rotate(vec3d(1,0,0),0.35);

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

    double x,y,z;
    for(uint pid=0; pid < m.num_polys(); ++pid){
        x = m.poly_centroid(pid).x();
        y = m.poly_centroid(pid).y();
        z = m.poly_centroid(pid).z();

        if(x >= 0 && y >= 0 && z >= 0) m.poly_data(pid).color = colors[0];
        if(x >= 0 && y >= 0 && z <= 0) m.poly_data(pid).color = colors[1];
        if(x >= 0 && y <= 0 && z >= 0) m.poly_data(pid).color = colors[2];
        if(x >= 0 && y <= 0 && z <= 0) m.poly_data(pid).color = colors[3];
        if(x <= 0 && y >= 0 && z >= 0) m.poly_data(pid).color = colors[4];
        if(x <= 0 && y >= 0 && z <= 0) m.poly_data(pid).color = colors[5];
        if(x <= 0 && y <= 0 && z >= 0) m.poly_data(pid).color = colors[6];
        if(x <= 0 && y <= 0 && z <= 0) m.poly_data(pid).color = colors[7];

    }
    m.updateGL(); //Always update after transforms on mesh

    vector<DrawableSphere> points;

    // For each color
    /*
    for(uint c = 0; c < colors.size(); ++c){
        // Cycle Over vertices
        for(uint vid = 0; vid < m.num_verts(); ++vid){
            if( !m.vert_is_manifold(vid, colors[c]) ){
                points.push_back(m.vert(vid));
                cout << "Vertex non manifold: " + to_string(vid) << endl;
            }
        }
    }
    */

    // Test for Pastel Orange
    for(uint vid = 0; vid < m.num_verts(); ++vid){
        if( !m.vert_is_manifold(vid, colors[6]) ){
            points.push_back(m.vert(vid));
            cout << "Vertex non manifold: " + to_string(vid) << endl;
        }
    }

////     Poly adjacent to vertex
//    uint vertex = 333;
//    vector<uint> indicent_poly = m.adj_v2p(vertex);
//    for(uint pid : indicent_poly ) {
//        m.poly_data(pid).color = Color::RED();
//    }
//    vector<uint> indicent_edges = m.adj_v2e(vertex);
//    for(uint eid : indicent_edges ) {
//        m.edge_split(eid, 0.5);
//    }

//    indicent_poly = m.adj_v2p(vertex);
//    for(uint pid : indicent_poly ) {
//        m.poly_data(pid).color = Color::BLUE();
//    }

    m.updateGL(); //Always update after transforms on mesh

    GLcanvas gui;
    gui.push_obj(&m);
    // Then push other things!  ORDER MATTERS

    for(uint i = 0; i < points.size() ; ++i){
        points[i].size = 0.2;
        points[i].color = Color(1,0,0,0.5);
        gui.push_obj(&points[i]);
    }
    cout << "Non manifold points number: " + to_string(points.size()) << endl;
    gui.show();

    // CMD+1 to show mesh controls.
    VolumeMeshControlPanel<DrawableTetmesh<>> panel(&m, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
