/**
			在2D空间内编辑模型更加简单
*/
void front_view()
{
	glPushMatrix();
	glScalef(g_hlr_scale.x, g_hlr_scale.y, 1);
	glLineWidth(1.0);
	glPointSize(1.0);

	vec2 mouse = vec2(mouse_x, glutGet(GLUT_WINDOW_HEIGHT) - mouse_y) / g_coord;

	for (auto& feature : GTable::featuremap)
	{
		for (auto& feature_segs : feature.second)
		{
			if (feature_segs.empty())
				continue;

			for (auto& seg : (feature_segs))
			{
				vec2 a_(200 + seg.a.p.x * 180, 400 + seg.a.p.y * 180);
				vec2 b_(200 + seg.b.p.x * 180, 400 + seg.b.p.y * 180);

				{
					// 绘制红色误差圆圈
					glColor3f(1.0, 0.0, 0.0);
					glBegin(GL_LINE_LOOP);
					real r = c_tor_dis_level1 * 180;
					for (int i = 0; i <= 8; i++)
					{
						float angle = i * 2 * PI / 8;
						glVertex2f(a_.x + r * cos(angle), a_.y + r * sin(angle));
					}
					glEnd();
				}

				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_LINES);
				glVertex2f(a_.x, a_.y);
				glVertex2f(b_.x, b_.y);
				glEnd();
			}
		}
	}

	glPopMatrix();
}