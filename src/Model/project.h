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

#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QHash>

#include "Core/communicator.h"
#include "Core/modelitem.h"

#include "transport.h"
#include "song.h"
#include "generator.h"

class Project : public ModelItem {
    Q_OBJECT
private:
    IdGenerator* id;
    Transport* transport;
    Song* song;
    QHash<QString, Generator*> generators;
    QVector<QString> generatorOrder;
public:
    Project(Communicator* parent, IdGenerator* id);
    Project(Communicator* parent, IdGenerator* id, QJsonObject& node);

    void serialize(QJsonObject& node) const override;

    Transport* getTransport();
    Song* getSong();
signals:

public slots:
};

#endif // PROJECT_H
