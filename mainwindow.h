#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPixmap>
#include <QMainWindow>
#include "pieceslist.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0, const int newWinSize = 480);

public slots:
    void changePuzzleSize();
    void openImage(const QString &path = QString());
    void setupPuzzle();
    void solvePuzzle();

private slots:
    void setCompleted();

private:
    void setupMenus();
    void setupWidgets();

    QPixmap puzzleImage; /* Stores the image loaded by the user */
    PiecesList *piecesList; /* Stores the puzzle board */

    int puzzleSize; /* Stores the number of puzzle pieces per column or row */
    int windowSize; /* Stores the main window's size, expressed in pixels */
};

#endif
