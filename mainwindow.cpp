#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stereomaker.h"

#include "imageviewer.h"
#include "modeldepthviewer.h"
#include "anaglyphmaker.h"
#include "FormulaGen.h"
#include "stringtype.h"
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QDesktopServices>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->presetSelect->addItem(QString("Default"));
    loadPresets();
    ui->depthImage->setBasicImageParent(this);
    ui->depthImage->setFolderSettings("depthmaps");
    ui->depthImage->setSaveTitle("Depth Map");
    ui->composeImage->setBasicImageParent(this);
    ui->composeImage->setFolderSettings("compose");
    ui->composeImage->setSaveTitle("Depth Compose");
    ui->textureImage->setBasicImageParent(this);
    ui->textureImage->setFolderSettings("textures");
    ui->textureImage->setSaveTitle("Texture");
    QObject::connect(ui->presetEdit,SIGNAL(onEdited()),ui->presetSketcher,SLOT(presetUpdated()));
}

MainWindow::~MainWindow()
{
    delete ui;
    Preset *temp;
    foreach (temp,m_presets)
        delete temp;
}

void MainWindow::setDepthImage(const QImage &image)
{
    ui->depthImage->setImage(image);
}
void MainWindow::setComposePattern(const QImage &image)
{
    ui->composeImage->setImage(image);
}


void MainWindow::on_renderButton_clicked()
{

    StereoMaker smaker;
    if (ui->depthImage->getImage().isNull() || ui->textureImage->getImage().isNull())
    {
        QMessageBox::warning(this,"Error","Please first load both texture and depth images");
    }
    else
    {
        QImage depth_image=ui->depthImage->getImage().convertToFormat(QImage::Format_Indexed8,smaker.getGrayScale());
        depth_image=depth_image.scaled(cur_preset->getResultWidth(),cur_preset->getResultHeight());
        if (!ui->composeImage->getImage().isNull())
        {
            float compose_height = ui->composeLevel->value()/100.0;
            float compose_scale = ui->composeScale->value()/128.0;
            QImage compose_image=ui->composeImage->getImage();
            if (compose_scale!=1)
            {
                //compose_image = compose_image.scaledToWidth((int)(compose_image.width()*compose_scale),Qt::FastTransformation);
                compose_image = compose_image.scaled((int)(compose_image.width()*compose_scale),(int)(compose_image.height()*compose_scale),Qt::KeepAspectRatio,Qt::SmoothTransformation);
            }
            compose_image=compose_image.convertToFormat(QImage::Format_Indexed8,smaker.getGrayScale());
            smaker.composeDepth(depth_image,compose_image,compose_height);
        }
		ImageViewer * imview=new ImageViewer(this);
        imview->setWindowTitle("Result");
        imview->setFolderSettings("stereograms");
        imview->setSaveTitle("Stereogram");
        if (!cur_preset->getIsParallel())
            depth_image.invertPixels();
        const QImage *eye_helper_left=0;
        const QImage *eye_helper_right=0;
        bool show_helpers = ui->show_helpers->isChecked();
        bool margin_helpers = show_helpers && ui->margin_helpers->isChecked();
        QImage eye_helper_image_left=ui->helpers_image->getImage();
        QImage eye_helper_image_right;
        if (show_helpers)
        {
            if(!eye_helper_image_left.isNull())
            {
                if (ui->dual_helpers->isChecked())
                {
                    int half_width=eye_helper_image_left.width()/2;
                    int height=eye_helper_image_left.height();
                    eye_helper_image_right=eye_helper_image_left.copy(half_width,0,half_width,height);
                    eye_helper_image_left=eye_helper_image_left.copy(0,0,half_width,height);

                    if (cur_preset->getIsParallel())
                    {
                        eye_helper_left=&eye_helper_image_left;
                        eye_helper_right=&eye_helper_image_right;
                    }
                    else
                    {
                        eye_helper_right=&eye_helper_image_left;
                        eye_helper_left=&eye_helper_image_right;
                    }
                }
                else
                {
                    eye_helper_left=&eye_helper_image_left;
                    eye_helper_right=&eye_helper_image_left;
                }
            }

        }
        QImage result =smaker.render(depth_image, ui->textureImage->getImage(),cur_preset,ui->parallelProgress,eye_helper_right,eye_helper_left,show_helpers,margin_helpers);
        imview->setImage(result);
		imview->show();
    }
}


void MainWindow::on_action_obj_triggered()
{
	QSettings settings;
	QString setting_name="objdir";
	QString cdir=settings.value(setting_name,"models/").toString();

	QString fileName = QFileDialog::getOpenFileName(parentWidget(), "Load Obj file", cdir, tr("OBJ File (*.obj *.OBJ)"));
    Model3D *m3d;
	if(fileName!="")
	{
		settings.setValue(setting_name, QFileInfo(fileName).absoluteDir().absolutePath());
        m3d = new Model3D();
        m3d->LoadObj(fileName);
        ModelDepthViewer * modelviewer=new ModelDepthViewer(this);
        modelviewer->setModel(m3d);
        modelviewer->show();
	}

}


