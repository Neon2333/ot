#include "mainwindow.h"
#include <vector>
#include <QDebug>
#include "config.hpp"

MainWindow::MainWindow(QWidget *parent)
{
    initUI();

    m_ocrLanguage=config::ocrLanguageCh;

    m_shot = new ScreenshotWidget;
    m_shot->hide();

    installGlobalEventFilter(); //安装事件过滤器，实现按键事件

    m_ocr = new Ocr(std::vector<QString>{"--screenshot"});
    connect(m_shot, &ScreenshotWidget::areaSelectFinished, this, [&](const QRect& rectSelected){
        //识别
        m_ocr->useConfig(m_ocrLanguage); //更新配置
        QString cmdRect = QString("rect=%1,%2,%3,%4")
                              .arg(rectSelected.x())
                              .arg(rectSelected.y())
                              .arg(rectSelected.width())
                              .arg(rectSelected.height());
        m_ocr->setCommands(std::vector<QString>{"--sc", "screen=0", cmdRect});
        // m_ocr->setCommands(std::vector<QString>{QString("--path \"%1\"").arg(config::saveShotPath)});

        m_ocr->startOcr();
    });

    //识别内容，讲识别内容写入控件
    connect(m_ocr, &Ocr::resReady, this, [&](QString res){
        qDebug()<<"resReady";
        this->show();
        m_label_textOCR->setText(res);
    });
}

MainWindow::~MainWindow() {}

void MainWindow::initUI()
{
    resize(600,300);
    move(800,300);

    const QString qssStr = QString("QLabel{background-color:rgba(170,118,105,255);}");


    m_label_textOCR = new AutoFontSizeLabel(this);
    // m_label_textOCR->setText("hello");
    m_label_textTran = new AutoFontSizeLabel(this);
    // m_label_textTran->setText("world");
    m_hlay = new QHBoxLayout;
    m_hlay->setSpacing(20);
    m_hlay->setContentsMargins(4,4,4,4);
    m_hlay->addWidget(m_label_textOCR, 1);
    m_hlay->addWidget(m_label_textTran, 1);

    m_label_textOCR->setStyleSheet(qssStr);
    m_label_textTran->setStyleSheet(qssStr);

    content = new QWidget(this);
    content->setLayout(m_hlay);
    this->setCentralWidget(content);
}

void MainWindow::installGlobalEventFilter()
{
    // 安装全局事件过滤器
    qApp->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        // 处理按键事件
        if(keyEvent->modifiers() & Qt::AltModifier)
        {
            // if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            if(keyEvent->key() == Qt::Key_1)
            {
                m_ocrLanguage=config::ocrLanguageCh;
                this->hide();
                m_shot->show();//显示截图控件，按下鼠标进行截图

                return true; // 表示事件已处理
            }
            else if(keyEvent->key() == Qt::Key_2)
            {
                m_ocrLanguage=config::ocrLanguageEn;
                this->hide();
                m_shot->show();

                return true;
            }
            else if(keyEvent->key() == Qt::Key_3)
            {
                m_ocrLanguage=config::ocrLanguageJpn;
                this->hide();
                m_shot->show();

                return true;
            }
        }
    }
    // 标准事件处理
    return QMainWindow::eventFilter(watched, event);
}

