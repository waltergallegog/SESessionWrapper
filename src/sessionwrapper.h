#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>


//SEcure related
//#include "SEfile.h"
#include "se3/L1.h"
#include <QSharedMemory>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QProcess *walletProc;
    se3_session *s;              // session variable
    QSharedMemory shaMemSession, shaMemReq, shaMemRes, shaMemDevBuf;

    void clean();

private slots:
    void processFinished(int code , QProcess::ExitStatus status);
//    void processOutput();
};

#endif // MAINWINDOW_H
