#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget::setWindowTitle("Тестовое задание KODE ");
    btnOpen = new QPushButton("Открыть", this);
    connect(btnOpen, &QPushButton::clicked, this, &MainWindow::BtnOpenClick);

    btnPrint = new QPushButton("Вывод", this);
    connect(btnPrint, &QPushButton::clicked, this, &MainWindow::BtnPrintClick);
    btnPrint->setEnabled(false);

    btnSave = new QPushButton("Сохранить", this);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::BtnSaveClick);
    btnSave->setEnabled(false);


    radioDistance = new QRadioButton("По расстоянию", this);
    radioDistance->setChecked(true);
    radioDistance->setEnabled(false);
    radioTime = new QRadioButton("По времени", this);
    radioTime->setEnabled(false);
    radioCategory = new QRadioButton("По типу", this);
    radioCategory->setEnabled(false);
    connect(radioCategory, &QRadioButton::toggled, this, &MainWindow::RadioCategoryToggled);
    radioName = new QRadioButton("По имени", this);
    radioName->setEnabled(false);

    textBox = new QTextEdit(this);
    textBox->setFixedHeight(600);
    textBox->setMinimumWidth(600);

    rbtnGroup = new QButtonGroup(this);
    rbtnGroup->addButton(radioDistance, 1);
    rbtnGroup->addButton(radioTime, 2);
    rbtnGroup->addButton(radioCategory, 3);
    rbtnGroup->addButton(radioName, 4);


    lblCount = new QLabel("Количество объектов для группировки по типу:", this);
    spinBoxCount = new QSpinBox(this);
    spinBoxCount->setMinimum(1);
    spinBoxCount->setMaximum(100);
    spinBoxCount->setEnabled(false);


    vbox = new QVBoxLayout(this);
    vbox->setSpacing(1);

    hbox = new QHBoxLayout(this);

    hbox->addWidget(btnOpen);
    hbox->addWidget(btnPrint);
    hbox->addWidget(radioDistance);
    hbox->addWidget(radioCategory);
    hbox->addWidget(radioTime);
    hbox->addWidget(radioName);
    vbox->addLayout(hbox);

    hboxCount = new QHBoxLayout(this);

    hboxCount->addWidget(lblCount);
    hboxCount->addWidget(spinBoxCount);
    vbox->addLayout(hboxCount);

    vbox->addWidget(textBox);
    vbox->addWidget(btnSave);

    mainWidget = new QWidget();
    mainWidget->setLayout(vbox);
    this->setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BtnOpenClick()
{

    QString fileContent;
    QString filename= QFileDialog::getOpenFileName(this, "Открыть файл", "", "Текстовые файлы (*.txt )" );

    if(filename.isEmpty())
        return;

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    vecItem.clear();
    while (1)
    {
        QString name, category;
        double x, y, creationTime;

        in >> name >> x >> y >> category >> creationTime;

        if ( in.atEnd())
            break;

        Item item = Item( name, x, y, category, creationTime );

        vecItem.push_back(item);

    }

    file.close();
    radioDistance->setEnabled(true);
    radioCategory->setEnabled(true);
    radioName->setEnabled(true);
    radioTime->setEnabled(true);

    btnPrint->setEnabled(true);
    btnSave->setEnabled(true);
}

void MainWindow::BtnSaveClick()
{
    QString filename= QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Текстовые файлы (*.txt )");

    if(filename.isEmpty())
        return;

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream ss(&file);
    ss << (textBox->toPlainText());

    file.close();

}

void MainWindow::RadioCategoryToggled(bool checked){
    if ( checked )
        spinBoxCount->setEnabled(true);
    else
        spinBoxCount->setEnabled(false);


}

void MainWindow::FillByDistance(QMap<QString, QVector<Item>>& mapDistance) const{
    for( Item item : vecItem ){

        if( item.GetDistance() < 100 )
            mapDistance[groupsDistance[0]].push_back(item);
        else if ( item.GetDistance() >= 100  && item.GetDistance() < 1000 )
            mapDistance[groupsDistance[1]].push_back(item);
        else if ( item.GetDistance() >= 1000  && item.GetDistance() < 10000 )
            mapDistance[groupsDistance[2]].push_back(item);
        else
            mapDistance[groupsDistance[3]].push_back(item);
    }
}

