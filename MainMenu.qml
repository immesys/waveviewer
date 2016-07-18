import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import WaveViewer 1.0

Rectangle {
    color: "#707070"
    /*
     Set default entity from file/QR code
     Load a uri from QR code
     Type a uri

     */
    property bool agentConnected : false
    property bool entityOk : false
    property bool entityChecked : false

    TextField {
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right:parent.right
        anchors.margins: 20
        id:url
        style: TextFieldStyle{}
        onAccepted: {
            WV.loadWavelet(url.text)
        }
    }
    Text {
        anchors.top: url.bottom
        anchors.left: url.left
        anchors.right:url.right
        anchors.margins: 5
        id: stattext
        text: agentConnected ? "agent connected" : (entityOk ? "connecting to agent" : "no available entity");
    }

//    BusyIndicator {
//        id:bi
//        anchors.top:parent.top
//        anchors.bottom:url.top
//       // anchors.left:parent.left
//      //  anchors.right:parent.right
//        anchors.horizontalCenter: parent.horizontalCenter
//        width:100;//(parent.width < parent.height ? parent.width : parent.height)*0.3
//        height:100;//(parent.width < parent.height ? parent.width : parent.height)*0.3
//        running: !agentConnected
//    }
//    Text {
//        anchors.centerIn:bi
//        width:bi.width*0.9
//        height:bi.width*0.9
//        text: "Connecting to BW2 agent"

//        horizontalAlignment: Text.AlignHCenter
//        verticalAlignment: Text.AlignVCenter
//    }


    Rectangle {
        color:"#D8D8D8"
        id: go
        width:100*Screen.devicePixelRatio
        height:100*Screen.devicePixelRatio
        anchors.top:url.bottom
        anchors.right:url.right
        anchors.topMargin: 20
        Image {
            anchors.fill:parent
            source: "qrc:/mainassets/browser.png"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                 WV.loadWavelet(url.text)
            }
        }
    }
    Rectangle {
        color:"#D8D8D8"
        id:setebutton
        width:100*Screen.devicePixelRatio
        height:100*Screen.devicePixelRatio
        anchors.top:url.bottom
        anchors.topMargin: 20
        anchors.right:go.left
        anchors.rightMargin: 20
        Image {
            anchors.fill:parent
            source: "qrc:/mainassets/account.png"
        }
    }
    Rectangle {
        color:"#D8D8D8"
        id:qrcodebutton
        width:100*Screen.devicePixelRatio
        height:100*Screen.devicePixelRatio
        anchors.top:url.bottom
        anchors.right:setebutton.left
        anchors.topMargin: 20
        anchors.rightMargin: 20
        Image {
            anchors.fill:parent
            source: "qrc:/mainassets/camera.png"
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
