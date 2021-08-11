#include <common.h>
#include <mainwindow.h>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusInterface>
#include <QDebug>

void MainWindow::titlebarDbusInit()
{
    //session bus 会话总线
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if(sessionBus.registerService("org.ukui.titlebarDemo")){
        sessionBus.registerObject("/org/ukui/titlebarDemo",this,
                                  QDBusConnection::ExportAllContents);
    }

    // send message to kwin, tell kwin titlebar info before show
    qDebug() << "titlebarDbusInit in";
    QDBusInterface interface( "org.ukui.KWin", "/KWin", "org.ukui.KWin", QDBusConnection::sessionBus());
    QDBusReply<bool> reply = interface.call( "customTitlebar", "titlebarDemo", 1, \
                                                "org.ukui.titlebarDemo", "/org/ukui/titlebarDemo", "titlebar", "updateTitlebar");
    if ( reply.isValid() && reply.value() == 0) {
        qDebug( "titlebarDbus Init end, return value is %d", reply.value());          // prints 4
    } else {
        qDebug() << "titlebarDbus Init failed.";
        return;
    }


    m_titlebarLeft.value = LEFTWID_WIDTH;
    m_titlebarLeft.left = 100;
    m_titlebarLeft.right = 1000;
    m_titlebarLeftPosition = "center";
    m_titlebarType = 1;

    return ;
}

bool MainWindow::updateTitlebar(int value, bool midShow)
{
    if (m_titlebarLeft.value == value && m_midWidShow == midShow) {
        return 0;
    }
    m_titlebarLeft.value = value;
    m_midWidShow = midShow;
    updateTitlebarValue(value, midShow);

    return false;
}
