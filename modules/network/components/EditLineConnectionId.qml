import QtQuick 2.1
import Deepin.Widgets 1.0

BaseEditLine {
    id: root
    
    rightLoader.sourceComponent: Row {
    DTextInput {
        width: valueWidth
        visible: connectionSession.type != "wired"
        // TODO
        // Connections {
        //     target: root
        //     onWidgetShown: {
        //         text = root.value
        //     }
        //     onValueChanged: {
        //         text = root.value
        //     }
        // }
        Binding on text {
            when: root.value != undefined
            value: root.value
        }
        onTextChanged: {
            root.value = text
            setKey()
        }
    }
    DLabel {
        width: valueWidth
        visible: connectionSession.type == "wired"
        Binding on text {
            when: root.value != undefined
            value: root.value
        }
    }
    }
}

