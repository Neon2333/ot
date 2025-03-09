#include "mainwindow.h"
#include <vector>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
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

    installGlobalEventFilter(); //安装事件过滤器，实现按键事件

    m_ocr = new Ocr(std::vector<QString>{"--screenshot"}, config::saveShotPath);
    connect(m_ocr, &Ocr::resReady, this, [&](QString res){
        this->show();
        m_label_textOCR->setText(res);
    });
}

MainWindow::~MainWindow() {}

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
                m_ocr->startOcr();

                return true; // 表示事件已处理
            }
        }
    }
    // 标准事件处理
    return QMainWindow::eventFilter(watched, event);
}

