import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 800; height: 600; visible: true
    title: "3D Asset Manager"

    Connections {
        target: assetService
        function onVehiclesLoaded(data) {
            rawVehicleModel.updateData(data)
        }
    }

    StackView {
        id: mainStack
        anchors.fill: parent
        initialItem: vehicleListComponent
    }

    Component {
        id: vehicleListComponent
        VehicleListView {
            onVehicleSelected: (id) => {
                assetService.fetchVersions(id)
                mainStack.push(versionListComponent)
            }
        }
    }

    Component {
        id: versionListComponent
        VersionListView {
            onBackRequested: mainStack.pop()
            onVersionSelected: (id) => {
                assetService.fetchGlobalParts(id)
                mainStack.push(globalPartListComponent)
            }
        }
    }

    Component {
        id: globalPartListComponent
        GlobalPartListView {
            onBackRequested: mainStack.pop()
            onGlobalPartSelected: (id) => {
                assetService.fetchComponents(id)
                mainStack.push(componentListComponent)
            }
        }
    }

    Component {
        id: componentListComponent
        ComponentListView {
            onBackRequested: mainStack.pop()
        }
    }
}