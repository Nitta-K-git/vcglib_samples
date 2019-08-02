
#define OPENMESHAPPS_MESHVIEWERWIDGET_CC

//== INCLUDES =================================================================

#include "MeshViewerWidget.hh"


//== IMPLEMENTATION ========================================================== 

/// default constructor
MeshViewerWidget::MeshViewerWidget(QWidget* parent)  : MeshViewerWidgetT<MyMesh>(parent)
{}

 void MeshViewerWidget::open_mesh_gui(QString fname)
{
    // OpenMesh::Utils::Timer t;
    // t.start();
    // if ( fname.isEmpty() || !open_mesh(fname.toLocal8Bit(), _options) )
    // {
    //     QString msg = "Cannot read mesh from file:\n '";
    //     msg += fname;
    //     msg += "'";
    //     QMessageBox::critical( nullptr, windowTitle(), msg);
    // }
    // t.stop();
    // std::cout << "Loaded mesh in ~" << t.as_string() << std::endl;
    
}

void MeshViewerWidget::open_texture_gui(QString fname)
{
    if ( fname.isEmpty() || !open_texture( fname.toLocal8Bit() ) )
    {
        QString msg = "Cannot load texture image from file:\n '";
        msg += fname;
        msg += "'\n\nPossible reasons:\n";
        msg += "- Mesh file didn't provide texture coordinates\n";
        msg += "- Texture file does not exist\n";
        msg += "- Texture file is not accessible.\n";
        QMessageBox::warning( nullptr, windowTitle(), msg );
    }
}

void MeshViewerWidget::query_open_mesh_file() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open mesh file"),
        tr(""),
        tr("OBJ Files (*.obj);;"
        "OFF Files (*.off);;"
        "STL Files (*.stl);;"
        "All Files (*)"));
    if (!fileName.isEmpty())
        open_mesh_gui(fileName);
}

void MeshViewerWidget::query_open_texture_file() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open texture file"),
        tr(""),
        tr("PNG Files (*.png);;"
        "BMP Files (*.bmp);;"
        "GIF Files (*.gif);;"
        "JPEG Files (*.jpg);;"
        "TIFF Files (*.tif);;"
        "All Files (*)"));
    if (!fileName.isEmpty())
        open_texture_gui(fileName);
}

//=============================================================================

