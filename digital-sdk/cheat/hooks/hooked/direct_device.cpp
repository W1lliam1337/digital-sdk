#include "../hooks.h"

long __stdcall hooks::hk_present(IDirect3DDevice9* device, RECT* src, RECT* dest, HWND window_override, RGNDATA* dirty_region)
{
	if (!ImGui::GetCurrentContext())
		return og::m_present(device, src, dest, window_override, dirty_region);

	ImGui_ImplDX9_Init(device);
	menu::init_style();

	IDirect3DVertexDeclaration9* vert_declaration;
	IDirect3DVertexShader9* vert_shader;
	DWORD old_d3drs_colorwriteenable;

	device->GetRenderState(D3DRS_COLORWRITEENABLE, &old_d3drs_colorwriteenable);
	device->GetVertexDeclaration(&vert_declaration);
	device->GetVertexShader(&vert_shader);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::IsKeyPressed(VK_INSERT, false))
		ctx::menu_data::m_is_menu_opened = !ctx::menu_data::m_is_menu_opened;

	menu::render();

	ImGui::EndFrame();
	ImGui::Render(render::render_scene());

	if (device->BeginScene() == D3D_OK)
	{
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	device->SetRenderState(D3DRS_COLORWRITEENABLE, old_d3drs_colorwriteenable);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	device->SetVertexDeclaration(vert_declaration);
	device->SetVertexShader(vert_shader);

	return hooks::og::m_present(device, src, dest, window_override, dirty_region);
}

long __stdcall hooks::hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto og = hooks::og::m_reset(device, params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return og;
}