import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    signal backRequested()
    signal versionSelected(int id)

    Button {
        text: "Back"
        onClicked: backRequested()
    }

    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ListView {
            id: versionsList
            anchors.fill: parent
            model: versionModel

            delegate: Rectangle {
                width: ListView.view.width; height: 50
                border.color: "blue"

                Text {
                    anchors.centerIn: parent;
                    text: "Version: " + model.name
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Клік по версії. ID:", model.id);
                        versionSelected(model.id);
                    }
                }
            }
        }

        Text {
            anchors.centerIn: parent
            text: "No versions have been added for this car yet."
            font.pixelSize: 16
            color: "#808080"
            visible: versionsList.count === 0
        }
    }
}