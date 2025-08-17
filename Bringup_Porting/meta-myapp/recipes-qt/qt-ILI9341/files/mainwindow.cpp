#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet(R"(
        QMainWindow {
            background-color: white;
        }
        QWidget {
            background-color: white;
        }
        QMenuBar, QMenu {
            background-color: white;
        }
        QStatusBar {
            background-color: white;
        }
    )");

    // init timer
    idleTimer = new QTimer(this);
    idleTimer->setInterval(5000); // 5s
    idleTimer->setSingleShot(true); // run one time when start
    connect(idleTimer, &QTimer::timeout, this, &MainWindow::resetToDefaultImage);

    // connect button -> change image
    connect(ui->btn_cake, &QPushButton::clicked, this, [=](){
        changeDoraemonImage(":/images/doraemon_eating.png");
        resetIdleTimer();
    });
    connect(ui->btn_mourse, &QPushButton::clicked, this, [=](){
        changeDoraemonImage(":/images/doraemon_scared.jpg");
        resetIdleTimer();
    });
    connect(ui->btn_nobita, &QPushButton::clicked, this, [=](){
        changeDoraemonImage(":/images/doraemon_nobita.png");
        resetIdleTimer();
    });

    // start count when open app
    resetIdleTimer();
}

void MainWindow::changeDoraemonImage(const QString &path)
{
    auto effect = new QGraphicsOpacityEffect(ui->label_doraemon);
    ui->label_doraemon->setGraphicsEffect(effect);

    auto fadeOut = new QPropertyAnimation(effect, "opacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    auto fadeIn = new QPropertyAnimation(effect, "opacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    connect(fadeOut, &QPropertyAnimation::finished, this, [=]() {
        ui->label_doraemon->setPixmap(QPixmap(path));
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    });

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::resetToDefaultImage()
{
    changeDoraemonImage(":/images/doraemon.png");
}

void MainWindow::resetIdleTimer()
{
    idleTimer->start(); // restart 10s
}

MainWindow::~MainWindow()
{
    delete ui;
}
