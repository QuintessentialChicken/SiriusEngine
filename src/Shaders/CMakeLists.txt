add_custom_target(Shaders)

set(HLSL_SHADER_FILES VertexShader.hlsl PixelShader.hlsl TexturePS.hlsl TextureVS.hlsl)

set_source_files_properties(VertexShader.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(PixelShader.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(TextureVS.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(TexturePS.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "5_0")

foreach(FILE ${HLSL_SHADER_FILES})
    message( ${CMAKE_BINARY_DIR})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} ShaderType)
    get_source_file_property(shadermodel ${FILE} ShaderModel)
    add_custom_command(TARGET Shaders
            COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/Main/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/Main/${FILE_WE}.pdb ${FILE}
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
endforeach(FILE)