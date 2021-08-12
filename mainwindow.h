#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <common.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ukui.titlebar")

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void titlebarDbusInit();
    void updateTitlebarValue(int value, bool midShow);

    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public Q_SLOTS:
    bool updateTitlebar(int value, bool show);

private:
    QHBoxLayout *m_mainLayout;
    QWidget *m_leftWid;
    QWidget *m_midWid;
    QWidget *m_rightWid;
    struct {
        int left;
        int value;
        int right;
    } m_titlebarLeft;
    int m_titlebarType;
    bool m_titlebarPressed = 0;
    QString m_titlebarLeftPosition;
    bool m_titlebarOn = 0;
    bool m_midWidShow = false;
};
#endif // MAINWINDOW_H
