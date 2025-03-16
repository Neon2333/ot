#ifndef AUTOFONTSIZELABEL_H
#define AUTOFONTSIZELABEL_H

#include <QWidget>
#include <QLabel>
#include <QTextDocument>
#include <QResizeEvent>

namespace Ui {
class AutoFontSizeLabel;
}

/*
*   实现字体大小可随着字数自动调整，填充整个label。
*   在label尺寸调整时，也自动调整字体大小。
*/
class AutoFontSizeLabel : public QLabel
{
    Q_OBJECT

public:
    explicit AutoFontSizeLabel(QWidget *parent = nullptr);
    ~AutoFontSizeLabel();

    void adjustFontSize();
    void setText(const QString& text);

private:
    Ui::AutoFontSizeLabel *ui;

protected:
    void resizeEvent(QResizeEvent* ev) override;
};

#endif // AUTOFONTSIZELABEL_H
