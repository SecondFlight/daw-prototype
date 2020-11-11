/*
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

#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QColor>
#include <QJsonObject>

#include "Core/modelitem.h"
#include "Utilities/idgenerator.h"

#include "Model/pattern.h"

class Song : public ModelItem {
    Q_OBJECT
private:
    IdGenerator* id;
    QHash<QString, Pattern*> patterns;
public:
    Song(ModelItem* parent, IdGenerator* id);
    Song(ModelItem* parent, IdGenerator* id, QJsonObject& node);

    void serialize(QJsonObject& node) const override;

    QString addPattern(QString name, QColor color);
    void addPattern(QString id, QString name, QColor color);
    void removePattern(QString id);

    const QHash<QString, Pattern*>& getPatterns();
    Pattern* getPattern(QString key);

signals:

public slots:
};

#endif // SONG_H
