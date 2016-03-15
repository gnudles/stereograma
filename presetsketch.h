#ifndef PRESETSKETCH_H
#define PRESETSKETCH_H

#include <QWidget>
#include <QPainter>
#include "preset.h"

class PresetSketch : public QWidget
{
    Q_OBJECT
public:
    explicit PresetSketch(QWidget *parent = 0);
	void setPreset(Preset * preset);
signals:

public slots:
    void presetUpdated();
protected:
	void paintEvent(QPaintEvent *);

private:
	float def_width;
	float def_height;
	float eye_size;
    Preset *m_preset;
    QVector<QPointF> m_heights;

};

#endif // PRESETSKETCH_H
