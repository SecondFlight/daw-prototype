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

#include "pattern.h"

Pattern::Pattern(ModelItem* parent, IdGenerator* id, QString displayName,
                 QColor color) : ModelItem(parent, "pattern") {
    this->id = id;
    this->displayName = displayName;
    this->color = color;
}

Pattern::Pattern(
        ModelItem* parent,
        IdGenerator* id,
        QJsonObject& patternNode) : ModelItem(parent, "pattern") {
    this->id = id;
    this->displayName = patternNode["display_name"].toString();
    this->color = QColor(patternNode["color"].toString());
}

void Pattern::serialize(QJsonObject& node) const {
    node["display_name"] = this->displayName;
    node["color"] = this->color.name();
}

QString Pattern::getDisplayName() {
    return displayName;
}

QColor Pattern::getColor() {
    return color;
}
