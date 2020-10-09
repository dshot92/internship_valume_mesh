if(offset == 0){

    uint start_vid = m.poly_vert_id(pid,offset);
    vector<uint> edge_star;
    for(auto eid: m.adj_v2e(start_vid)){
        if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
            edge_star.push_back(eid);
        }
    }

    double x, y, z;
    uint eid;

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,1));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v4(x,y,z);
    uint id_v4 = m.vert_add(v4);

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,2));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v5(x,y,z);
    uint id_v5 = m.vert_add(v5);

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v6(x,y,z);
    uint id_v6 = m.vert_add(v6);

//                uint id_v0 = m.poly_vert_id(pid,0);
    uint id_v1 = m.poly_vert_id(pid,1);
    uint id_v2 = m.poly_vert_id(pid,2);
    uint id_v3 = m.poly_vert_id(pid,3);

    uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
    uint new_poly_2 = m.poly_add({    id_v6, id_v4, id_v5, id_v3 });
    uint new_poly_3 = m.poly_add({    id_v4, id_v1, id_v2, id_v3 });
    uint new_poly_4 = m.poly_add({    id_v5, id_v4, id_v2, id_v3 });

    m.poly_data(new_poly_1).label = pid_label;
    m.poly_data(new_poly_2).label = pid_label;
    m.poly_data(new_poly_3).label = pid_label;
    m.poly_data(new_poly_4).label = pid_label;

}

if(offset == 1){
    uint start_vid = m.poly_vert_id(pid,offset);
    vector<uint> edge_star;
    for(auto eid: m.adj_v2e(start_vid)){
        if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
            edge_star.push_back(eid);
        }
    }
    double x, y, z;
    uint eid;

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,0));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v4(x,y,z);
    uint id_v4 = m.vert_add(v4);

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,2));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v5(x,y,z);
    uint id_v5 = m.vert_add(v5);

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v6(x,y,z);
    uint id_v6 = m.vert_add(v6);

    uint id_v0 = m.poly_vert_id(pid,0);
//                uint id_v1 = m.poly_vert_id(pid,1);
    uint id_v2 = m.poly_vert_id(pid,2);
    uint id_v3 = m.poly_vert_id(pid,3);

    uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
    uint new_poly_2 = m.poly_add({    id_v0, id_v2, id_v3, id_v4 });
    uint new_poly_3 = m.poly_add({    id_v2, id_v3, id_v4, id_v5 });
    uint new_poly_4 = m.poly_add({    id_v3, id_v4, id_v5, id_v6});

    m.poly_data(new_poly_1).label = pid_label;
    m.poly_data(new_poly_2).label = pid_label;
    m.poly_data(new_poly_3).label = pid_label;
    m.poly_data(new_poly_4).label = pid_label;
}

if(offset == 2){
    uint start_vid = m.poly_vert_id(pid,offset);
    vector<uint> edge_star;
    for(auto eid: m.adj_v2e(start_vid)){
        if(CONTAINS_VEC(m.adj_p2e(pid),eid) && DOES_NOT_CONTAIN_VEC(edge_star, eid)){
            edge_star.push_back(eid);
        }
    }
    double x, y, z;
    uint eid;

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,0));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v4(x,y,z);
    uint id_v4 = m.vert_add(v4);

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,1));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v5(x,y,z);
    uint id_v5 = m.vert_add(v5);

    eid = m.poly_edge_id(pid, m.poly_vert_id(pid,offset), m.poly_vert_id(pid,3));
    x = (m.edge_verts(eid)[0].x() + m.edge_verts(eid)[1].x()) / 2;
    y = (m.edge_verts(eid)[0].y() + m.edge_verts(eid)[1].y()) / 2;
    z = (m.edge_verts(eid)[0].z() + m.edge_verts(eid)[1].z()) / 2;
    vec3d v6(x,y,z);
    uint id_v6 = m.vert_add(v6);

    uint id_v0 = m.poly_vert_id(pid,0);
    uint id_v1 = m.poly_vert_id(pid,1);
//                uint id_v2 = m.poly_vert_id(pid,2);
    uint id_v3 = m.poly_vert_id(pid,3);

    uint new_poly_1 = m.poly_add({start_vid, id_v4, id_v5, id_v6 });
    uint new_poly_2 = m.poly_add({    id_v0, id_v1, id_v4, id_v6 });
    uint new_poly_3 = m.poly_add({    id_v1, id_v4, id_v5, id_v6 });
    uint new_poly_4 = m.poly_add({    id_v0, id_v1, id_v3, id_v6 });

    m.poly_data(new_poly_1).label = pid_label;
    m.poly_data(new_poly_2).label = pid_label;
    m.poly_data(new_poly_3).label = pid_label;
    m.poly_data(new_poly_4).label = pid_label;
}

if(offset == 3){
    uint start_vid = m.poly_vert_id(pid,offset);
    vector<uint> edge_star;
    for(auto eid: m.adj_v2e(start_vid)){
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
    uint new_poly_2 = m.poly_add({    id_v0, id_v1, id_v4, id_v5 });
    uint new_poly_3 = m.poly_add({    id_v0, id_v4, id_v5, id_v6 });
    uint new_poly_4 = m.poly_add({    id_v0, id_v1, id_v2, id_v5 });

    m.poly_data(new_poly_1).label = pid_label;
    m.poly_data(new_poly_2).label = pid_label;
    m.poly_data(new_poly_3).label = pid_label;
    m.poly_data(new_poly_4).label = pid_label;
}
