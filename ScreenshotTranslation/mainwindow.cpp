#include "mainwindow.h"
#include <vector>
#include <QCoreApplication>
#include "common.hpp"


MainWindow::MainWindow(QWidget *parent)
{
    initUI();

    setOcrLanguage(config::cfg.initOcrLanguage);
    setTransLanguage(config::cfg.initTransLanguage);

    m_shot = new ScreenshotWidget;
    m_shot->hide();

    m_pronounce = new Pronounce();

    installGlobalEventFilter(); //安装事件过滤器，实现按键事件

    //正常启动截图
    connect(this, &MainWindow::ocrLanguageNotChanged, this, [this](){
        this->hide();
        m_shot->show();
    });

    //截图结束，开始识别
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
        this->show();

        QString resTmp=res.mid(0, res.size()-4);//res末尾有"\r\n\r\n"，这是ocr识别结果所带
        m_textedit_textOCR->setText(resTmp);
        m_translate->doTranslate(resTmp.toStdString(), m_ocrLanguage, m_transLanguage);
    });

    //识别错误
    connect(m_ocr, &Ocr::resStatus, this, [&](QString err){
        this->show();
        setStatus(err);
    });

    //翻译内容写入控件
    m_translate = new Translate();
    connect(m_translate, &Translate::translateFinished, this, [&](QString rslt){
        m_textedit_textTran->setText(rslt);
    });

    //翻译状态（错误）
    connect(m_translate, &Translate::setStatus, this, [&](QString rslt){
        setStatus(rslt);
    });

    //换识别语言重启ocr后，启动截图
    connect(m_ocr, &Ocr::quitOcrFinished, this, [this](){
        this->hide();
        m_shot->show();//显示截图控件，按下鼠标进行截图
    });

    //请求读音错误
    connect(m_pronounce, &Pronounce::setStatus, this, [&](QString status){
        setStatus(status);
    });


    connect(m_btn_volume, &QPushButton::clicked, this, [this](){
        switchModel();
        // m_label_phonetic->setText("[fʌk]");
        m_pronounce->getSoundFile(m_ocrLanguage, m_textedit_textOCR->toPlainText());
        // m_pronounce->getSoundFile(Language::en, QString("how are you"));
    });

    connect(m_pronounce, &Pronounce::pronouncationAudioDownloaded, this, [this](QString audioName){
        qout<<audioName;
        m_pronounce->playPronounationFile(audioName);
    });

    connect(m_btn_justTranslate, &QPushButton::clicked, this, [this](){
        m_translate->doTranslate(m_textedit_textOCR->toPlainText().toStdString(), m_ocrLanguage, m_transLanguage);
    });
}

MainWindow::~MainWindow()
{
    Close();
    spdlog::info("【" + nowStr() + "】	"+ "ot quit");
}

void MainWindow::setStatus(QString status)
{
    m_label_status->setText(status);
}

void MainWindow::switchModel()
{
    if(curModel==Model::expand)
    {
        curModel=Model::shrink;
        // m_label_mode->setText(QString("shrink"));
        m_label_status->setText(QString("shrink"));
        qDebug()<<"shrink";
    }
    else
    {
        curModel=Model::expand;
        m_label_status->setText(QString("expand"));
        qDebug()<<"expand";
    }
}

