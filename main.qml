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
         visible:true
         z: -1
    }
    function setState(entok,agentconn) {
        agentConnected = agentconn
        entityOk = entok
        mainmenu.entityChecked = true
        console.log("ss:",agentconn, entok);
    }

    function loadMain (body) {
      appContainer.children = [body]
      appContainer.z = 3
      body.anchors.fill = appContainer
    }

}
