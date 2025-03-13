#include "screenshotwidget.h"
#include "config.hpp"


ScreenshotWidget::ScreenshotWidget(QWidget *parent)
    :QWidget(parent)
    ,isSelecting(false)
{
    // 设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowState(Qt::WindowFullScreen);

    // 获取所有屏幕的合并尺寸
    QRect totalRect;
    foreach (QScreen *screen, QGuiApplication::screens())
    {
        totalRect = totalRect.united(screen->geometry());
    }
    resize(totalRect.size());
}

ScreenshotWidget::~ScreenshotWidget()
{}

std::pair<QRect,bool> ScreenshotWidget::saveSelectedRect(QString path) const
{
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    qreal scaleFactor = primaryScreen->devicePixelRatio();//屏幕放缩比例（125%即1.25）
    // 获取全屏截图
    QPixmap fullScreen = QGuiApplication::primaryScreen()->grabWindow(0);
    // 100%比例下的rect转换为放缩比例下的rect
    QRect physicalSelectedRect(
        static_cast<int>(m_selectedRect.x() * scaleFactor),
        static_cast<int>(m_selectedRect.y() * scaleFactor),
        static_cast<int>(m_selectedRect.width() * scaleFactor),
        static_cast<int>(m_selectedRect.height() * scaleFactor)
        );
    // 截取选中区域
    QPixmap pixmapSelectedArea = fullScreen.copy(physicalSelectedRect);
    // 保存到文件
    bool isSave = pixmapSelectedArea.save(path);

    return std::move(std::make_pair(physicalSelectedRect,isSave));
}

void ScreenshotWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPos = event->globalPos();
        currentPos = startPos;
        isSelecting = true;
        update();
    }
}

void ScreenshotWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isSelecting) {
        currentPos = event->globalPos();
        update();
    }
}

void ScreenshotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isSelecting)
    {
        //先关闭截图窗口，再发送获取截图信号
        this->close();
        isSelecting = false;
        m_selectedRect = QRect(startPos, currentPos).normalized();
        std::pair<QRect,bool> res = saveSelectedRect(config::saveShotPath);

        if(res.second)  emit areaSelectFinished(res.first); //截图保存完毕再发射信号
    }
}

void ScreenshotWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit canceled();
        close();
    }
}

void ScreenshotWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制半透明黑色背景
    painter.fillRect(rect(), QColor(0, 0, 0, 160));

    if (isSelecting) {
        // 绘制选中区域
        QRect drawRect = QRect(startPos, currentPos).normalized();

        // 绘制白色边框
        painter.setPen(QPen(Qt::white, 2));
        painter.drawRect(drawRect);

        // 绘制半透明背景
        painter.fillRect(drawRect, QColor(255, 255, 255, 50));

        // 显示尺寸信息
        QString sizeText = QString("%1 x %2")
                               .arg(drawRect.width())
                               .arg(drawRect.height());
        QFontMetrics fm(painter.font());
        int textWidth = fm.horizontalAdvance(sizeText);

        painter.setPen(Qt::white);
        painter.drawText(drawRect.bottomRight() + QPoint(-textWidth-10, 20),
                         sizeText);
    }
}
