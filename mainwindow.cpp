#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QUrl"
#include "QRegExp"
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"
#include "QTextCodec"
#include "QDebug"
QString URL="https://www.bilibili.com/video/";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete manager;
    delete ui;
}

void MainWindow::on_INPUT_URL_textChanged(const QString &arg1)
{
    QString url=ui->INPUT_URL->text().toLatin1();
    QString GETURL;
   QRegExp rx1=QRegExp("bilibili.com/video/(av[0-9]{1,})");
   if(url.indexOf(rx1)>=0)
   {
       QString GETURL=URL+rx1.cap(1)+"/";
       manager = new QNetworkAccessManager(this);
       connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
       manager->get(QNetworkRequest(QUrl(GETURL)));
   }
   else
   {
       QRegExp rx2=QRegExp("(av[0-9]{1,})");
       if(url.indexOf(rx2)>=0)
       {
           QString GETURL=URL+rx2.cap(1)+"/";
           qDebug()<<GETURL;
           manager = new QNetworkAccessManager(this);
           connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
           manager->get(QNetworkRequest(QUrl(GETURL)));
       }
       else
       {
          ui->OUTPUT_TITLE->setText("地址输入不正确!");
       }
   }

}

void MainWindow::GET_INFO_Finished(QNetworkReply *reply)
{

    if (reply->error() == QNetworkReply::NoError)
     {//得到信息
       QTextCodec *codec =QTextCodec::codecForName("UTF-8");
        QString all = codec->toUnicode(reply->readAll());
         QRegExp rx=QRegExp("<title>(.*)</title>");
        rx.setMinimal(true);//设置最短匹配
        int pos = all.indexOf(rx);
       //     qDebug()<<pos;
          if (pos>=0)
          {
              if(rx.cap(1).indexOf("哔哩哔哩 (゜-゜)つロ 干杯~-bilibili")!=-1)
              {
                  ui->OUTPUT_TITLE->setText("找不到此视频，请检查地址是否输入正确!");
              }
              else
              {
                  QString title=rx.cap(1);
                  QRegExp r=QRegExp("(.*)_");
                  r.setMinimal(true);
                 int pos= title.indexOf(r);
                 if(pos>=0)
                 ui->OUTPUT_TITLE->setText(r.cap(1));
              }
          }
          else
          {
             ui->OUTPUT_TITLE->setText("找不到此视频，请检查地址是否输入正确!");
          }
     }
     else
     {//得不到信息
         ui->OUTPUT_TITLE->setText("网络君开小差了...请检查网络链接后重试.");
     }
    reply->deleteLater();//回收
}
