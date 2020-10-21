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

    mesh = "/mesh/trimesh/torus.obj";                 // 56 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/blub_triangulated.obj";     // 49 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/casting.off";               // 26 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/maxFace.obj";               // 16 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/twirl.off";                 // 14 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/3holes.obj";                //  7 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/sharp_sphere.off";          //  6 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/spot_triangulated.obj";     //  6 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/bunny.obj";                 //  5 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/cup.obj";                   //  5 NON manifold points -> 0 points
//    mesh = "/mesh/trimesh/Laurana.obj";               //  4 NON manifold points -> 0 points

    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + mesh;
    cout << "\nMesh : " << mesh << endl << endl;

    /* Select TRUE to fix mesh*/
    /////////////////////////
    bool fix = true;
    /////////////////////////

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
        cout << "Number of labels : " << lab.size() << endl;
    }

    // Recalculare Non manifold points Before
    uint verts = m.num_verts();
    uint verts_before = verts;
    uint edges = m.num_edges();
    uint edges_before = edges;
    uint polys = m.num_polys();
    uint polys_before = polys;
    int non_manifold_vid_before = 0;
    {
        uint verts = m.num_verts();
        for(uint vid = 0; vid < verts ; ++vid){
            if( !m.vert_is_manifold_cluster(vid)){
                non_manifold_vid_before++;
            }
        }
    }

    // for testing
    if ( !fix ) verts=0;

    // Check every vert of mesh
    for(uint vid = 0; vid < verts ; ++vid){
        if( !m.vert_is_manifold_cluster(vid)){

            // Fix Non manifold vertss
            m.vid_label_manifold_fix(vid);

        }

        // Update number of mesh vertices
        verts = m.num_verts();
    }

    // Apply color labels
    m.poly_color_wrt_label();
    m.updateGL();

    GLcanvas gui;
    // Push mesh before
    gui.push_obj(&m);

    // Drawables sphere on non manifold vertices
    vector<DrawableSphere> points;

    // Recalculare Non manifold points AFTER
    {
        verts = m.num_verts();
        for(uint vid = 0; vid < verts ; ++vid){
            if( !m.vert_is_manifold_cluster(vid)){
                //Push sphere to GUI
                DrawableSphere sphere(m.vert(vid));
                points.push_back(sphere);
            }
        }
        // Then push other things!  ORDER MATTERS
        for(uint i = 0; i < points.size() ; ++i){
            points[i].size = 0.10;
            points[i].color = Color(1,0,0,0.5);
            gui.push_obj(&points[i]);
        }
    }

    // PRINT SUMMARY
    {
        // PRINT SUMMARY
        verts = m.num_verts();
        edges = m.num_edges();
        polys = m.num_polys();
        cout << "|===========================================|" << endl;
        if ( fix ){
            cout << "VIDs NON manifold Before :\t" << non_manifold_vid_before << endl;
            cout << "VIDs NON manifold After  :\t" << points.size() << endl;
        }
        else{
            cout << "VIDs NON manifold :\t" << non_manifold_vid_before << endl;
        }
        cout << "|===========================================|" << endl;
        cout << "Verts Before:\t" << verts_before << endl;
        cout << "Verts  After:\t" << verts << endl;
        cout << "Verts  Added:\t" << verts - verts_before << endl;
        cout << "|-------------------------------------------|" << endl;
        cout << "Edges Before:\t" << edges_before << endl;
        cout << "Edges  After:\t" << edges << endl;
        cout << "Edges  Added:\t" << edges - edges_before << endl;
        cout << "|-------------------------------------------|" << endl;
        cout << "Polys Before:\t" << polys_before << endl;
        cout << "Polys  After:\t" << polys << endl;
        cout << "Polys  Added:\t" << polys - polys_before << endl;
    }

    // GUI Information
    gui.push_marker(vec2i(10, gui.height()-20), "CMD + click to select a vertex", Color::BLACK(), 12, 0);

    gui.push_marker(vec2i(10, 50), "VIDs NON manifold Before : " + to_string( non_manifold_vid_before ), Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, 70), "VIDs NON manifold  After : " + to_string( points.size() ), Color::BLACK(), 12, 0);

    gui.push_marker(vec2i(10,  90), "Verts Before :" + to_string( verts_before ), Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, 110), "Verts  After : " + to_string( verts ), Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, 130), "Verts  Added : " + to_string( verts - verts_before ), Color::BLACK(), 12, 0);

    gui.push_marker(vec2i(10, 150), "Edges Before :" + to_string( edges_before ), Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, 170), "Edges  After : " + to_string( edges ), Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, 190), "Edges  Added : " + to_string( edges - edges_before ), Color::BLACK(), 12, 0);

    gui.push_marker(vec2i(10, 210), "Polys Before :" + to_string( polys_before ), Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, 230), "Polys  After : " + to_string( polys ), Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, 250), "Polys  Added : " + to_string( polys - polys_before ), Color::BLACK(), 12, 0);

    Profiler profiler;
    gui.callback_mouse_press = [&](GLcanvas *c, QMouseEvent *e)
    {
        // Relabel Picked PID with label 0
        if(e->modifiers() == Qt::ShiftModifier){
            vec3d p;
            vec2i click(e->x(), e->y());
            if (c->unproject(click, p)) // transform click in a 3d point
            {
                profiler.push("Pick Poly");
                uint pid = m.pick_poly(p);
                profiler.pop();

                cout << "PID - " << pid << endl;
                m.poly_data(pid).label = 0;


                // Update non manifold points
                {
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
                        points[i].size = 0.1;
                        points[i].color = Color(1,0,0,0.5);
                        gui.push_obj(&points[i]);
                    }
                    cout << ((points.size() > 0) ? ANSI_fg_color_red : ANSI_fg_color_default) << "VIDs NON manifold after  : " << points.size() << ANSI_fg_color_default << endl;
                    m.poly_color_wrt_label();
                    m.updateGL();
                    c->updateGL();
                }
            }
        }

        // Cut Around Vid, and fix it if not manifold
        if (e->modifiers() == Qt::ControlModifier )
        {
            vec3d p;
            vec2i click(e->x(), e->y());
            if (c->unproject(click, p)) // transform click in a 3d point
            {
                profiler.push("Pick Vertex");
                uint vid = m.pick_vert(p);
                profiler.pop();

                cout << "VID - " << vid << endl;
                bool manifold = m.vert_is_manifold_cluster(vid);
                cout << ( !manifold ? "Manifold - FALSE" : "Manifold - TRUE") << endl;

                if(manifold){
                    /// Cut without Order
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
                }
                else{
                    m.vid_label_manifold_fix(vid);
                }

                // Update non manifold points
                {
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
                        points[i].size = 0.1;
                        points[i].color = Color(1,0,0,0.5);
                        gui.push_obj(&points[i]);
                    }
                    cout << ((points.size() > 0) ? ANSI_fg_color_red : ANSI_fg_color_default) << "VIDs NON manifold after  : " << points.size() << ANSI_fg_color_default << endl;
                    m.poly_color_wrt_label();
                    m.updateGL();
                    c->updateGL();
                }
            }
        }

    };

    gui.show();
    gui.updateGL();

    // CMD+1 to show mesh controls.
    SurfaceMeshControlPanel<DrawableTrimesh<>> panel(&m, &gui);
    QApplication::connect(new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_1), &gui), &QShortcut::activated, [&](){panel.show();});

    return a.exec();
}
