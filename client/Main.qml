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
            vehicleModel.updateData(data)
        }
        function onVersionsLoaded(data) {
            versionModel.updateData(data)
            mainStack.push(versionListComponent)
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
            }
        }
    }

    Component {
        id: versionListComponent
        VersionListView {
            onBackRequested: mainStack.pop()
        }
    }
}