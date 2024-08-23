// -------------------------------------------------
// For Lua
// -------------------------------------------------
void text(crvec2 p, crstr label)
{
	ImGui::SetWindowPos(ImVec2(p.x, p.y));
	ImGui::Text(label.c_str());
}
void textpos(crvec2 p)
{
	ImGui::SetWindowPos(ImVec2(p.x, p.y));
}
void text(crstr label)
{
	ImGui::Text(label.c_str());
}
void draw_hlr()
{
	textpos(vec2(25.8, 25.8));
	text("CAD");
}

// -------------------------------------------------
// For Lua
// -------------------------------------------------
float poly_scaler = 250;
void draw_poly2(int index, ImU32 color )
{
	std::string poly = "poly_" + std::to_string(index);
	if (index == 0)
		poly = "cur";
	else if (index < 0)
	{
		index = GTable::polymap.size() + index;
		poly = "poly_" + std::to_string(index);
	}
	
	if (GTable::polymap.find(poly) == GTable::polymap.end())
	{
		return;
	}

	ImGui::Begin(poly.c_str());
	ImGui::SetWindowSize(ImVec2(glutGet(GLUT_WINDOW_WIDTH) * 0.5, glutGet(GLUT_WINDOW_HEIGHT) * 0.8));
	
	const EPOLY& e = GTable::polymap[poly];
	if (!e.empty())
	{
		ImVec2 center = ImGui::GetCursorScreenPos();			// ÉèÖÃÔ²ÐÄÎª(100, 100)
		ImVec2 imgui_center = ImVec2(ImGui::GetWindowSize().x / 2, ImGui::GetWindowSize().y / 2);
		//glTranslatef(center.x + imgui_center.x, center.y + imgui_center.y, 0.0f);
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		int idx = 0;

		vec3 poly_center;
		for (int i = 0; i < e.size(); i++)
		{
			poly_center += e[i].p * poly_scaler;
		}
		poly_center /= e.size();
		// poly_center += vec3(g_coord.o.x * g_hlr_scale, g_coord.o.y * g_hlr_scale, 0);

		ImVec2 n_center;
		for (int i = 1; i < e.size(); i++)
		{
			real sz = blend(12, 2, i / real(e.size()));

			ImVec2 p0(center.x + e[i - 1].p.x * poly_scaler - poly_center.x + imgui_center.x, center.y + e[i - 1].p.y * poly_scaler - poly_center.y + imgui_center.y);
			ImVec2 p1(center.x + e[i].p.x * poly_scaler - poly_center.x + imgui_center.x, center.y + e[i].p.y * poly_scaler - poly_center.y + imgui_center.y);
			
			if (i == 1)
				drawList->AddCircle(p0,
					sz, IM_COL32(255, 0, 0, 255), 6, 1.0f);	

			drawList->AddCircle(p1,
				sz, color, 6, 1.0f);

			drawList->AddLine(p0, p1,
				IM_COL32(0, 128, 255, 255), 1.0f);
		}
	}
	ImGui::End();
}

void draw_poly3(int index)
{
	std::string poly = "poly_" + std::to_string(index);
	if (index == 0)
		poly = "cur";
	else if (index < 0)
	{
		index = GTable::polymap.size() + index;
		poly = "poly_" + std::to_string(index);
	}
	if (GTable::polymap.find(poly) == GTable::polymap.end())
	{
		return;
	}
	const EPOLY& e = GTable::polymap[poly];
	if (!e.empty())
	{
		PM::smesh_list.clear();
		drawpoly(e);
		PM::setup_sm(CUR_SM, false);

		glutPostRedisplay();
	}
}

void draw_sm(int index)
{
	std::string sm;
	if (index == 0)
		sm = "cur";
	else
	{
		sm = "sm_" + std::to_string(index);
	}

	PM::smesh_list.clear();
	PM::smesh_list.push_back(GTable::submeshmap[sm]);

	for (int i = 0; i < PM::smesh_list.size(); i++)
		if(PM::smesh_list[i])
			PM::setup_sm(*PM::smesh_list[i], false);

	glutPostRedisplay();
}
void refresh_sm()
{
	PM::setup_mesh_data();
	glutPostRedisplay();
}
void refresh()
{
	glutPostRedisplay();
}