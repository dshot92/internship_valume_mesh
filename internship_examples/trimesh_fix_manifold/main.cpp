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
#include <cinolib/fix_manifold.h>
#include <cinolib/gui/qt/qt_gui_tools.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cinolib/drawable_sphere.h>
#include <cinolib/geometry/vec3.h>
#include <cinolib/profiler.h>


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char **argv)
{
    using namespace std;
    using namespace cinolib;

    QApplication a(argc, argv);
    string mesh;

//    mesh = "/mesh/trimesh/torus.obj";                 // 56 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/blub_triangulated.obj";     // 49 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/casting.off";               // 26 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/maxFace.obj";               // 16 NON manifold points -> 0 points
    mesh = "/mesh/trimesh/twirl.off";                 // 14 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/3holes.obj";                //  7 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/sharp_sphere.off";          //  6 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/spot_triangulated.obj";     //  6 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/bunny.obj";                 //  5 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/cup.obj";                   //  5 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/Laurana.obj";               //  4 NON manifold points -> 0 points

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + mesh;

    DrawableTrimesh<> m(s.c_str());
    m.translate(-m.centroid());
    m.updateGL();

    // Labelling Clusters
    {
        double x1 = m.bbox().delta_x() / 4;
        double y1 = m.bbox().delta_y() / 4;
        double z1 = m.bbox().delta_z() / 4;
        double x,y,z;
        int label_x = 1;
        int label_y = 1;
        int label_z = 1;
        set<int> lab;
        for(uint pid=0; pid < m.num_polys(); ++pid){
            x = m.poly_centroid(pid).x();
            y = m.poly_centroid(pid).y();
            z = m.poly_centroid(pid).z();

            if (            x  < -x1) label_x = 1;
            if (x >= -x1 && x  <   0) label_x = 2;
            if (x >=   0 && x  <  x1) label_x = 3;
            if (            x  >= x1) label_x = 4;

            if (            y  < -y1) label_y = 1;
            if (y >= -y1 && y  <   0) label_y = 2;
            if (y >=   0 && y  <  y1) label_y = 3;
            if (            y >=  y1) label_y = 4;

            if (            z  < -z1) label_z = 1;
            if (z >= -z1 && z  <   0) label_z = 2;
            if (z >=   0 && z  <  z1) label_z = 3;
            if (            z >=  z1) label_z = 4;

            m.poly_data(pid).label = (label_x * 100) + (label_y * 10) + label_z;
            lab.insert(m.poly_data(pid).label);
        }
    }

    auto t = startChrono(); // Start timer

    fix_non_manifold_verts(m);

    double time = stopChrono(t); // end timer and print
    cout << "Fixing Time: " << time << endl;

    // Apply color labels
    m.poly_color_wrt_label();
    m.updateGL();

    GLcanvas gui;
    gui.push_obj(&m);
    gui.show();
    gui.updateGL();

    // CMD+1 to show mesh controls.
    SurfaceMeshControlPanel<DrawableTrimesh<>> panel(&m, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
