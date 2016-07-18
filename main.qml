import QtQuick 2.6
import QtQuick.Controls 1.4
import BOSSWAVE 1.0
import WaveViewer 1.0

import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    width: 1024
    height: 768
    visible:true
    title: "Wavelet viewer"
    id: appwin
    property bool agentConnected : false
    property bool entityOk : false
//    toolBar: ToolBar {
//        id: "maintb"
//        style: ToolBarStyle{}
//        RowLayout {
//            anchors.fill: parent
//            ToolButton {
//                Image {
//                    source: "qrc:/mainassets/entity.png"
//                    anchors.fill: parent
//                    anchors.margins: 4
//                    mipmap: true
//                }
//            }
//            TextField {
//                Layout.fillWidth: true

//                style: TextFieldStyle{}
//                id: url
//                text: "410.dev/michael/s.helloworld/app"
//                onAccepted: {
//                    WV.loadWavelet(url.text)
//                }
//            }
//            ToolButton {
//                Image {
//                    source: "qrc:/mainassets/search.png"
//                    anchors.fill: parent
//                    anchors.margins: 4
//                    mipmap: true
//                }
//                onClicked: {
//                   WV.loadWavelet(url.text)
//                }
//            }
//        }
//    }

    MainMenu {
        id: mainmenu
        anchors.fill: parent
        clip:true
        visible:true
        agentConnected: appwin.agentConnected
        entityOk: appwin.entityOk
        z: 1
    }
    Rectangle {
         anchors.fill: parent
         clip:true
         id: appContainer
         visible:false
         z: 2
    }
    function setState(entok,agentconn) {
        agentConnected = agentconn
        entityOk = entok
        mainmenu.entityChecked = true
        console.log("ss:",agentconn, entok);
    }

    function loadMain (body) {
      appContainer.children = [body]
      appContainer.visible = true
      body.anchors.fill = appContainer
     // maintb.visible = false
    }

//    function setthing(st) {
//        url.text = st;
//    }




//    Rectangle {
//        anchors.fill: parent
//             clip:true
//             id: "appContainer"
//             Label {
//                 id:thelabel
//                 text:"unset"
//             }
//    }
}
