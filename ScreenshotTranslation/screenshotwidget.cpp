#include "screenshotwidget.h"

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

QRect ScreenshotWidget::getSelectedRect() const
{
    return selectedRect.normalized();
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
    if (event->button() == Qt::LeftButton && isSelecting) {
        selectedRect = QRect(startPos, currentPos).normalized();
        isSelecting = false;
        emit areaSelected(selectedRect);
        this->close();
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
