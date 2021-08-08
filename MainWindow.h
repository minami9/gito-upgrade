#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QFile>
#include <QProcess>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

    void doUpgrade(const QUrl &url);
    void setFileSize(const quint64 &fileSize);
private:
    Ui::MainWindow *ui;
    QNetworkAccessManager mNetMgr;
    QFile mTargetFile;
    QString mFileName;
    QProcess p;
    quint64 mFileSize;
    quint64 mRecvSize = 0;
};
#endif // MAINWINDOW_H
