/*
    Copyright (C) 2018 Rob van den Berg <rghvdberg at gmail dot org>
    Copyright (C) 2020 Joshua Wade

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
import QtGraphicalEffects 1.14

Rectangle {
    anchors.centerIn: parent
    id: root
    property Gradient borderGradient: borderGradient
    property int borderWidth: 1
    property real hue: 162 / 360;
    property bool showHighlightColor: false;
    property bool boostBrightness: false; // Knobs need this, for some reason
    property double _brightnessBoost: boostBrightness ? 0.2 : 0
    implicitWidth: 26
    implicitHeight: 26
    radius: 1
    color:"transparent"

    Loader {
        id: loader
        width: root.width
        height: root.height
        anchors.centerIn: parent
        active: borderGradient
        sourceComponent: gradientborder
    }

    Gradient {
        id: borderGradient
        GradientStop {
           position: 0
           color: Qt.rgba(1, 1, 1, 0.1 + _brightnessBoost)
        }
        GradientStop {
            position: 1
            color: Qt.rgba(1, 1, 1, 0 + _brightnessBoost * 0.5)
        }
    }

    Gradient {
        id: highlightGradient
        GradientStop {
           position: 0
           color: colors.main
        }
        GradientStop {
            position: 1
            color: colors.main
        }
    }

    Component {
        id: gradientborder
        Item {
            Rectangle {
                id: borderFill
                anchors.fill: parent
                gradient: showHighlightColor ? highlightGradient : borderGradient
                visible: false
            }

            Rectangle {
                id: mask
                radius: root.radius
                border.width: root.borderWidth
                anchors.fill: parent
                color: 'transparent'
                visible: false// otherwise a thin border might be seen.
            }

            OpacityMask {
                id: opM
                anchors.fill: parent
                source: borderFill
                maskSource: mask
            }
        }
    }
}
