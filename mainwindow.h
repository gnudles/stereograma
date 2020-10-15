#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "preset.h"

#define SETTINGS_FILE "settings.bin"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setDepthImage(const QImage & image);
	void setComposePattern(const QImage & image);
	

private slots:
    void on_renderButton_clicked();
	void on_action_obj_triggered();

	void on_action_ply_triggered();

    void on_actionLoad_Texture_triggered();

    void on_actionLoad_Depth_Map_triggered();

    void on_actionCreate_Anaglyph_triggered();

    void on_actionAdd_preset_triggered();

    void on_presetSelect_currentIndexChanged(int index);

    void on_actionSave_Presets_triggered();

    void on_actionReset_triggered();

    void on_actionRename_Preset_triggered();

    void on_actionRemove_preset_triggered();

	void on_actionGenerate_Depth_Map_triggered();
	
    void on_rolling_clicked();

    void on_show_helpers_toggled(bool checked);

private:
    void loadPresets();
    void savePresets();
    Ui::MainWindow *ui;
    Preset *cur_preset;
    QList<Preset*> m_presets;
};

#endif // MAINWINDOW_H