void MainWindow::on_action_ply_triggered()
{
    QSettings settings;
    QString setting_name="objdir";
    QString cdir=settings.value(setting_name,"models/").toString();
    QString fileName = QFileDialog::getOpenFileName(parentWidget(), "Load Ply file", cdir, tr("PLY File (*.ply *.PLY)"));
    Model3D *m3d;
    if(fileName!="")
    {
        settings.setValue(setting_name, QFileInfo(fileName).absoluteDir().absolutePath());
        m3d = new Model3D();
        m3d->LoadPly(fileName);
        ModelDepthViewer * modelviewer=new ModelDepthViewer(this);
        modelviewer->setModel(m3d);
        modelviewer->show();
    }
}

void MainWindow::on_actionLoad_Texture_triggered()
{
    ui->textureImage->openDialog();
}

void MainWindow::on_actionLoad_Depth_Map_triggered()
{
    ui->depthImage->openDialog();
}

void MainWindow::on_actionCreate_Anaglyph_triggered()
{
    AnaglyphMaker * anaglyphWindow=new AnaglyphMaker();
    anaglyphWindow->show();
}

void MainWindow::loadPresets()
{
    cur_preset=new Preset(QString("Default"));
    m_presets.append(cur_preset);
    ui->presetEdit->setPreset(cur_preset);
    ui->presetSketcher->setPreset(cur_preset);
    QFile file(SETTINGS_FILE);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QDataStream in(&file);
    int size;
    in>>size;
    Preset * temp_preset;
    for(;size>0;size--)
    {
        temp_preset=new Preset(in);
        m_presets.append(temp_preset);
        ui->presetSelect->addItem(temp_preset->getName());
    }
    file.close();
}

void MainWindow::savePresets()
{
    QFile file(SETTINGS_FILE);
    if (!file.open(QIODevice::WriteOnly))
        return;
    QDataStream out(&file);
    int size=m_presets.size()-1;
    out<<size;
    int i;
    for(i=1;i<=size;i++)
    {
        m_presets.at(i)->toStream(out);
    }
    file.close();
}
void MainWindow::on_actionAdd_preset_triggered()
{
    StringType stdialog("Select Preset Name",this);
    if (stdialog.exec()==QDialog::Accepted)
    {
        Preset * temp_preset=new Preset(stdialog.getString());
        m_presets.append(temp_preset);
        ui->presetSelect->addItem(temp_preset->getName());
    }
}

void MainWindow::on_presetSelect_currentIndexChanged(int index)
{
    if(index<m_presets.size())
    {
        cur_preset = m_presets.at(index);
        ui->presetEdit->setPreset(cur_preset);
        ui->presetSketcher->setPreset(cur_preset);
    }
}


void MainWindow::on_actionSave_Presets_triggered()
{
    savePresets();
}

void MainWindow::on_actionReset_triggered()
{
    ui->presetSketcher->setPreset(0);
    Preset *temp;
    foreach (temp,m_presets)
        delete temp;
    m_presets.clear();
    int size=ui->presetSelect->count()-1;
    for(;size>0;size--)
    {
        ui->presetSelect->removeItem(size);
    }

    loadPresets();
}

void MainWindow::on_actionRename_Preset_triggered()
{
    int index=ui->presetSelect->currentIndex();
    if(index>0)
    {
        StringType stdialog("Rename Preset",this);
        if (stdialog.exec()==QDialog::Accepted)
        {
            QString input_string=stdialog.getString();
            ui->presetSelect->setItemText(index,input_string);
            m_presets.at(index)->setName(input_string);
        }
    }
}

void MainWindow::on_actionRemove_preset_triggered()
{
    int index=ui->presetSelect->currentIndex();
    if(index>0)
    {
        m_presets.removeAt(index);
        ui->presetSelect->removeItem(index);
    }
}

void MainWindow::on_actionGenerate_Depth_Map_triggered()
{
	FormulaGen * formulaWindow=new FormulaGen(this);
    formulaWindow->show();
}

void MainWindow::on_rolling_clicked()
{
    QDesktopServices::openUrl(QUrl("https://play.google.com/store/apps/details?id=com.github.gnudles.rollingmarbles"));
}

void MainWindow::on_show_helpers_toggled(bool checked)
{

    ui->helpers_image->setDisabled(!checked);
    ui->dual_helpers->setDisabled(!checked);
    ui->margin_helpers->setDisabled(!checked);
}
