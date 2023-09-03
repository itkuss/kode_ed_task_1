#ifndef ITEM_H
#define ITEM_H

#include <QString>

// класс описывает объект из входного файла
class Item
{

private:
    QString name, category;
    double x, y, creationTime;


public:
    Item();
    Item( QString name, double x, double y,  QString category, double creationTime ) :
    name(name), category(category), x(x), y(y), creationTime(creationTime) {

    }

    const QString &getName() const;
    const QString &getCategory() const;
    double getX() const;
    double getY() const;
    double getCreationTime() const;
    void setName(const QString &newName);
    void setCategory(const QString &newCategory);
    void setX(double newX);
    void setY(double newY);
    void setCreationTime(double newCreationTime);
    QString toString() const;
    double GetDistance() const;

};

#endif // ITEM_H
