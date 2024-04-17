DirectX:-
The DirectX implementation users HLSL shaders which shall be compiled using DirectX Shader Compiler (DXC).
The latest release of DXC can be found at https://github.com/microsoft/DirectXShaderCompiler/releases 
Set DXC_BASE_PATH to the extracted zip folder for the shaders to be compiled for the example project.
The command being used to compile the hlsl shader is as follows:
`dxc.exe SDK_ProcAmp.hlsl -E main -Fo DirectX_Assets\SDK_ProcAmp.cso -Frs DirectX_Assets\SDK_ProcAmp.rs -T cs_6_5 -enable-16bit-types`

Copy the DirectX_Assets folder from $(PREMSDKBUILDPATH) to [Program Files]\Adobe\Common\Plug-ins\7.0\MediaCore\ for the shaders to be picked up at runtime.