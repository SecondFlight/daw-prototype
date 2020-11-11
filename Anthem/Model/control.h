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

#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QJsonValue>
#include <QJsonObject>

#include "Core/modelitem.h"

#include "Utilities/idgenerator.h"

class Control : public ModelItem {
    Q_OBJECT
private:
    quint64 id;
    float initialValue;
    float ui_currentValue;
    float minimum;
    float maximum;
    float step;
    // control symbol
    // connection
    bool overrideAutomation;

    void setOverrideState(bool isOverridden);
public:
    /// Create new control with the given values
    Control(
        ModelItem* parent,
        QString name,
        IdGenerator& idGenerator,
        float initialValue,
        float minimum,
        float maximum,
        float step
    );

    /// Derive information from existing JSON node
    Control(
        ModelItem* parent,
        QString name,
        QJsonObject& node
    );

    /// Generate new JSON node and add as field under parentNode
    /// parentNode[controlName] = {...newly generated control...}
    Control(QObject *parent,
            QJsonObject* parentNode,
            std::string controlName,
            float minimum,
            float maximum,
            float initialValue,
            float step);

    void serialize(QJsonObject& node) const override;

    void set(float val, bool isFinal);
    float get();

signals:

public slots:
};

#endif // CONTROL_H
