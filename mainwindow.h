#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QVector>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>

#include "item.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
  void BtnOpenClick();
  void BtnPrintClick();
  void BtnSaveClick();
  void RadioCategoryToggled(bool checked);

private:
    // c++17 inline const
    inline static QVector<QString> groupsDistance = {"До 100 ед.", "До 1000 ед.", "До 10000 ед.", "Слишком далеко"};
    inline static QVector<QString> groupsTime = {"Сегодня", "Вчера", "На этой неделе", "В этом месяце", "В этом году", "Ранее"};
    inline static QVector<QString> groupsCategory = {};
    inline static QString otherCategoryName = "Разное";
    inline static QVector<QString> groupsName = {};

    Ui::MainWindow *ui;
    QButtonGroup *rbtnGroup;
    QRadioButton *radioDistance, *radioCategory,*radioTime, *radioName;
    QPushButton *btnOpen, *btnPrint, *btnSave;
    QVBoxLayout *vbox;
    QHBoxLayout *hbox, *hboxCount;
    QWidget *mainWidget;
    QVector <Item> vecItem;
    QTextEdit* textBox;
    QLabel* lblCount;
    QSpinBox* spinBoxCount;

    void FillByDistance(QMap<QString, QVector<Item>>& mapDistance) const;
    void FillByTime(QMap<QString, QVector<Item>>& mapTime) const;
    void FillByCategory(QMap<QString, QVector<Item>>& mapCategory) const;
    void FillByName(QMap<QString, QVector<Item>>& mapName) const;
    void PrintToTexBox(QMap<QString, QVector<Item>>& map, std::function<bool(Item, Item)> sortFunc, QVector<QString>& groups ) const;

};


#endif // MAINWINDOW_H
