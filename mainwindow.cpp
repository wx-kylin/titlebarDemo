#include "mainwindow.h"
#include <QDebug>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

void sleep(unsigned int msec)
{
    QTime reachTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < reachTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "mainWindow init in.";
    m_mainLayout = new QHBoxLayout(this);
    this->resize(MAINWID_WIDTH, MAINWID_HIGHT);
    this->setMouseTracking(true);

    titlebarDbusInit();

    QPalette p;
    // left widget init
    m_leftWid = new QWidget(this);
    m_leftWid->setGeometry(0, 0, LEFTWID_WIDTH, this->height());
    m_leftWid->setAutoFillBackground(true);
    p.setBrush(m_leftWid->backgroundRole(),QBrush(QColor(LEFTWID_COLOR_R, LEFTWID_COLOR_G, LEFTWID_COLOR_B)));
    m_leftWid->setPalette(p);
    m_leftWid->setMouseTracking(true);

    // mid widget init
    m_midWid = new QWidget(this);
    m_midWid->setGeometry(m_leftWid->width(), 0, MIDWID_WIDTH_HIDE, this->height());
    m_midWid->setAutoFillBackground(true);
    p.setBrush(m_midWid->backgroundRole(),QBrush(QColor(MIDWID_COLOR_R, MIDWID_COLOR_G, MIDWID_COLOR_B)));
    m_midWid->setPalette(p);
    m_midWid->setMouseTracking(true);

    // right widget init
    m_rightWid = new QWidget(this);
    m_rightWid->setGeometry(m_leftWid->width() + m_midWid->width(), 0, this->width() - m_leftWid->width() - m_midWid->width(), this->height());
    m_rightWid->setAutoFillBackground(true);
    p.setBrush(m_rightWid->backgroundRole(),QBrush(QColor(RIGHTWID_COLOR_R, RIGHTWID_COLOR_G, RIGHTWID_COLOR_B)));
    m_rightWid->setPalette(p);
    m_rightWid->setMouseTracking(true);

    // add to the layout
    m_mainLayout->addWidget(m_leftWid);
    m_mainLayout->addWidget(m_midWid);
    m_mainLayout->addWidget(m_rightWid);
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);
}

MainWindow::~MainWindow()
{
    delete m_mainLayout;
    delete m_leftWid;
    delete m_midWid;
    delete m_rightWid;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_leftWid->resize(m_leftWid->width(), this->height());
    m_midWid->resize(m_midWid->width(), this->height());
    m_rightWid->resize(this->width() - m_leftWid->width() - m_midWid->width(), this->height());
}

void MainWindow::updateTitlebarValue(int value, bool midShow)
{
    QPalette p;

    m_leftWid->setGeometry(0, 0, value, this->height());
    p.setBrush(m_leftWid->backgroundRole(),QBrush(QColor(LEFTWID_COLOR_R, LEFTWID_COLOR_G, LEFTWID_COLOR_B)));
    m_leftWid->setPalette(p);

    if (midShow == true) {
        m_midWid->setGeometry(m_leftWid->width(), 0, MIDWID_WIDTH_SHOW, this->height());
    } else {
        m_midWid->setGeometry(m_leftWid->width(), 0, MIDWID_WIDTH_HIDE, this->height());
    }
    p.setBrush(m_midWid->backgroundRole(),QBrush(QColor(MIDWID_COLOR_R, MIDWID_COLOR_G, MIDWID_COLOR_B)));
    m_midWid->setPalette(p);

    m_rightWid->setGeometry(m_leftWid->width() + m_midWid->width(), 0, this->width() - m_leftWid->width() - m_midWid->width(), this->height());
    p.setBrush(m_rightWid->backgroundRole(),QBrush(QColor(RIGHTWID_COLOR_R, RIGHTWID_COLOR_G, RIGHTWID_COLOR_B)));
    m_rightWid->setPalette(p);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_titlebarType != 0) {
        if (m_titlebarPressed == 1) {
            if (event->pos().x() <= m_titlebarLeft.left) {
                m_titlebarLeft.value = m_titlebarLeft.left;
                m_titlebarLeftPosition = "left";
//                this->setCursor(Qt::SplitHCursor);
            } else if (event->pos().x() >= m_titlebarLeft.right) {
                m_titlebarLeft.value = m_titlebarLeft.right;
                m_titlebarLeftPosition = "right";
//                this->setCursor(Qt::SplitHCursor);
            } else {
                m_titlebarLeftPosition = "center";
                m_titlebarLeft.value = event->pos().x();
//                this->setCursor(Qt::SplitHCursor);
            }

            QDBusMessage message = QDBusMessage::createMethodCall("org.ukui.KWin",
                                    "/KWin",
                                    "org.ukui.KWin",
                                    "titlebarLeftUpdate");
            message << QString("titlebarDemo") << int(m_titlebarLeft.value) << bool(true);   //0代表隐藏菜单，1表示正常显示菜单
            QDBusConnection::sessionBus().send(message);
            sleep(3);
            updateTitlebarValue(m_titlebarLeft.value, true);
        } else {
            if (((event->pos().x() - 0) >= (m_titlebarLeft.value - 3 > 0 ? (m_titlebarLeft.value - 3): 0)) && ((event->pos().x() - 0) <= (m_titlebarLeft.value + 3))) {
                m_titlebarOn = 1;
                this->setCursor(Qt::SplitHCursor);
                QDBusMessage message = QDBusMessage::createMethodCall("org.ukui.KWin",
                                        "/KWin",
                                        "org.ukui.KWin",
                                        "titlebarLeftUpdate");
                message << QString("titlebarDemo") << int(m_titlebarLeft.value) << bool(true);   //0代表隐藏菜单，1表示正常显示菜单
                QDBusConnection::sessionBus().send(message);
                sleep(3);
                updateTitlebarValue(m_titlebarLeft.value, true);
            } else {
                if (m_titlebarOn == 1) {
                    m_titlebarOn = 0;
//                    this->setCursor(Qt::ArrowCursor);

                    QDBusMessage message = QDBusMessage::createMethodCall("org.ukui.KWin",
                                            "/KWin",
                                            "org.ukui.KWin",
                                            "titlebarLeftUpdate");
                    message << QString("titlebarDemo") << int(m_titlebarLeft.value) << bool(false);   //0代表隐藏菜单，1表示正常显示菜单
                    QDBusConnection::sessionBus().send(message);
                    sleep(3);
                    updateTitlebarValue(m_titlebarLeft.value, false);
                }

            }
            update();
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (m_titlebarType != 0) {
        if (((event->pos().x() - 0) >= (m_titlebarLeft.value - 3 > 0 ? (m_titlebarLeft.value - 100): 0)) && ((event->pos().x() - 0) <= (m_titlebarLeft.value + 3))) {
            m_titlebarPressed = 1;
        } else {
            m_titlebarPressed = 0;
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "===========================mouseReleaseEvent in";
    if (m_titlebarType != 0) {
        if (m_titlebarPressed == 1) {
            m_titlebarPressed = 0;
        }
    }
}
