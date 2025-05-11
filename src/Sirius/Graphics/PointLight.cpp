// //
// // Created by Leon on 09/05/2025.
// //
//
// #include "PointLight.h"
//
// #include <External/imgui.h>
//
// PointLight::PointLight(float radius ) {}
//
// void PointLight::SpawnControlWindow() noexcept
// {
//     if( ImGui::Begin( "Light" ) )
//     {
//         ImGui::Text( "Position" );
//         ImGui::SliderFloat( "X",&pos.x,-60.0f,60.0f,"%.1f" );
//         ImGui::SliderFloat( "Y",&pos.y,-60.0f,60.0f,"%.1f" );
//         ImGui::SliderFloat( "Z",&pos.z,-60.0f,60.0f,"%.1f" );
//         if( ImGui::Button( "Reset" ) )
//         {
//             Reset();
//         }
//     }
//     ImGui::End();
// }
//
// void PointLight::Reset() noexcept
// {
//     pos = { 0.0f,0.0f,0.0f };
// }
//
// void PointLight::Draw() const noexcept
// {
//     mesh.SetTransform( pos );
//     mesh.Draw();
// }
//
// void PointLight::Bind() const noexcept
// {
//     cbuf.Update(PointLightCBuf{ pos } );
//     cbuf.Bind();
// }
