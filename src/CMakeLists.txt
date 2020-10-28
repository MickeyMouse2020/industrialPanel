message(STATUS "============================================================================")
message(STATUS "below is src directory")

#���ļ�Ŀ¼����VS�ļ���ɸѡ��
function(group_by_dir src_dir)
  foreach(FILE ${ARGN})
    # ��ȡ�ļ�����·��
    get_filename_component(FULL_NAME "${FILE}" ABSOLUTE)

    # ��ȡ�ļ���·��
    get_filename_component(PARENT_DIR "${FULL_NAME}" PATH)

    # �Ƴ���·���е�Դ���·��
    string(REPLACE "${ARGV0}" "" GROUP "${PARENT_DIR}")

    # ȷ��·��ʹ��windows·������
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # ���ļ����鵽 "Source Files" �� "Header Files"

    if("${FILE}" MATCHES ".*\\.h")
      set(GROUP "Header Files${GROUP}")
    else()
      set(GROUP "Source Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
  endforeach()
endfunction(group_by_dir)

set(QT Core Gui Widgets Sql)
find_package(Qt5 REQUIRED ${QT})

file(GLOB_RECURSE SOURCES_FILES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE FORMS "*.ui")
file(GLOB_RECURSE RESOURCES "*.qrc")

set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTOUIC ON)

qt5_wrap_ui(WRAP_FILES ${FORMS})

qt5_add_resources(qrc_FILES ${RESOURCES})

source_group("Generated Files" FILES ${WRAP_FILES} ${qrc_FILES})
source_group("Form Files" FILES ${FORMS})
group_by_dir("${PROJECT_SOURCE_DIR}/src" ${HEADERS})
group_by_dir("${PROJECT_SOURCE_DIR}/src" ${SOURCES_FILES})

# ͷ�ļ�����Ŀ¼
include_directories(${CMAKE_CURRENT_BINARY_DIR}
					${PROJECT_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES_FILES} ${qrc_FILES} ${WRAP_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql)

Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/ DESTINATION ${CMAKE_INSTALL_PREFIX}/data)