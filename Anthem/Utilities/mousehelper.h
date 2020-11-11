/*
    Copyright (C) 2019 Joshua Wade

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

#ifndef MOUSEHELPER_H
#define MOUSEHELPER_H

#include <QObject>
#include <QCursor>

class MouseHelper : public QObject {
    Q_OBJECT
public:
    explicit MouseHelper(QObject *parent = nullptr);

    Q_INVOKABLE void setCursorPosition(int x, int y);
    Q_INVOKABLE QPoint getCursorPosition();
    Q_INVOKABLE void setCursorToBlank();
    Q_INVOKABLE void clearOverride();

signals:

public slots:
};

#endif // MOUSEHELPER_H
