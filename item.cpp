#include <QTextStream>
#include <QtCore>
#include <cmath>
#include "item.h"

const QString &Item::getName() const
{
    return name;
}

const QString &Item::getCategory() const
{
    return category;
}

double Item::getX() const
{
    return x;
}

double Item::getY() const
{
    return y;
}

double Item::getCreationTime() const
{
    return creationTime;
}

void Item::setName(const QString &newName)
{
    name = newName;
}

void Item::setCategory(const QString &newCategory)
{
    category = newCategory;
}

void Item::setX(double newX)
{
    x = newX;
}

void Item::setY(double newY)
{
    y = newY;
}

void Item::setCreationTime(double newCreationTime)
{
    creationTime = newCreationTime;
}

QString Item::toString() const
{
    QString line;
    QTextStream ss(&line);
    ss << name << " " << x << " " << y << " " << category << " " << QDateTime::fromTime_t(creationTime).toString();
    return line;

}

double Item::GetDistance() const
{
    return sqrt( x * x + y * y );
}

Item::Item()
{

}