void MainWindow::initUI()
{
    // setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/images/mainIcon.jpg"));

    resize(600,150);
    move(800,300);

    m_textedit_textOCR = new QTextEdit(this);
    m_textedit_textTran = new QTextEdit(this);

    m_btn_volume = new QPushButton(this);
    m_btn_volume->setFixedSize(40,40);
    m_btn_volume->setIcon(QIcon(":/icons/volume.svg"));
    m_btn_volume->setIconSize(QSize(20,20));

    m_btn_justTranslate = new QPushButton(this);
    m_btn_justTranslate->setFixedSize(40,40);
    m_btn_justTranslate->setIcon(QIcon(":/icons/translate.svg"));
    m_btn_justTranslate->setIconSize(QSize(20,20));

    m_btn_command = new QPushButton(this);
    m_btn_command->setFixedSize(40,40);
    m_btn_command->setIcon(QIcon(":/icons/command.svg"));
    m_btn_command->setIconSize(QSize(20,20));

    // m_label_mode = new QLabel(this);
    // m_label_mode->setFixedSize(40,40);
    // m_label_mode->setText(QString("expand"));
    // m_label_mode->setAlignment(Qt::AlignCenter);

    m_hlayTopCenter = new QVBoxLayout;
    m_hlayTopCenter ->setSpacing(20);
    m_hlayTopCenter ->setContentsMargins(4,4,4,4);
    m_hlayTopCenter->addWidget(m_btn_volume);
    m_hlayTopCenter->addWidget(m_btn_justTranslate);
    m_hlayTopCenter->addWidget(m_btn_command);
    // m_hlayTopCenter->addWidget(m_label_mode);

    m_widget_center = new QWidget(this);
    m_widget_center->setLayout(m_hlayTopCenter);

    m_hlayTop = new QHBoxLayout;
    m_hlayTop->setSpacing(4);
    m_hlayTop->setContentsMargins(0,0,0,0);
    m_hlayTop->addWidget(m_textedit_textOCR, 1);
    m_hlayTop->addWidget(m_widget_center);
    m_hlayTop->addWidget(m_textedit_textTran, 1);


    m_label_ocrLanguage = new QLabel(this);
    m_label_ocrLanguage->setAlignment(Qt::AlignCenter);
    m_label_ocrLanguage->setFixedSize(50,20);

    m_label_status = new QLabel(this);
    m_label_status->setAlignment(Qt::AlignCenter);
    m_label_status->setFixedSize(200,20);

    m_label_transLanguage = new QLabel(this);
    m_label_transLanguage->setAlignment(Qt::AlignCenter);
    m_label_transLanguage->setFixedSize(50,20);

    int widthPlaceHolder = 0.5*(this->width()-m_label_status->width()-m_label_ocrLanguage->width()-m_label_transLanguage->width());
    m_placeholderLeft = new QSpacerItem(widthPlaceHolder, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_placeholderRight = new QSpacerItem(widthPlaceHolder, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_hlayBottom = new QHBoxLayout;
    m_hlayBottom->setSpacing(0);
    m_hlayBottom->setContentsMargins(0,0,0,0);
    m_hlayBottom->addWidget(m_label_ocrLanguage, 1);
    m_hlayBottom->addSpacerItem(m_placeholderLeft);
    m_hlayBottom->addWidget(m_label_status);
    m_hlayBottom->addSpacerItem(m_placeholderRight);
    m_hlayBottom->addWidget(m_label_transLanguage);

    m_vlay=new QVBoxLayout;
    m_vlay->addLayout(m_hlayTop);
    m_vlay->addLayout(m_hlayBottom);
    m_vlay->setSpacing(4);
    m_vlay->setContentsMargins(4,4,4,4);

    //this->setStyleSheet(QString("QWidget{backround-color:white;}"));
    QPalette pal(this->palette());
    pal.setColor(QPalette::Window, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    m_textedit_textOCR->setStyleSheet(QString("QTextEdit{border-radius:5px;"
                                            "background-color:rgba(148,165,141,255);"
                                            "color:rgba(255,255,255,255);}"
                                              ));

    m_btn_volume->setStyleSheet(QString("QPushButton{border:none;border-radius:20px;background:white;color:black;}"
                                        "QPushButton:hover{background:lightblue;}"
                                        ));

    m_btn_justTranslate->setStyleSheet(QString("QPushButton{border:none;border-radius:20px;background:white;color:black;}"
                                                "QPushButton:hover{background:lightblue;}"
                                               ));

    m_btn_command->setStyleSheet(QString("QPushButton{border:none;border-radius:20px;background:white;color:black;}"
                                        "QPushButton:hover{background:lightblue;}"
                                        "QPushButton:pressed{width:38px; height:38px;}"
                                         ));

    m_widget_center->setStyleSheet(QString("QWidget{border:1px;border-radius:5px;background-color:rgba(225,209,186,255);font:11px;}"));


    m_textedit_textTran->setStyleSheet(QString("QTextEdit{border-radius:5px;"
                                            "background-color:rgba(251,210,106,255);"
                                            "color:rgba(255,255,255,255);}"));

    m_label_ocrLanguage->setStyleSheet(QString("QLabel{border-radius:5px;"
                                                 "background-color:rgba(81,118,147,255);"
                                                 "color:rgba(255,255,255,255);}"));

    m_label_transLanguage->setStyleSheet(QString("QLabel{border-radius:5px;"
                                               "background-color:rgba(81,118,147,255);"
                                               "color:rgba(255,255,255,255);}"));

    m_label_status->setStyleSheet(QString("QLabel{border-radius:5px;"
                                          "background-color:rgba(129,216,208,255);"
                                          "color:rgba(255,255,255,255);}"));

    // m_label_mode->setStyleSheet(QString("QLabel{border-radius:5px;"
    //                                       // "background-color:rgba(245,236,231,255);"
    //                                       "color:rgba(255,255,255,255);}"));


    content = new QWidget(this);
    content->setLayout(m_vlay);
    this->setCentralWidget(content);
    this->adjustSize();

    expandRect = this->geometry();
    int shrinkX = expandRect.x() + expandRect.width()*0.5;
    shrinkRect = QRect(shrinkX, expandRect.y(), 0, expandRect.height());

}

void MainWindow::installGlobalEventFilter()
{
    qApp->installEventFilter(this);
}

void MainWindow::setOcrLanguage(Language language)
{
    if(m_ocrLanguage==language)
    {
        emit ocrLanguageNotChanged();
        return;
    }
    m_ocrLanguage=language;

    switch(m_ocrLanguage)
    {
    case Language::zh:
        m_label_ocrLanguage->setText("中");
        break;
    case Language::en:
        m_label_ocrLanguage->setText("英");
        break;
    case Language::jp:
        m_label_ocrLanguage->setText("日");
        break;
    default: break;
    }

    if(m_ocr) m_ocr->quitOcr(); //修改语言后退出ocr，待startOcr重启
}

void MainWindow::setTransLanguage(Language language)
{
    if(m_transLanguage==language)
    {
        return;
    }
    m_transLanguage=language;

    switch(m_transLanguage)
    {
    case Language::zh:
        m_label_transLanguage->setText("中");
        break;
    case Language::en:
        m_label_transLanguage->setText("英");
        break;
    case Language::jp:
        m_label_transLanguage->setText("日");
        break;
    default: break;
    }
}


Language MainWindow::ocrLanguage()
{
    return m_ocrLanguage;
}

void MainWindow::Close()
{
    if(m_ocr)
    {
        m_ocr->quitOcr();
        m_ocr->useConfig(Language::zh);//设定下次启动ocr为zh
        m_ocr->deleteLater();
        m_ocr=nullptr;
    }
    if(m_shot)
    {
        m_shot->deleteLater();
        m_shot=nullptr;
    }
    if(m_translate)
    {
        m_translate->deleteLater();
        m_translate=nullptr;
    }
    if(m_pronounce)
    {
        m_pronounce->deleteLater();
        m_pronounce=nullptr;
    }
    QCoreApplication::quit();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        // 处理按键事件
        if(keyEvent->modifiers() & Qt::AltModifier)
        {
            if(keyEvent->key() == Qt::Key_4)
            {
                close();
            }
            else if(keyEvent->key() == Qt::Key_1)
            {
                setOcrLanguage(Language::zh);
                setTransLanguage(Language::zh);
                return true; // 表示事件已处理
            }
            else if(keyEvent->key() == Qt::Key_2)
            {
                setOcrLanguage(Language::en);
                setTransLanguage(Language::zh);
                return true;
            }
            else if(keyEvent->key() == Qt::Key_3)
            {
                setOcrLanguage(Language::jp);
                setTransLanguage(Language::zh);
                return true;
            }
        }
    }
    // 标准事件处理
    return QMainWindow::eventFilter(watched, event);
}

