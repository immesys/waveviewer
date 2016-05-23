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
    objectName: "appwin"
    toolBar: ToolBar {
        id: "maintb"
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
                text: "410.dev/michael/s.helloworld/app"
                onAccepted: {
                    WV.loadWavelet(url.text)
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
                   WV.loadWavelet(url.text)
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
    function loadMain (body) {
      appContainer.children = [body]
      body.anchors.fill = appContainer
      maintb.visible = false
    }

    Rectangle {
      anchors.fill: parent
      clip:true
      id: "appContainer"
    }
}
