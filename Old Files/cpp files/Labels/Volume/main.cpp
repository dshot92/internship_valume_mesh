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

//    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/sphere.mesh";
    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/part.mesh";
    // part file download link
    // https://people.sc.fsu.edu/~jburkardt/data/mesh/mesh.html

    DrawableTetmesh<> m(s.c_str());

    m.translate(m.bbox().center());

    m.rotate(vec3d(1,0,0),0.35);

    m.updateGL(); //Always update after transforms on mesh

    // Subdivision in Cluster into the 8
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
    m.updateGL(); //Always update after transforms on mesh

    m.poly_color_wrt_label();

    vector<DrawableSphere> points;
    std::vector<uint> to_split;

    // Test for Pastel Orange
    for(auto label : labels){

    }

    for(uint vid = 0; vid < m.num_verts() ; ++vid){
        if( !m.vert_is_manifold_cluster(vid, 4)){
            //Push sphere to GUI
            DrawableSphere sphere(m.vert(vid));
            points.push_back(sphere);

            to_split.push_back(vid);
        }
    }

//    for(auto vid : to_split){
//        vector<uint> indicent_edges = m.adj_v2e(vid);
//        for(uint eid : indicent_edges ) {
//            m.edge_split(eid, 0.5);
//        }
//    }

    m.poly_color_wrt_label();




////     Poly adjacent to vertex
//    uint vertex = 333;
//    vector<uint> indicent_poly = m.adj_v2p(vertex);
//    for(uint pid : indicent_poly ) {
//        m.poly_data(pid).color = Color::RED();
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
