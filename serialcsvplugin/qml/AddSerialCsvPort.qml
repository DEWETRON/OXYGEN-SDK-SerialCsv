import QtQuick 2.3
import QtQuick.Controls 1.2 as QtControls
import QtQuick.Layouts 1.1
import Oxygen 1.0
import Oxygen.Dialogs 1.0
import Oxygen.Layouts 1.0
import Oxygen.Themes 1.0
import Oxygen.Tools 1.0
import Oxygen.Widgets 1.0


Item
{
    id: root

    property var channels: QObjectTreeModel {}

    property string filename
    property bool csv_valid: false
    readonly property bool settingsValid: filename !== "" && csv_valid
    property int num_ports: 0

    function queryProperties()
    {
        var props = {}

        props["SERIAL_CSV_PLUGIN/ComPort"] = root.filename;

        return props;
    }

    ColumnLayout
    {
        anchors.leftMargin: Theme.smallMargin
        anchors.rightMargin: Theme.smallMargin
        anchors.fill: parent

        spacing: Theme.mediumSpacing

        Button {
            id: RefreshPortsButton
            text: qsTranslate("ODK_REPLAY_SYNC_SCALAR/AddChannel", "Select CSV file") + Theme.actionEllipsis

            onClicked: {
                queryComPorts.query();
            }
        }

        Label
        {
            text: qsTranslate("SERIAL_CSV_PLUGIN/AddSerialCsvPort", "Selected CSV file") + ":"
        }

        TextField
        {
            id: idInputField
            Layout.fillWidth: true
            text: root.filename
            readOnly: true
            placeholderText: qsTranslate("SERIAL_CSV_PLUGIN/AddSerialCsvPort", "No file selected")
        }

        Label
        {
            text: qsTranslate("SERIAL_CSV_PLUGIN/AddSerialCsvPort", "Not a valid CSV file")
            visible: !root.csv_valid
            color: Theme.error
        }

        VerticalSpacer {}

    }

    CustomPluginRequest {
        id: queryComPorts
        messageId: 1

        function query()
        {
        console.log("Query available COM-Ports...");
            var props = plugin.createPropertyList();
            request(props);
        }
        onResponse:
        {
            console.log("Received response: " + value.getString("5"));

        }
    }
}