void MainWindow::FillByTime(QMap<QString, QVector<Item>>& mapDistance) const{

    for( Item item : vecItem ){
        QDateTime crDate = QDateTime::fromTime_t( item.getCreationTime());
        if( crDate.date() == QDate::currentDate() )
            mapDistance[groupsTime[0]].push_back(item);
        else if( crDate.date() == QDate::currentDate().addDays(-1) )
            mapDistance[groupsTime[1]].push_back(item);
        else if ( crDate.date().weekNumber() == QDate::currentDate().weekNumber() )
            mapDistance[groupsTime[2]].push_back(item);
        else if ( crDate.date().month() == QDate::currentDate().month() )
            mapDistance[groupsTime[3]].push_back(item);
        else if ( crDate.date().year() == QDate::currentDate().year() )
            mapDistance[groupsTime[4]].push_back(item);
        else
            mapDistance[groupsTime[5]].push_back(item);
    }
}

void MainWindow::FillByCategory(QMap<QString, QVector<Item>>& mapCategory) const{
    groupsCategory.clear();
    for( Item item : vecItem )
        mapCategory[item.getCategory()].push_back(item);


    QVector<Item> vecOther;
    for( auto itMap = mapCategory.begin(); itMap != mapCategory.end(); ++itMap)
        if( itMap->size() <= spinBoxCount->value() ) {
            vecOther.append(*itMap);
            itMap->clear();
        }else
            groupsCategory.push_back(itMap.key());

    mapCategory[otherCategoryName] = vecOther;
    groupsCategory.push_back(otherCategoryName);

}

void MainWindow::FillByName(QMap<QString, QVector<Item>>& mapName) const{
    groupsName.clear();

    for( Item item : vecItem ) {
        QChar ch = item.getName().at(0);

        if( ( ch >= L'А' && ch <= L'я' ) || ch == L'Ё' || ch == L'ё' ) {
            mapName[QString(ch)].push_back(item);
        }
        else
            mapName["#"].push_back(item);

    }

    for( auto itMap = mapName.begin(); itMap != mapName.end(); ++itMap )
        groupsName.push_back(itMap.key());

}

void MainWindow::PrintToTexBox(QMap<QString, QVector<Item>>& map, std::function<bool(Item, Item)> sortFunc, QVector<QString>& groups ) const{

    for( auto i = 0; i < groups.size(); ++i )
        if ( map[groups[i]].size() ){
            std::sort(map[groups[i]].begin(), map[groups[i]].end(), sortFunc);
            textBox->append(groups[i] + " :\n");

            for( auto itVec = map[groups[i]].begin(); itVec != map[groups[i]].end(); ++itVec )
                textBox->append("\t" + itVec->toString());
        }
}

void MainWindow::BtnPrintClick(){

    textBox->clear();
    QMap<QString, QVector<Item>> mapCurrGroup;

    auto sortByName = []( const Item & a, const Item & b ) -> bool
    {
        return a.getName() < b.getName();
    };

    auto sortByDistance = []( const Item & a, const Item & b ) -> bool
    {
        return a.GetDistance() < b.GetDistance();
    };


    auto sortByTime = []( const Item & a, const Item & b ) -> bool
    {
        return a.getCreationTime() < b.getCreationTime();
    };


    if ( radioDistance->isChecked() ) {
        FillByDistance(mapCurrGroup);
        PrintToTexBox( mapCurrGroup, sortByDistance, groupsDistance );
    }
    else if ( radioTime->isChecked() ) {
        FillByTime(mapCurrGroup);
        PrintToTexBox( mapCurrGroup, sortByTime, groupsTime );
    }
    else if ( radioCategory->isChecked() ) {
        FillByCategory( mapCurrGroup );
        PrintToTexBox( mapCurrGroup, sortByName , groupsCategory );
    }
    else if ( radioName->isChecked()) {
        FillByName( mapCurrGroup );
        PrintToTexBox( mapCurrGroup, sortByName , groupsName );

    }
}
