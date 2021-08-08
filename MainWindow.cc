#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QMessageBox>
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::information(this, QString::fromLocal8Bit("gito-nikki更新"),
                             QString::fromLocal8Bit("更新过程中不允许退出。"));

    event->ignore();  //忽略退出信号，程序继续运
}

void MainWindow::setFileSize(const quint64 &fileSize)
{
    mFileSize = fileSize;
}

void MainWindow::doUpgrade(const QUrl &url)
{
    QNetworkRequest req;
    req.setUrl(url);
    req.setRawHeader("Accept", "application/json, text/plain, */*");
    req.setRawHeader("Accept-Language", "zh-CN,en-US;q=0.8");

    mFileName = url.fileName();

    QSslConfiguration sslConfig;
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfig.setProtocol(QSsl::TlsV1SslV3);   //具体以OpenSSL版本为准
    req.setSslConfiguration(sslConfig);

    QNetworkReply *reply = mNetMgr.get(req);

    mTargetFile.setFileName(QCoreApplication::applicationDirPath() + "/" + mFileName);

    connect(reply, &QNetworkReply::readyRead, this, [=]() {
        if (!mTargetFile.isOpen()) {
            if (!mTargetFile.open(QFile::ReadWrite)) {
                QMessageBox::information(this, QString::fromLocal8Bit("gito-nikki更新"),
                                         QString::fromLocal8Bit("更新失败，因为找不到当前的gito-nikki可执行文件。") + mTargetFile.fileName());
                return;
            }
            mTargetFile.resize(0);
        }
        QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
        if (!reply)
            return;
        QByteArray bytes = reply->readAll();
        mTargetFile.write(bytes);
        mTargetFile.flush();

        mRecvSize += bytes.size();
        int p = static_cast<float>(mRecvSize) / static_cast<float>(mFileSize) * 100;
        ui->progressBar->setValue(p);
    });
    connect(reply, &QNetworkReply::finished, this, [=](){
        mTargetFile.close();
        p.setProgram(QCoreApplication::applicationDirPath() + "/" + mFileName);
        p.startDetached();


        QCoreApplication::exit(0);
    });
    connect(reply,
            static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            this, [=](QNetworkReply::NetworkError error) {
        Q_UNUSED(error);
        QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
        if (!reply)
            return;
        QMessageBox::information(this, QString::fromLocal8Bit("gito-nikki更新"),
                                 QString::fromLocal8Bit("更新失败，") + reply->errorString());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

