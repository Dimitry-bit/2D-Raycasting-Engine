#include "imgui-SFML.h"

#include "renderer.h"
#include "boundary.h"

boundary_t b_1 = CreateBoundary(300, 100, 300, 300);

void RenderWindow()
{
	rWindow.clear();
	ImGui::SFML::Render(rWindow);

	DrawBoundary(b_1);
	rWindow.display();
}