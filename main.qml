import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import BOSSWAVE 1.0

Window {
    visible: true

    Button {
        text: "clickme"
        onClicked: {
            console.log("foo");
            BW.publishMsgPack("410.dev/bleedding", "2.0.1.2", {"hi":"bye"}, function(status)
            {
                console.log("status was in qml ", status);
            });

        }
    }
}
