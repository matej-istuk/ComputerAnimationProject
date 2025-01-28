cmake_minimum_required(VERSION 3.0)
project(lab1)


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(headers/
                    ../dependencies/assimp/include/
                    ../dependencies/glad/include/
                    ../dependencies/glfw/include/
                    ../dependencies/glm/
					../dependencies/stb/)

file(GLOB DEPENDENCY_SOURCES ../dependencies/glad/src/glad.c)
file(GLOB PROJECT_HEADERS headers/*.hpp
                          headers/*.h)
file(GLOB PROJECT_SOURCES sources/*.cpp)
file(GLOB PROJECT_SHADERS shaders/*.comp
                          shaders/*.frag
                          shaders/*.geom
                          shaders/*.vert)
file(GLOB_RECURSE PROJECT_RESOURCES resources/*)
						  


source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Dependencies" FILES ${DEPENDENCY_SOURCES})
source_group("Resources" FILES ${PROJECT_RESOURCES})
set_source_files_properties(${PROJECT_RESOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} 
                               ${DEPENDENCY_SOURCES} ${PROJECT_RESOURCES}
        sources/main.cpp
        headers/model/Renderable.h
        headers/model/RenderVisitor.h
        headers/model/SingleViewportRenderVisitor.h
        sources/model/Transformable.cpp
        headers/model/Transformable.h
        sources/model/Mesh.cpp
        headers/model/Mesh.h
        sources/model/Object.cpp
        headers/model/Object.h
        sources/model/Scene.cpp
        headers/model/Scene.h
        sources/model/Shader.cpp
        headers/model/Shader.h
        sources/exceptions/IOException.cpp
        sources/exceptions/MeshException.cpp
        sources/util/loaders.cpp
        headers/util/loaders.h
        sources/util/FPSManager.cpp
        sources/model/InitializerRenderVisitor.cpp
        sources/model/Camera.cpp
        headers/model/Camera.h
        sources/model/SingleViewportRenderVisitor.cpp
        sources/model/Frustum.cpp
        headers/model/Frustum.h
        sources/model/CoordinateSystem.cpp
        headers/model/CoordinateSystem.h
        sources/model/Curve.cpp
        headers/model/Curve.h
        sources/model/ControlPolygon.cpp
        headers/model/ControlPolygon.h
        headers/model/ControlPointChangeListener.h
        sources/model/BezierCurve.cpp
        headers/model/BezierCurve.h
        sources/model/BCurve.cpp
        headers/model/BCurve.h
        sources/model/ApproximationBezierCurve.cpp
        headers/model/ApproximationBezierCurve.h
        sources/model/InterpolationBezierCurve.cpp
        headers/model/InterpolationBezierCurve.h
        sources/model/Light.cpp
        headers/model/Light.h
        sources/model/ObjectMaterial.cpp
        headers/model/ObjectMaterial.h
        sources/model/SingleViewportRenderVisitor.cpp
        sources/model/ObjectTexture.cpp
        headers/model/ObjectTexture.h
)
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
					  
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    DEPENDS ${PROJECT_SHADERS})
	
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${PROJECT_RESOURCES})
