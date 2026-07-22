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

    // 1. Екран Автомобілів
    Component {
        id: vehicleListComponent
        VehicleListView {
            onVehicleSelected: (id) => {
                assetService.fetchVersions(id)
                mainStack.push(versionListComponent)
            }
        }
    }

    // 2. Екран Версій
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

    // 3. Екран Глобальних Частин
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

    // 4. Екран Компонентів (ФІНАЛ)
    Component {
        id: componentListComponent
        ComponentListView {
            onBackRequested: mainStack.pop()
        }
    }
}