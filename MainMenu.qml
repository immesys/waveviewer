import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import WaveViewer 1.0

Rectangle {
    color: "blue"
    /*
     Set default entity from file/QR code
     Load a uri from QR code
     Type a uri

     */
    property bool agentConnected : false
    property bool entityValid : false
    property bool entityChecked : false

    BusyIndicator {
        id:bi
        anchors.top:parent.top
        anchors.bottom:url.top
       // anchors.left:parent.left
      //  anchors.right:parent.right
        anchors.horizontalCenter: parent.horizontalCenter
        width:100;//(parent.width < parent.height ? parent.width : parent.height)*0.3
        height:100;//(parent.width < parent.height ? parent.width : parent.height)*0.3
        running: !agentConnected
    }
    Text {
        anchors.centerIn:bi
        width:bi.width*0.9
        height:bi.width*0.9
        text: "Connecting to BW2 agent"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    TextField {
        id:url
        width:parent.width-50*Screen.devicePixelRatio
        anchors.centerIn: parent
        style: TextFieldStyle{}
        onAccepted: {
            WV.loadWavelet(url.text)
        }
    }
    Rectangle {
        color:"yellow"
        id: go
        width:100*Screen.devicePixelRatio
        height:100*Screen.devicePixelRatio
        anchors.top:url.bottom
        anchors.right:url.right
        anchors.topMargin: 20
        Text {
            anchors.centerIn: parent
            text: "go"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                 WV.loadWavelet(url.text)
            }
        }
    }
    Rectangle {
        color:"red"
        id:setebutton
        width:100*Screen.devicePixelRatio
        height:100*Screen.devicePixelRatio
        anchors.top:url.bottom
        anchors.topMargin: 20
        anchors.right:go.left
        anchors.rightMargin: 20
        Text {
            anchors.centerIn: parent
            text: "set entity"
        }
    }
    Rectangle {
        color:"green"
        id:qrcodebutton
        width:100*Screen.devicePixelRatio
        height:100*Screen.devicePixelRatio
        anchors.top:url.bottom
        anchors.right:setebutton.left
        anchors.topMargin: 20
        anchors.rightMargin: 20
        Text {
            anchors.centerIn: parent
            text: "Scan QR code"
        }
    }



    //                style: TextFieldStyle{}
    //                id: url
    //                text: "410.dev/michael/s.helloworld/app"
    //                onAccepted: {
    //                    WV.loadWavelet(url.text)
    //                }
    //            }
}
