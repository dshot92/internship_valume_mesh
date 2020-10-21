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
#include <cinolib/profiler.h>

int main(int argc, char **argv)
{
    using namespace cinolib;
    using namespace std;

    QApplication a(argc, argv);
    string mesh_file;

    // Graphite to Convert trimesh to tetmesh
    /// http://alice.loria.fr/software/geogram/doc/html/index.html

//    mesh_file = "korean_tet.mesh";                    // 2120 NON manifold points -> 0 points
//    mesh_file = "blub_triangulated_tet.mesh";         //  344 NON manifold points -> 0 points
//    mesh_file = "cup_tet.mesh";                       //  332 NON manifold points -> 0 points
//    mesh_file = "Laurana_tet.mesh";                   //  553 NON manifold points -> 0 points
//    mesh_file = "bunny_tet.mesh";                     //  585 NON manifold points -> 0 points
//    mesh_file = "maxFace_tet.mesh";                   //  403 NON manifold points -> 0 points
//    mesh_file = "part.mesh";                          //  320 NON manifold points -> 0 points
//    mesh_file = "sphere.mesh";                        //  162 NON manifold points -> 0 points
//    mesh_file = "teapot_tet.mesh";                    //  200 NON manifold points -> 0 points
//    mesh_file = "cubespikes_tet.mesh";                //  194 NON manifold points -> 0 points
//    mesh_file = "spot_triangulated_tet.mesh";         //  195 NON manifold points -> 0 points
//    mesh_file = "p01_tet.mesh";                       //   41 NON manifold points -> 0 points
//    mesh_file = "3holes_tet.mesh";                    //   25 NON manifold points -> 0 points
//    mesh_file = "bamboo_pen_tet.mesh";                //   74 NON manifold points -> 0 points
//    mesh_file = "torus_tet.mesh";                     //  186 NON manifold points -> 0 points
//    mesh_file = "cyl248.mesh";                        //   42 NON manifold points -> 0 points
//    mesh_file = "cube86.mesh";                        //    2 NON manifold points -> 0 points

    /// Hexlab conversion to tetmesh
//    mesh_file = "hexlab2tet/BU_remesh_hex_tet.mesh";  //  284 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/asm001_tet.mesh";         //  101 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/anc101_a1_tet.mesh";      //   99 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/bumpy_torus_tet.mesh";    //   89 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/metatron_tet.mesh";       //   76 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/casting_tet.mesh";        //   71 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/fertility_ref_tet.mesh";  //    1 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/Kiss_hex_coarse_tet.mesh";//    1 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/kiss_hex_tet.mesh";       //    1 NON manifold points -> 0 points
//    mesh_file = "hexlab2tet/Stab3_refine3_tet.mesh";  //    1 NON manifold points -> 0 points

    ///Testing Giammi
    mesh_file = "testing/dino.mesh";                  //  531 NON manifold points -> 0 points
//    mesh_file = "testing/kiss.mesh";                  //  517 NON manifold points -> 0 points
//    mesh_file = "testing/buste.mesh";                 //  351 NON manifold points -> 0 points
//    mesh_file = "testing/armadillo.mesh";             //  256 NON manifold points -> 0 points
//    mesh_file = "testing/teapot.mesh";                //  200 NON manifold points -> 0 points
//    mesh_file = "testing/dog.mesh";                   //  128 NON manifold points -> 0 points
//    mesh_file = "testing/blech.mesh";                 //   24 NON manifold points -> 0 points


    std::string s = (argc==2) ? std::string(argv[1]) : std::string(DATA_PATH) + "/mesh/tetmesh/" + mesh_file;
    cout << "\nMesh : " << mesh_file << endl << endl;


    /* Select to fix mesh*/
    /////////////////////////
    bool fix = true;
    /////////////////////////

    DrawableTetmesh<> m(s.c_str());
    m.translate(-m.centroid());
    m.updateGL();

    // Labelling Clusters
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

    vector<DrawableSphere> points;
    set<uint> fixed_vids;
    set<uint> fixed_edges;

    uint verts = m.num_verts();
    uint verts_before = verts;
    uint edges = m.num_edges();
    uint edges_before = edges;
    uint polys = m.num_polys();
    uint polys_before = polys;

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

    // Check every vert of mesh
    // for each vertex get edges and check edge vertices

    for(uint mesh_vid = 0; mesh_vid < verts ; ++mesh_vid){

        // Get edges incident to vid and cycle over them
        poly_edges.clear();
        for(auto v : m.adj_v2e(mesh_vid)){
            poly_edges.insert(v);
        }

        for(auto eid : poly_edges){

            // Verts of eid
            v0 = m.edge_vert_id(eid, 0);
            v1 = m.edge_vert_id(eid, 1);

            // Case EDGE non manifold
            if ( !m.vert_is_manifold_cluster(v0) &&
                 !m.vert_is_manifold_cluster(v1) &&
                 DOES_NOT_CONTAIN(fixed_edges, eid)
                 ){

                // add edge to set of fixed edges
                fixed_edges.insert(eid);

                // fix non manifold eid
                m.edge_label_manifold_fix(eid);
            }
            else{

                // Cases for single Vertex of the edge non manifold
                if ( !m.vert_is_manifold_cluster(v0) &&
                     DOES_NOT_CONTAIN(fixed_vids, v0)){

                    // add it set of fixed vids
                    fixed_vids.insert(v0);

                    // fix non manifold vid
                    m.vid_label_manifold_fix(v0);
                }

                if ( !m.vert_is_manifold_cluster(v1) &&
                     DOES_NOT_CONTAIN(fixed_vids, v1)){

                    // add it set of fixed vids
                    fixed_vids.insert(v1);

                    // fix non manifold vid
                    m.vid_label_manifold_fix(v1);
                }
            }
        }

        // Update mesh to add the new vid to be checked
        verts = m.num_verts();
    }

    // Edge split updated normals only if the new face are on the surface
    // It enough do this once after all vertices are fixed
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
        points[i].size = 0.1;
        points[i].color = Color(1,0,0,0.5);
        gui.push_obj(&points[i]);
    }

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


