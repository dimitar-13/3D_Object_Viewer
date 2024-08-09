set(SHADERS_SOURCE 
${CMAKE_CURRENT_SOURCE_DIR}/ColorShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/ColorSpaceUtilityFunctions.glsl
${CMAKE_CURRENT_SOURCE_DIR}/GridShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/LightShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/MaterialShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/NormalOrientationShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/PostProcessShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/ShaderCommonFunctions.glsl
${CMAKE_CURRENT_SOURCE_DIR}/SingleTextureInspectShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/SkyboxShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/UVShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/WireframePointShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/WireframeShader.glsl
${CMAKE_CURRENT_SOURCE_DIR}/ClearColorMeshShader.glsl
)


target_sources(3D_Object_Viewer PRIVATE ${SHADERS_SOURCE})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
         PREFIX "Shaders"
         FILES ${SHADERS_SOURCE})

add_custom_target(copy_shaders ALL
       COMMAND ${CMAKE_COMMAND} -E make_directory 
		${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Shaders
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${SHADERS_SOURCE}
       ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/Shaders/
    COMMENT "Creating Shaders directory and copying shader files to $<CONFIG> directory."
)

add_dependencies(3D_Object_Viewer copy_shaders)