#include "preset.h"

Preset::Preset(const QString & name)
{
    m_preset_name=name;
    m_eye_seperation=2.5;
    m_observer_distance=20;
    m_minimum_depth=5;
    m_maximum_depth=9;
    m_dpi = 150;
    m_isParallel=true;
    m_result_width=2048;
    m_result_height=1536;
}

Preset::Preset(QDataStream &qdata)
{
    qdata>>m_preset_name;
    qdata>>m_eye_seperation;
    qdata>>m_observer_distance;
    qdata>>m_minimum_depth;
    qdata>>m_maximum_depth;
    qdata>>m_dpi;
    qdata>>m_result_width;
    qdata>>m_result_height;
    qdata>>m_isParallel;
}

void Preset::toStream(QDataStream &qdata)
{
    qdata<<m_preset_name;
    qdata<<m_eye_seperation;
    qdata<<m_observer_distance;
    qdata<<m_minimum_depth;
    qdata<<m_maximum_depth;
    qdata<<m_dpi;
    qdata<<m_result_width;
    qdata<<m_result_height;
    qdata<<m_isParallel;
}
