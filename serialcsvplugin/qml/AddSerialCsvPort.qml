import QtQuick 2.3
import QtQuick.Controls 1.2 as QtControls
import QtQuick.Layouts 1.1
import Oxygen 1.0
import Oxygen.Dialogs 1.0
import Oxygen.Layouts 1.0
import Oxygen.Themes 1.0
import Oxygen.Tools 1.0
import Oxygen.Widgets 1.0


Item{
    id: root

    property var channels: QObjectTreeModel {}

    // ListModel holding available ports
    property var availableComPorts: ListModel{}
    property var selectedSerialPortindex: -1



    Timer {
        interval: 5000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            queryComPorts.query()
        }
    }

    ColumnLayout{
        anchors.leftMargin: Theme.smallMargin
        anchors.rightMargin: Theme.smallMargin
        anchors.fill: parent

        spacing: Theme.mediumSpacing

        GridLayout{
            columns: 3

            Label{
                text: "Select Serial Port"
            }

            ComboBox{
                id: selectedSerialPort
                model: root.availableComPorts
                textRole: "text"
                currentIndex: 0
                onCurrentIndexChanged: {
                    //console.log(currentIndex)
                    //console.log(root.availableComPorts.get(currentIndex).text)
                    //console.log(root.availableComPorts.get(currentIndex).desc)
                    var desc = root.availableComPorts.get(currentIndex) ? root.availableComPorts.get(currentIndex).desc : ""
                    if (desc) {
                        selectedSerialPortDesc.text = desc
                        root.selectedSerialPortindex = currentIndex
                    }
                   
                }

            }

            Label{
                id: selectedSerialPortDesc
                text: ""
            }



            Label{
                text: "Baudrate"
            }

            ComboBox{
                model: ListModel{
                    ListElement { text: "115200" }
                    ListElement { text: "57600" }
                    ListElement { text: "38400" }
                    ListElement { text: "19200" }
                    ListElement { text: "9600" }
                }
            }
            
            Label{
                text: ""
            }

            Label{
                text: "Number of channels"
            }

            TextField{
                id: idNumChannels
                //Layout.fillWidth: true
                text: "6"
                readOnly: false
            }

            Label{
                text: ""
            }


            VerticalSpacer {}
        }

        VerticalSpacer {}

    }

    CustomPluginRequest {
        id: queryComPorts
        messageId: 1

        function query() {
            //console.log("Query available COM-Ports...");
            var props = plugin.createPropertyList();
            request(props);
        }
        onResponse: {
            root.availableComPorts.clear()

            for (var i=0; i<value.size; i=i+2) {
                // console.log("-> " + value.getString(i))
                // console.log("-> " + value.getString(i+1))
                root.availableComPorts.append({ text: value.getString(i), desc: value.getString(i + 1) })
            }

            selectedSerialPort.model = root.availableComPorts
            // TODO This is a workaround so that the first serialport is displayed
            selectedSerialPort.currentIndex = 10
            if (root.selectedSerialPortindex == -1) { 
                root.selectedSerialPortindex = 0
            }

            selectedSerialPort.currentIndex = root.selectedSerialPortindex
            //console.log("currIndex = " + selectedSerialPort.currentIndex)
        }
    }
}
