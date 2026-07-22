cmake_minimum_required(VERSION 3.16)
project(AssetManagerClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network)

qt_add_executable(appAssetManagerClient
        main.cpp
        assetservice.cpp
        assetservice.h
        vehiclemodel.cpp
        vehiclemodel.h
        versionmodel.cpp
        versionmodel.h
        componentmodel.cpp
        componentmodel.h
        globalpartmodel.cpp
        globalpartmodel.h
        searchproxymodel.cpp
        searchproxymodel.h
)

qt_add_qml_module(appAssetManagerClient
        URI Main
        VERSION 1.0
        QML_FILES
        Main.qml
        VehicleListView.qml
        VersionListView.qml
        GlobalPartListView.qml
        ComponentListView.qml
)

target_link_libraries(appAssetManagerClient PRIVATE
        Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::Network
)