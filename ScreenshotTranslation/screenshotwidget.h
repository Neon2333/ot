#ifndef SCREENSHOTWIDGET_H
#define SCREENSHOTWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QPixmap>

class ScreenshotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenshotWidget(QWidget *parent = nullptr);
    ~ScreenshotWidget();

    QRect getSelectedRect() const;

signals:
    void areaSelected(const QRect& rect);
    void canceled();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *) override;

private:
    bool isSelecting;
    QPoint startPos;
    QPoint currentPos;
    QRect selectedRect;
};

#endif // SCREENSHOTWIDGET_H