//    gui.set_scene_center(m.vert(0), 0.5, false);

    // GUI pick VID
    gui.push_marker(vec2i(10, gui.height()-20), "CMD + click to split a vertex", Color::BLACK(), 12, 0);
    gui.push_marker(vec2i(10, gui.height()-40), "SHIFT + click to split an edge", Color::BLACK(), 12, 0);

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
        // Fix non manifold edge
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
                        points[i].size = 0.1;
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

        // Fix non manifold Vid, os cut around manifold ones
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
                bool manifold = m.vert_is_manifold_cluster(vid);
                cout << ( manifold ? "Manifold - FALSE" : "Manifold - TRUE") << endl;

                if(!manifold){
                    m.vid_label_manifold_fix(vid);
                }
                else{
                    {
                        //Cutting
                        std::vector<uint> e_link = m.adj_v2e(vid);
                        std::unordered_set<uint> edge_set(e_link.begin(), e_link.end());

                        std::queue<uint> q;
                        q.push(e_link.front());

                        std::unordered_set<uint> visited;
                        visited.insert(e_link.front());

                        std::unordered_set<uint> new_vids;
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
                                    new_vids.insert(m.edge_split(nbr, 0.5));
                                    q.push(nbr);
                                }
                            }
                        }
                    }
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

                    m.update_normals();
                    m.poly_color_wrt_label();
                    m.updateGL();
                    c->updateGL();
                }
            }
        }

        // Relabel picked Pid with label == 0
        if (e->modifiers() == Qt::AltModifier )
        {
            vec3d p;
            vec2i click(e->x(), e->y());
            if (c->unproject(click, p)) // transform click in a 3d point
            {
                profiler.push("Pick Vertex");
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
