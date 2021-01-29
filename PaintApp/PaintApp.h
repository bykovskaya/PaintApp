#pragma once

#include <QtWidgets/QMainWindow>
#include <QList>
#include <QCloseEvent>
#include "ScribbleArea.h"
#include "ui_PaintApp.h"


class PaintApp : public QMainWindow
{
    Q_OBJECT

public:
    PaintApp();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void open();
    void saveAs();
    void save();
    void about();

private :
    void createActoins();
    void createMenus();
    void createToolbars();
    bool maybeSave();
    bool saveAsFile(); 
    bool saveFile(); 

    ScribbleArea* scrArea;
    QString openedFile;
    
    QToolBar* drawingToolsToolbar;
    QPushButton* lineButton;
    QPushButton* rectButton;
    QPushButton* ellipseButton;
    QPushButton* triangleButton;
    QPushButton* cursorButton;

    QMenu* fileMenu;
    QMenu* optionMenu;
    //QMenu* drawingMenu;
    QMenu* helpMenu;
    
    QAction* openAct;
    QAction* saveAsAct;
    QAction* saveAct;
    QAction* exitAct;
    QAction* clearScreenAct;
    
    QAction* cursorAct;
    QAction* drawLineAct;
    QAction* drawRectAct;
    QAction* drawEllipseAct;
    QAction* drawTriangleAct;
    QAction* aboutAct;
};
