import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ColumnLayout {
    signal backRequested()

    property int currentVersionId: 0

    Button {
        text: "Back"
        onClicked: backRequested()
    }

    FileDialog {
        id: fileDialog
        title: "Select a location to save"
        fileMode: FileDialog.SaveFile
        onAccepted: {
            let path = fileDialog.selectedFile.toString().replace("file:///", "");
            if (path.startsWith("/C:")) {
                path = path.substring(1);
            } else if (path.startsWith("/D:")) {
                path = path.substring(1);
            }

            console.log("Starting ID upload:", currentVersionId, "on the journey:", path);
            assetService.downloadFile(currentVersionId, path);
        }
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
                        console.log("Click on the version. ID obtained from the model:", model.id);
                        currentVersionId = model.id;
                        fileDialog.open();
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