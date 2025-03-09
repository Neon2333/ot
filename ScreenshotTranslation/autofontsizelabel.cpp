#include "autofontsizelabel.h"
#include "ui_autofontsizelabel.h"

AutoFontSizeLabel::AutoFontSizeLabel(QWidget *parent)
    : QLabel(parent)
    , ui(new Ui::AutoFontSizeLabel)
{
    ui->setupUi(this);

    setAlignment(Qt::AlignCenter);
    setWordWrap(true);
}

AutoFontSizeLabel::~AutoFontSizeLabel()
{
    delete ui;
}

void AutoFontSizeLabel::adjustFontSize()
{
    QString text = this->text();
    if(text.isEmpty())  return;

    QFont font = this->font();
    QRectF bounds = contentsRect(); //获取控件显式content的矩形
    if(bounds.width()<=0||bounds.height()<=0)
    {
        return;
    }

    //二分查找获取最适合字体大小
    qreal minSize = 1.0;
    qreal maxSize = 100;
    qreal optimalSize = minSize;

    QTextDocument doc;
    doc.setDocumentMargin(0);   //去除边距
    doc.setDefaultFont(font);
    doc.setTextWidth(bounds.width());   //设置文本宽度为控件宽度

    const qreal epsilon = 0.5;
    while(maxSize-minSize>epsilon)
    {
        qreal currentSize = (minSize+maxSize)/2.0;
        font.setPointSizeF(currentSize);
        doc.setDefaultFont(font);
        doc.setPlainText(text);
        doc.adjustSize();
        qreal requiredHeight = doc.size().height();
        if(requiredHeight<=bounds.height())
        {
            //当前字体适合，尝试更大
            optimalSize = currentSize;
            minSize = currentSize;
        }
        else
        {
            maxSize = currentSize;
        }
    }

    //最终确定
    font.setPointSizeF(optimalSize);
    setFont(font);
}

void AutoFontSizeLabel::setText(const QString &text)
{
    QLabel::setText(text);
    adjustFontSize();
}

void AutoFontSizeLabel::resizeEvent(QResizeEvent *ev)
{
    QLabel::resizeEvent(ev);
    adjustFontSize();
}

















