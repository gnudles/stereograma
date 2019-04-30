#ifndef IMAGEFILEDIALOG_H
#define IMAGEFILEDIALOG_H
#include <QFileDialog>
#include <QLabel>

class ImageFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    ImageFileDialog(QWidget *parent):QFileDialog(parent){setOption(QFileDialog::DontUseNativeDialog);}
    void init();
    void deinit();
private:
    QLabel *_preview;
private slots:
    void fileChanged(const QString &file);
};

#endif // IMAGEFILEDIALOG_H
