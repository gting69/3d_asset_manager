import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    signal backRequested()
    signal globalPartSelected(int id)

    Button {
        text: "Back to Versions"
        onClicked: backRequested()
    }

    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ListView {
            id: globalPartsList
            anchors.fill: parent
            model: globalPartModel

            delegate: Rectangle {
                width: ListView.view.width; height: 50
                border.color: "green"

                Text {
                    anchors.centerIn: parent
                    text: "Global Part: " + model.name
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Клік по Global Part. ID:", model.id);
                        globalPartSelected(model.id);
                    }
                }
            }
        }

        Text {
            anchors.centerIn: parent
            text: "No global parts have been added for this version yet."
            font.pixelSize: 16
            color: "#808080"
            visible: globalPartsList.count === 0
        }
    }
}