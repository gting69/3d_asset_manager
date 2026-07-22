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

    // --- НОВИЙ ЕЛЕМЕНТ: РЯДОК ПОШУКУ ---
    TextField {
        Layout.fillWidth: true
        Layout.margins: 10
        placeholderText: "Search auto..."
        font.pixelSize: 16

        // Коли текст змінюється, викликаємо метод C++ для фільтрації
        onTextChanged: {
            vehicleModel.setSearchQuery(text)
        }
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