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
#include <cinolib/drawable_sphere.h>
#include <stack>
#include <cinolib/profiler.h>

int main(int argc, char **argv)
{
    using namespace cinolib;
    using namespace std;

    QApplication a(argc, argv);
    string mesh_file;

    /// https://people.sc.fsu.edu/~jburkardt/data/mesh/mesh.html

    // Geogram to Convert trimesh to tetmesh
    /// http://alice.loria.fr/software/geogram/doc/html/index.html

//    mesh_file = "korean_tet.mesh";                    // 587 NON manifold points -> 0 points
//    mesh_file = "blub_triangulated_tet.mesh";         // 176 NON manifold points -> 0 points
//    mesh_file = "cup_tet.mesh";                       // 153 NON manifold points -> 0 points
//    mesh_file = "Laurana_tet.mesh";                   // 150 NON manifold points -> 0 points
//    mesh_file = "bunny_tet.mesh";                     // 137 NON manifold points -> 0 points
//    mesh_file = "maxFace_tet.mesh";                   // 111 NON manifold points -> 0 points
//    mesh_file = "part.mesh";                          //  55 NON manifold points -> 0 points
//    mesh_file = "sphere.mesh";                        //  49 NON manifold points -> 0 points
//    mesh_file = "teapot_tet.mesh";                    //  43 NON manifold points -> 0 points
//    mesh_file = "cubespikes_tet.mesh";                //  39 NON manifold points -> 0 points
//    mesh_file = "spot_triangulated_tet.mesh";         //  32 NON manifold points -> 0 points
//    mesh_file = "p01_tet.mesh";                       //  25 NON manifold points -> 0 points
//    mesh_file = "3holes_tet.mesh";                    //  25 NON manifold points -> 0 points
//    mesh_file = "bamboo_pen_tet.mesh";                //  20 NON manifold points -> 0 points
//    mesh_file = "torus_tet.mesh";                     //  18 NON manifold points -> 0 points
//    mesh_file = "cyl248.mesh";                        //   5 NON manifold points -> 0 points
//    mesh_file = "cube86.mesh";                        //   1 NON manifold points -> 0 points

//    mesh_file = "hexlab2tet/BU_remesh_hex_tet.mesh";  // 284 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/asm001_tet.mesh";         // 101 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/anc101_a1_tet.mesh";      //  99 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/bumpy_torus_tet.mesh";    //  89 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/metatron_tet.mesh";       //  76 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/casting_tet.mesh";        //  71 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/fertility_ref_tet.mesh";  //   1 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/Kiss_hex_coarse_tet.mesh";//   1 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/kiss_hex_tet.mesh";       //   1 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/Stab3_refine3_tet.mesh";  //   1 NON manifold points -> 0 points



    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/" + mesh_file;
    cout << mesh_file << endl;


    /* Select to fix mesh*/
    /////////////////////////
    bool fix = true;
    /////////////////////////

    DrawableTetmesh<> m(s.c_str());
    m.translate(-m.centroid());

    // Labelling Around World Origin
//    double x,y,z;
//    for(uint pid=0; pid < m.num_polys(); ++pid){
//        x = m.poly_centroid(pid).x();
//        y = m.poly_centroid(pid).y();
//        z = m.poly_centroid(pid).z();

//        if(x >= 0 && y >= 0 && z >= 0) m.poly_data(pid).label = 0;
//        if(x >= 0 && y >= 0 && z <= 0) m.poly_data(pid).label = 1;
//        if(x >= 0 && y <= 0 && z >= 0) m.poly_data(pid).label = 2;
//        if(x >= 0 && y <= 0 && z <= 0) m.poly_data(pid).label = 3;
//        if(x <= 0 && y >= 0 && z >= 0) m.poly_data(pid).label = 4;
//        if(x <= 0 && y >= 0 && z <= 0) m.poly_data(pid).label = 5;
//        if(x <= 0 && y <= 0 && z >= 0) m.poly_data(pid).label = 6;
//        if(x <= 0 && y <= 0 && z <= 0) m.poly_data(pid).label = 7;
//    }

    /*** Labelling Quality of PIDs mapped into (0-7) ***/
    double min = 10000000;
    double max = 0;
    for(uint pid = 0; pid < m.num_polys(); ++pid){
        double q = double(m.poly_data(pid).quality);
        min = (min < q) ? min : q;
        max = (max > q) ? max : q;
    }
    cout << "Min poly quality : " << min << endl;
    cout << "Man poly quality : " << max << endl;

    for(uint pid = 0; pid < m.num_polys(); ++pid){
        double q = double(m.poly_data(pid).quality);
        m.poly_data(pid).label = int((q-min)/(max-min) * (7-0) + 0);
    }

    vector<DrawableSphere> points;
    set<uint> edges_list;
    set<uint> fixed_vids;

    uint verts = m.num_verts();
    uint original_num_verts = verts;
    int non_manifold_vid_before = 0;
    for(uint vid = 0; vid < verts ; ++vid){
        if( !m.vert_is_manifold_cluster(vid)){
            non_manifold_vid_before++;
        }
    }

    // for testing
    if ( !fix ) verts=0;

    uint v0, v1;
    set<uint> poly_edges;
    for(uint mesh_vid = 0; mesh_vid < verts ; ++mesh_vid){

//        cout << mesh_vid << " of " << verts << endl;

        poly_edges.clear();
        for(auto v : m.adj_v2e(mesh_vid)){
            poly_edges.insert(v);
        }

        for(auto eid : poly_edges){
            v0 = m.edge_vert_id(eid, 0);
            v1 = m.edge_vert_id(eid, 1);

            // Case EDGE non manifold
            if ( !m.vert_is_manifold_cluster(v0) &&
                 !m.vert_is_manifold_cluster(v1) &&
                 DOES_NOT_CONTAIN(edges_list, eid) &&
                 DOES_NOT_CONTAIN(fixed_vids, v0) &&
                 DOES_NOT_CONTAIN(fixed_vids, v1)
                 ){

                edges_list.insert(eid);

//                fixed_vids.insert(v0);
//                fixed_vids.insert(v1);
                m.edge_label_manifold_fix(eid);
            }
            else{

                // Cases for single Vertex of the edge non manifold
                if ( !m.vert_is_manifold_cluster(v0) &&
                     DOES_NOT_CONTAIN(fixed_vids, v0)){

                    fixed_vids.insert(v0);

                    m.vid_label_manifold_fix(v0);
                    m.edge_split(eid);
                }

                if ( !m.vert_is_manifold_cluster(v1) &&
                     DOES_NOT_CONTAIN(fixed_vids, v1)){

                    fixed_vids.insert(v1);

                    m.vid_label_manifold_fix(v1);
                }
            }
        }

        // Update mesh to add the new vid to be checked
        verts = m.num_verts();
    }

    m.update_normals();
    m.poly_color_wrt_label();
    m.updateGL();

    // Recalculare Non manifold points
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

    // Then push other things!  ORDER MATTERS
    for(uint i = 0; i < points.size() ; ++i){
        points[i].size = 0.5;
        points[i].color = Color(1,0,0,0.5);
        gui.push_obj(&points[i]);
    }

    // PRINT SUMMARY
    verts=m.num_verts();
    cout << "|===========================================|" << endl;
    if ( fix ){
        cout << "VIDs NON manifold Before :\t" << non_manifold_vid_before << endl;
        cout << "VIDs NON manifold After  :\t" << points.size() << endl;
    }
    else{
        cout << "VIDs NON manifold :\t" << non_manifold_vid_before << endl;
    }
    cout << "|===========================================|" << endl;
    cout << "Original Mesh Verts:\t" << original_num_verts << endl;
    cout << "   Fixed Mesh Verts:\t" << verts << endl;
    cout << "        Added Verts:\t" << verts - original_num_verts << endl;

//    gui.set_scene_center(m.vert(0), 0.5, false);

    // GUI pick VID -> components -> labels
    gui.push_marker(vec2i(10, gui.height()-20), "CMD + click to select a vertex", Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, gui.height()-40), "SHIFT + click to select an edge", Color::BLACK(), 12, 0);

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
                cout << ( manifold0 && manifold1 ? "Manifold - FALSE" : "Manifold - TRUE") << endl;

                m.edge_label_manifold_fix(eid);

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
                        points[i].size = 0.5;
                        points[i].color = Color(1,0,0,0.5);
                        gui.push_obj(&points[i]);
                    }

                    cout << ((points.size() > 0) ? ANSI_fg_color_red : ANSI_fg_color_default) << "VIDs NON manifold after  : " << points.size() << ANSI_fg_color_default << endl;

                    m.update_normals();
                    m.poly_color_wrt_label();
                    m.updateGL();
                    c->updateGL();
                }
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
                cout << ( manifold ? "Manifold - FALSE" : "Manifold - TRUE") << endl;

                m.vid_label_manifold_fix(vid);

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
                        points[i].size = 0.5;
                        points[i].color = Color(1,0,0,0.5);
                        gui.push_obj(&points[i]);
                    }

                    cout << ((points.size() > 0) ? ANSI_fg_color_red : ANSI_fg_color_default) << "VIDs NON manifold after  : " << points.size() << ANSI_fg_color_default << endl;

                    m.update_normals();
                    m.poly_color_wrt_label();
                    m.updateGL();
                    c->updateGL();
                }
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
