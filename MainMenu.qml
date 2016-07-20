
import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.0
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
        inputMethodHints: Qt.ImhUrlCharactersOnly
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


    Button {
        //color:"#D8D8D8"
        id: go
        width:100
        height:100
        anchors.top:url.bottom
        enabled:url.text.length > 0
        anchors.right:url.right
        anchors.topMargin: 20
        Image {
            anchors.fill:parent
            source: "qrc:/mainassets/browser.png"
        }
        onClicked: {
             WV.loadWavelet(url.text)
        }

    }


    Button {
        id:setebutton
        width:100
        height:100
        anchors.top:url.bottom
        anchors.topMargin: 20
        anchors.right:go.left
        anchors.rightMargin: 20
        Image {
            anchors.fill:parent
            source: "qrc:/mainassets/account.png"
        }
        onClicked: {
            fd.open()
        }
    }
    Button {
        id:qrcodebutton
        width:100
        height:100
        anchors.top:url.bottom
        anchors.right:setebutton.left
        anchors.topMargin: 20
        anchors.rightMargin: 20
        Image {
            anchors.fill:parent
            source: "qrc:/mainassets/camera.png"
        }
    }
   ListView {
       id:recent
       clip:true
        anchors.top:setebutton.bottom
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.bottom:parent.bottom
        anchors.margins: 20
        delegate: Item {
            height:txt.implicitHeight
            width:parent.width
            Rectangle {
                id:recurl
                width:parent.width - parent.height - 4
                height:parent.height
                anchors.left:parent.left
                anchors.top:parent.top
                Text {
                    id: txt
                    anchors.fill:parent
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 20
                    anchors.margins: 10
                    text: modelData
                }
                MouseArea {
                    anchors.fill:parent
                    onClicked: {
                        WV.loadWavelet(modelData)
                    }
                }
            }
            Button {
                width: parent.height
                height:parent.height
                anchors.left:recurl.right
                anchors.leftMargin: 3
                anchors.top:parent.top
                text: "X"
                onClicked: {
                    WV.removeRecentURI(modelData);
                    recent.model = WV.getRecentURIs();
                }
            }



        }
    }
   FileDialog {
       id: fd
       title: "Select entity file"
       onAccepted: {
           WV.setDefaultEntityFile(fd.fileUrl)
       }
   }
   Component.onCompleted: {
        //WV.getRecentURIs();
       recent.model = WV.getRecentURIs();
   }




    //                style: TextFieldStyle{}
    //                id: url
    //                text: "410.dev/michael/s.helloworld/app"
    //                onAccepted: {
    //                    WV.loadWavelet(url.text)
    //                }
    //            }
}
