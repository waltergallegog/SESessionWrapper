#include "sessionwrapper.h"
#include "ui_mainwindow.h"

#include "securefiledialog.h"
#include "logindialog.h"

#include "se3/se3comm.h"

#include <QDebug>
#include <QBuffer>
#include <QDataStream>
#include <QString>

#define QD(print) qDebug() << "WRAPPER: " << print
#define QD2(print) qDebug() << #print << "                              "<< print

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    shaMemSession("sessionSharing"),
    shaMemReq("requestSharing"),
    shaMemRes("responseSharing"),
    shaMemDevBuf("devBufSharing")
{
    ui->setupUi(this);

    if (shaMemSession.isAttached())
        if (!shaMemSession.detach())
            qDebug() << "WRAPPER: Unable to detach from shared memory sess";

    if (shaMemReq.isAttached())
        if (!shaMemReq.detach())
            qDebug() << "WRAPPER: Unable to detach from shared memory req";

    if (shaMemRes.isAttached())
        if (!shaMemRes.detach())
            qDebug() << "WRAPPER: Unable to detach from shared memory res";

    if (shaMemDevBuf.isAttached())
        if (!shaMemDevBuf.detach())
            qDebug() << "WRAPPER: Unable to detach from shared memory buf";

    //    // load into shared memory
    //    QBuffer buffer;
    //    buffer.open(QBuffer::ReadWrite);
    //    QDataStream out(&buffer);
    //    int size = buffer.size();

    if (!shaMemSession.create(sizeof(se3_session))) {
        qDebug() << "WRAPPER: Unable to create session, try attach, detach method";
        QD(shaMemSession.attach());
        QD(shaMemSession.detach());
        if (!shaMemSession.create(sizeof(se3_session))) {
            qDebug() << "WRAPPER: attach detach did not work";
            exit(0);
        }
    }

    /// in L0.c, device request and response are assigned (uint8_t*)malloc(SE3_COMM_N*SE3_COMM_BLOCK);
    if (!shaMemReq.create(SE3_COMM_N*SE3_COMM_BLOCK)) {
        qDebug() << "WRAPPER: Unable to create req, try attach, detach method";
        QD(shaMemReq.attach());
        QD(shaMemReq.detach());
        if (!shaMemReq.create(SE3_COMM_N*SE3_COMM_BLOCK)) {
            qDebug() << "WRAPPER: attach detach did not work";
            exit(0);
        }
    }
    if (!shaMemRes.create(SE3_COMM_N*SE3_COMM_BLOCK)) {
        qDebug() << "WRAPPER: Unable to create res, try attach, detach method";
        QD(shaMemRes.attach());
        QD(shaMemRes.detach());
        if (!shaMemRes.create((SE3_COMM_N*SE3_COMM_BLOCK))) {
            qDebug() << "WRAPPER: attach detach did not work";
            exit(0);
        }
    }

    /// device has a void pointer which is later initialized as memalign 512
    if (!shaMemDevBuf.create(SE3_COMM_BLOCK)) {
        qDebug() << "WRAPPER: Unable to create buf, try attach, detach method";
        QD(shaMemDevBuf.attach());
        QD(shaMemDevBuf.detach());
        if (!shaMemDevBuf.create((SE3_COMM_BLOCK))) {
            qDebug() << "WRAPPER: attach detach did not work";
            exit(0);
        }
    }

    s = (se3_session*)shaMemSession.data();


    walletProc = new QProcess(this);

    QString folder = "../../build-SEcubeWallet-Desktop_Qt_5_8_0_GCC_64bit-Debug/scr/scr";
    folder = "/home/walter/Documents/Thesis/build-SEcubeWallet-Desktop_Qt_5_8_0_GCC_64bit-Debug/scr/scr";

    walletProc->setProcessChannelMode(QProcess::ForwardedChannels);

    connect(walletProc,
            SIGNAL(finished(int,QProcess::ExitStatus)),
            this,
            SLOT(processFinished(int, QProcess::ExitStatus)));


    walletProc->start(folder, QStringList() << "");

}

MainWindow::~MainWindow(){
    QD("destructor");

    delete ui;
}

void MainWindow :: processFinished(int code , QProcess::ExitStatus status){
    qDebug()<< "WRAPPER: on process finished";
    qDebug() << code <<status;
    clean();
}


void MainWindow :: clean(){
    qDebug() << "WRAPPER: clean";
    secure_finit(); /*Once the user has finished all the operations it is strictly required to call the secure_finit() to avoid memory leakege*/
    qDebug() << "WRAPPER: passed finit";

    int ret;

    shaMemSession.lock();
    shaMemReq.lock();
    shaMemRes.lock();
    shaMemDevBuf.lock();


    if(s->logged_in){
        QD ("s is logged in");
        s->device.request = (uint8_t*)shaMemReq.data();
        s->device.response = (uint8_t* )shaMemRes.data();
        s->device.f.buf = shaMemDevBuf.data();

        if( (ret=fix_fd(s->device.info.path, &(s->device.f))) !=SE3_OK)
            QD("error fixing file descriptor");

        else{
            L1_logout(s);
            qDebug ()<<"WRAPPER: logged out";
        }
    }

    shaMemSession.unlock();
    shaMemReq.unlock();
    shaMemRes.unlock();
    shaMemDevBuf.unlock();


    if (shaMemSession.isAttached()){
        qDebug()<<"WRAPPER: going to detach";
        if (!shaMemSession.detach())
            qDebug() << "WRAPPER: Unable to detach from shared memory sess";
    }

    if (shaMemReq.isAttached()){
        qDebug()<<"WRAPPER: going to detach";
        if (!shaMemReq.detach())
            qDebug() << "WRAPPER: Unable to detach from shared memory req";
    }

    if (shaMemRes.isAttached()){
        qDebug()<<"WRAPPER: going to detach";
        if (!shaMemRes.detach())
            qDebug() << "WRAPPER: Unable to detach from shared memory res";
    }

    if (shaMemDevBuf.isAttached()){
        qDebug()<<"WRAPPER: going to detach";
        if (!shaMemDevBuf.detach())
            qDebug() << "WRAPPER: Unable to detach from shared memory buf";
    }

    QApplication::quit();
    //TODO: check if quit or exit call destr
}
