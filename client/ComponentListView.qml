import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ColumnLayout {
    signal backRequested()
    property int currentComponentId: 0

    Button {
        text: "Back to Global Parts"
        onClicked: backRequested()
    }

    FileDialog {
        id: fileDialog
        title: "Select a location to save the 3D asset"
        fileMode: FileDialog.SaveFile
        onAccepted: {
            let path = fileDialog.selectedFile.toString().replace("file:///", "");
            if (path.startsWith("/C:")) {
                path = path.substring(1);
            } else if (path.startsWith("/D:")) {
                path = path.substring(1);
            }

            console.log("Starting asset download for Component ID:", currentComponentId, "to:", path);
            assetService.downloadFile(currentComponentId, path);
        }
    }

    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ListView {
            id: componentsList
            anchors.fill: parent
            model: componentModel

            delegate: Rectangle {
                width: ListView.view.width; height: 50
                border.color: "orange"

                Text {
                    anchors.centerIn: parent
                    text: "Component: " + model.name
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Click on Component. ID:", model.id);
                        currentComponentId = model.id;
                        fileDialog.open();
                    }
                }
            }
        }

        Text {
            anchors.centerIn: parent
            text: "No components found."
            font.pixelSize: 16
            color: "#808080"
            visible: componentsList.count === 0
        }
    }
}