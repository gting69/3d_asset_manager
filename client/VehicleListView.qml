import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    signal vehicleSelected(int id)

    Button {
        text: "Load auto"
        Layout.alignment: Qt.AlignHCenter
        onClicked: assetService.fetchVehicles()
    }

    ListView {
        Layout.fillWidth: true; Layout.fillHeight: true
        model: vehicleModel
        delegate: Rectangle {
            width: ListView.view.width; height: 50
            border.color: "gray"
            Text { anchors.centerIn: parent; text: "Auto: " + name + " (ID: " + id + ")" }
            MouseArea {
                anchors.fill: parent
                onClicked: vehicleSelected(id)
            }
        }
    }
}