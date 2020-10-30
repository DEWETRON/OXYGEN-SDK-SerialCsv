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

    // ListModel holding available ports
    property var availableComPorts: ListModel{}
    property var selectedSerialPortindex: -1

    property string selectedSerialPort: null
    property string selectedBaudRate: "115200"
    property string numChannels: "1"

    function queryProperties(){
        var props = {}

        props["SERIAL_CSV_PLUGIN/SerialPort"] = root.selectedSerialPort
        props["SERIAL_CSV_PLUGIN/BaudRate"] = root.selectedBaudRate
        props["SERIAL_CSV_PLUGIN/NumChannels"] = root.numChannels

        return props;
    }

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
                    var desc = root.availableComPorts.get(currentIndex) ? root.availableComPorts.get(currentIndex).desc : ""
                    if (desc) {
                        selectedSerialPortDesc.text = desc
                        root.selectedSerialPortindex = currentIndex
                        
                        var serial_port = root.availableComPorts.get(currentIndex).text
                        
                        if (serial_port != root.selectedSerialPort) {
                            root.selectedSerialPort = serial_port
                            selectComPort.selectComPort(serial_port, root.selectedBaudRate)
                        }
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
                onCurrentIndexChanged: {
                    //console.log(currentText + " != " + root.selectedBaudRate)
                    if (currentText != root.selectedBaudRate) {
                        root.selectedBaudRate = currentText
                        selectComPort.selectComPort(root.selectedSerialPort, root.selectedBaudRate)
                    }
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
                text: root.numChannels
                readOnly: false
                validator: IntValidator{ bottom: 0; top: 10;}
                onAccepted: {
                    root.numChannels = str(text)
                }
                onActiveFocusChanged: {
                    if (!activeFocus) {
                        root.numChannels = text
                    }
                }
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
            var props = plugin.createPropertyList();
            request(props);
        }
        onResponse: {
            root.availableComPorts.clear()

            for (var i=0; i<value.size; i=i+2) {
                root.availableComPorts.append({ text: value.getString(i), desc: value.getString(i + 1) })
            }

            selectedSerialPort.model = root.availableComPorts
            // TODO This is a workaround so that the first serialport is displayed
            selectedSerialPort.currentIndex = 10
            if (root.selectedSerialPortindex == -1) { 
                root.selectedSerialPortindex = 0
            }

            selectedSerialPort.currentIndex = root.selectedSerialPortindex
        }
    }

    CustomPluginRequest {
        id: selectComPort
        messageId: 2

        function selectComPort(serial_port, baudrate) {
            var props = plugin.createPropertyList();
            props.setString("serialport", serial_port);
            props.setString("baudrate", baudrate);
            request(props);
        }
        onResponse: {
            //console.log("Received response: " + value.getSigned("status"));
        }
    }
}
