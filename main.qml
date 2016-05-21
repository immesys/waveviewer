import QtQuick 2.6
import QtQuick.Controls 1.4
import BOSSWAVE 1.0

import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    width: 1024
    height: 768
    visible:true
    title: "Wavelet viewer"
    toolBar: ToolBar {
        style: ToolBarStyle{}
        RowLayout {
            anchors.fill: parent
            ToolButton {
                Image {
                    source: "qrc:/mainassets/entity.png"
                    anchors.fill: parent
                    anchors.margins: 4
                    mipmap: true
                }
            }
            TextField {
                Layout.fillWidth: true

                style: TextFieldStyle{}
                id: url
                onAccepted: {
                    url.text = bw.loadWavelet(url.text)
                }
            }
            ToolButton {
                Image {
                    source: "qrc:/mainassets/search.png"
                    anchors.fill: parent
                    anchors.margins: 4
                    mipmap: true
                }
                onClicked: {
                    url.text = bw.loadWavelet(url.text)
                }
            }
        }
    }
    /*
    statusBar: StatusBar {
      style: StatusBarStyle{}
      RowLayout {
                anchors.fill: parent
                Label { text: "Read Only" }
            }
    }*/
//    property var gg : Rectangle {
//      color: "red"
//    }
//    function killMain() {
//      if (appContainer.children.length != 0 ){
//        console.log("setting opacity")
//        appContainer.children[0].opacity=0
//      }
//      console.log("killed main")
//    }
    function loadMain (body) {
      console.log("doing loadmain")
      appContainer.children = [body]
      body.anchors.fill = appContainer
    }

    Rectangle {
      anchors.fill: parent
      clip:true
      id: "appContainer"
    }
}
