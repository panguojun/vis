/**					�����������Կ���һ����ת
*					ʹ����ת���Ǻ����Զ�ʵ��
*/
namespace spin_model
{
    void something()
    {
        if (CUR_SM.empty())
        {
            // ������߽綥���б�
           VERLIST vertices = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}};

           // �����׶������б�
           VERLIST hole1 = {{0.2, 0.2, 0}, {0.4, 0.2, 0}, {0.4, 0.4, 0}, {0.2, 0.4, 0}};
           VERLIST hole2 = {{0.6, 0.6, 0}, {0.8, 0.6, 0}, {0.8, 0.8, 0}, {0.6, 0.8, 0}};
           std::vector<VERLIST> holes = {hole1, hole2};
           
	        closedge(vertices);
	        invedge(vertices);

           // �������ǻ��㷨
          //  poly_topo_holes::face_holes(vertices, holes, vec3::UX, vec3::UZ);
            
            PM::setup_sm(CUR_SM);
        }
        PM::gl_draw_sm(CUR_SM);
    }

    // ħ���ձ�
    void drawBeaker()
    {
        glPushMatrix();
      
        auto it = GTable::boolmap.find("showground");
        if (it == GTable::boolmap.end() || it->second)
        {
            drawCoord();

            // light
            drawPoint3_cs(vec3(10.0f, 10.0f, 10.0f), 0.18, vec3(1.0f, 1.0f, 1.0f));

        }

        glScalef(GLOBAL_SCALE, GLOBAL_SCALE, GLOBAL_SCALE);

        if (b_draw_estack)
            draw_estack();

        glPopMatrix();
    }
}