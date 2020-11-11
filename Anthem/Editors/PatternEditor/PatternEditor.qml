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
import "../../BasicComponents"
import "../../Menus"

Item {
    id: patternEditor
    anchors.margins: 3

    clip: true

    property var patterns: ({})

    function updatePatternList() {
        const lastSelectedId = patternSelector.selectedItem.id;

        // If this starts as false then it can't become true
        let wasLastSelectedIdRemoved = patternSelector.selectedItem.id !== undefined;

        // Repopulate list items
        patternSelector.listItems = Object.keys(patterns).map((id, index) => {
            wasLastSelectedIdRemoved =
                wasLastSelectedIdRemoved && !(id === lastSelectedId);
            return { id, displayName: patterns[id].displayName };
        });

        // If the previously selected list item was removed, select a new item.
        if (wasLastSelectedIdRemoved) {
            patternSelector.selectItem(
                patternSelector.selectedItemIndex <
                    patternSelector.listItems.length
                    ? patternSelector.selectedItemIndex
                    : patternSelector.listItems.length - 1
            );
        }
    }

    Item {
        id: topRowContainer
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 20

        Button {
            id: menuButton
            anchors.top: parent.top
            anchors.left: parent.left
            height: parent.height
            width: parent.height

            imageSource: "Images/icons/file/hamburger.svg"
            imageWidth: 8
            imageHeight: 9

            onClicked: {
                patternEditorMenu.open();
            }
        }

        Menu {
            id: patternEditorMenu
            menuX: 0
            menuY: menuButton.height

            menuItems: [
                {
                    text: qsTr('New pattern...'),
                    onTriggered: () => {
                        addPatternTooltip.open();
                    }
                },
                {
                    text: qsTr('Delete pattern'),
                    onTriggered: () => {
                        const id = patternSelector.selectedItem.id;

                        const command = {
                            exec: () => {
                                PatternPresenter.removePattern(id);
                                delete patterns[id];
                                updatePatternList();
                            },
                            undo: (patternData) => {
                                const pattern = patternData.pattern;
                                const index = patternData.index;

                                PatternPresenter.createPattern(
                                    id, pattern.displayName, pattern.color
                                );

                                patterns[id] = pattern;
                                updatePatternList();
                                patternSelector.selectItem(index);
                            },
                            undoData: {pattern: patterns[id], index: patternSelector.selectedItemIndex},
                            description: qsTr('Delete pattern')
                        }

                        exec(command);
                    },
                    disabled: patternSelector.selectedItem.id === undefined
                }
            ]
        }

        ListSelector {
            id: patternSelector

            anchors {
                left: menuButton.right
                leftMargin: 3
                top: parent.top
                bottom: parent.bottom
            }
            width: 169
            menuMaxWidth: 200

            allowNoSelection: true
            hoverMessage: qsTr("Select a pattern to edit...")

            listItems: []
        }

        RenameTooltip {
            id: addPatternTooltip
            y: parent.height + 3
            defaultName: qsTr('New pattern');
            onAccepted: {
                const id = Anthem.createID();

                const command = {
                    exec: () => {
                        PatternPresenter.createPattern(id, name, color);
                        patterns[id] = {
                            displayName: name,
                            color: color
                        }
                        updatePatternList();

                        patternSelector.selectItem(patternSelector.listItems.length - 1);
                    },
                    undo: () => {
                        PatternPresenter.removePattern(id);
                        delete patterns[id];
                        updatePatternList();
                    },
                    description: qsTr('create pattern')
                }

                exec(command);
            }
        }

        Button {
            id: btnAddAudio
            anchors {
                left: patternSelector.right
                leftMargin: 3
                top: parent.top
                bottom: parent.bottom
            }

            width: 27

            imageWidth: 19
            imageHeight: 12
            imageSource: "Images/icons/pattern-editor/add-audio.svg"

            hoverMessage: qsTr("Add audio channel")
        }

        Button {
            id: btnAddAutomation
            anchors {
                left: btnAddAudio.right
                leftMargin: 3
                top: parent.top
                bottom: parent.bottom
            }

            width: 27

            imageWidth: 19
            imageHeight: 12
            imageSource: "Images/icons/pattern-editor/add-automation.svg"

            hoverMessage: qsTr("Add atuomation channel")
        }
    }

    Item {
        id: channelsContainer

        anchors {
            top: topRowContainer.bottom
            topMargin: 2
            bottom: footerContainer.top
            bottomMargin: 2
            left: parent.left
            leftMargin: 2
            right: parent.right
        }

        ChannelList {
            id: channelList
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: verticalScrollbar.left
                rightMargin: 2
            }
        }
        Scrollbar {
            id: verticalScrollbar
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
            }
            width: 20
        }
    }

    Item {
        id: footerContainer
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        height: 20

        Scrollbar {
            id: horizontalScrollbar
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                leftMargin: 258
                right: resizer.left
                rightMargin: 2
            }
        }

        Rectangle {
            id: resizer
            anchors {
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }
            width: parent.height
            color: Qt.rgba(1, 1, 1, 0.2)
        }
    }
}
