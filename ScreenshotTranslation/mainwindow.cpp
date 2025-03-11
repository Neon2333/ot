#include "mainwindow.h"
#include <vector>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
{
    initUI();

    m_shot = new ScreenshotWidget;
    m_shot->hide();

    installGlobalEventFilter(); //安装事件过滤器，实现按键事件

    m_ocr = new Ocr(std::vector<QString>{"--screenshot"});
    connect(m_shot, &ScreenshotWidget::areaSelected, this, [&](const QRect& rect){
        // 获取全屏截图
        QPixmap fullScreen = QGuiApplication::primaryScreen()->grabWindow(0);
        // 截取选中区域
        QPixmap selectedArea = fullScreen.copy(rect);
        // 保存到文件
        selectedArea.save(config::saveShotPath);

        qDebug() << "Saved area:" << rect;


        //识别
        int x=rect.x();
        int y=rect.y();
        int w=rect.width();
        int h=rect.height();
        QString cmdRect = QString(" rect=%1,%2,%3,%4").arg(x).arg(y).arg(w).arg(h);
        // m_ocr->setCommands(std::vector<QString>{"--screenshot"});
        m_ocr->setCommands(std::vector<QString>{"--screenshot", " screen=0", cmdRect});
        // m_ocr->hideOcrWindow(); //禁止弹出ocr主界面
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

    const QString qssStr = QString("QLabel{background-color:lightblue;}");


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
        if(keyEvent->modifiers() & Qt::ControlModifier)
        {
            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                this->hide();

                m_shot->show();

                // m_ocr->startOcr();

                return true; // 表示事件已处理
            }
        }
    }
    // 标准事件处理
    return QMainWindow::eventFilter(watched, event);
}

