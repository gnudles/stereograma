#ifndef PRESET_H
#define PRESET_H

#include <QString>
#include <QDataStream>
class Preset
{
public:
    Preset(const QString &name);
    Preset(QDataStream & qdata);
    void toStream(QDataStream & qdata);
    inline QString getName() { return m_preset_name;}
    inline void setName(const QString & str) { m_preset_name=str;}
    inline float getEyeSeperation() {return m_eye_seperation;}
    inline float getObserverDistance() {return m_observer_distance;}
    inline float getMinimumDepth() {return m_minimum_depth;}
    inline float getMaximumDepth() {return m_maximum_depth;}
    inline int getDotsPerInch() {return m_dpi;}
    inline int getResultWidth() {return m_result_width;}
    inline int getResultHeight() {return m_result_height;}
    inline bool getIsParallel() {return m_isParallel;}
    inline void setEyeSeperation(float eye_seperation) { m_eye_seperation=eye_seperation;}
    inline void setObserverDistance(float observer_distance) { m_observer_distance=observer_distance;}
    inline void setMinimumDepth(float minimum_depth) { m_minimum_depth=minimum_depth;}
    inline void setMaximumDepth(float maximum_depth) { m_maximum_depth=maximum_depth;}
    inline void setDotsPerInch(int dpi) { m_dpi=dpi;}
    inline void setIsParallel(bool isParallel) { m_isParallel=isParallel;}
    inline void setResultWidth(int width) {m_result_width=width;}
    inline void setResultHeight(int height) {m_result_height=height;}
private:
    QString m_preset_name;
	float m_eye_seperation;
	float m_observer_distance;
	float m_minimum_depth;
	float m_maximum_depth;
    int m_dpi;
    bool m_isParallel;
	bool m_show_helpers;
	bool m_use_image_resolution;
    int m_result_width;
    int m_result_height;

};

#endif // PRESET_H
