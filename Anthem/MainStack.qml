/*
    Copyright (C) 2019, 2020 Joshua Wade

    This file is part of Anthem.

    Anthem is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Anthem is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Anthem. If not, see
                        <https://www.gnu.org/licenses/>.
*/

import QtQuick 2.14
import QtQuick.Controls 2.14
import "BasicComponents"
import "Explorer"

SplitView {
    id: mainStack
    orientation: Qt.Horizontal
    property bool showControllerRack
    property bool showExplorer
    property bool showEditors

    Item {
        visible: showExplorer
        implicitWidth: 200
        SplitView.minimumWidth: 200
        Explorer {
            anchors.fill: parent
        }
    }

    CenterStack {
        SplitView.fillWidth: true
        showEditors: mainStack.showEditors
    }

    Item {
        visible: showControllerRack
        implicitWidth: 200
        SplitView.minimumWidth: 200
    }

    handle: Rectangle {
        implicitWidth: 4
        implicitHeight: this.height

        color: "transparent"
    }
}
