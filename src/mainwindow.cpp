#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QFrame>
#include <QHBoxLayout>
#include <QInputDialog>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const int newWinSize)
    : QMainWindow(parent)
{
    /* By default, the number of puzzle pieces per row (puzzle size) is set to 3. Of course, the user
     * can modify this number at any time during the execution, using the Game menu. */

    puzzleSize = 3;
    windowSize = newWinSize;

    /* Set up main window */

    setupMenus();
    setupWidgets();

    setWindowTitle("Jigsaw Puzzle - 3x3");
    setFixedSize(windowSize, windowSize);
    setWindowFlags(Qt::WindowCloseButtonHint);

    QMessageBox::information(0, "Welcome",
                              "Welcome to my Jigsaw Puzzle!\n\n"
                              "To begin, please load an image using the File menu. "
                              "You can then drag a puzzle piece and drop it on top of another one in order to swap their locations. "
                              "At any time, you can change the puzzle size, shuffle the pieces or see the puzzle's solution, using the Game menu.\n\n"
                              "Good luck!",
                              QMessageBox::Ok);
}

void MainWindow::openImage(const QString &path)
{
    QString filePath = path;

    /* Show a file dialog and filter out files that are not images. */

    if (filePath.isNull())
        filePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp)");

    /* If we didn't click the Cancel button, load the selected image. */

    if (!filePath.isEmpty())
    {
        QPixmap newImage;

        if (!newImage.load(filePath))
        {
            QMessageBox::warning(this,
                                  "Open Image",
                                  "Unable to load the selected image. Please try a different file.",
                                  QMessageBox::Cancel);
            return;
        }

        puzzleImage = newImage;
        setupPuzzle();
    }
}

void MainWindow::setupPuzzle()
{
    /* If the puzzle size is changed before loading an image, the puzzleImage variable will be null.
     * Thus, this function will trigger only after the user has loaded an image. */

    if (puzzleImage.isNull())
        return;

    /* Since images that the user can load are most of the time rectangular and vary in size, the
     * program will crop the square of maximum size, located in the center of the original image,
     * and then rescale it to a fixed size. Note that the resulting image will have to be a bit smaller
     * than the size of the main window, since we also need to account for the fact that our puzzle board
     * will be stored in a QHBoxLayout object, whose margins relative to the main window are not zero. */

    int imageSize = qMin(puzzleImage.width(), puzzleImage.height());

    QPixmap puzzleImageResized = puzzleImage.copy((puzzleImage.width() - imageSize) / 2,
                                                 (puzzleImage.height() - imageSize) / 2,
                                                 imageSize, imageSize)
                             .scaled(windowSize - 40, windowSize - 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    /* The size of the puzzle piece is now simply determined by dividing the width (or the height) or the resulting
     * image to the number of puzzle pieces per row. */

    int pieceSize = qFloor(puzzleImageResized.width() / puzzleSize);

    /* Clear the puzzle list and add new pieces */

    piecesList->clearList();

    for (int y = 0; y < puzzleSize; y++)
        for (int x = 0; x < puzzleSize; x++)
        {
            QPixmap pieceImage = puzzleImageResized.copy(x * pieceSize, y * pieceSize, pieceSize, pieceSize);
            piecesList->addPiece(pieceImage);
        }

    /* Once puzzle pieces are added to the list, this method generates random indices (i and j) between 0 and
     * the number of puzzle pieces - 1 and swaps piece placed at location i with piece placed at location j.
     * Also, the greater the number of pieces on the board, the more iterations are employed, to increase the
     * chances that a piece will not remain in place. */

    qsrand(QCursor::pos().x() ^ QCursor::pos().y());

    for (int i = 0; i < piecesList->count(); i++)
    {
        int randomIndex1 = int((double) qrand() / RAND_MAX  * puzzleSize * puzzleSize);
        int randomIndex2 = int((double) qrand() / RAND_MAX  * puzzleSize * puzzleSize);
        piecesList->swapPieces(randomIndex1, randomIndex2);
    }
}

void MainWindow::setCompleted()
{
    /* If puzzle completed, invite user to play another game */

    int userResponse = QMessageBox::question(this,
                                              "Puzzle Completed",
                                              "Congratulations! You have successfully completed the puzzle!\n"
                                              "Would you like to try a new one?",
                                              QMessageBox::Yes | QMessageBox::No);

    /* Set up a new game or quit */

    if (userResponse == QMessageBox::Yes)
        setupPuzzle();
    else
        close();
}

void MainWindow::setupMenus()
{
    /* Create the menu bar and add menus to it */

    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *openAction = fileMenu->addAction("&Load Image");
    openAction->setShortcuts(QKeySequence::Open);

    QAction *quitAction = fileMenu->addAction("&Quit");
    quitAction->setShortcuts(QKeySequence::Quit);

    QMenu *gameMenu = menuBar()->addMenu("&Game");
    QAction *shuffleAction = gameMenu->addAction("S&huffle");
    QAction *changePuzzleSizeAction = gameMenu->addAction("Change &Size");
    QAction *solveAction = gameMenu->addAction("Sol&ve");

    /* Connect signals generated by clicking on the menus to slots that will process them */

    connect(openAction, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(changePuzzleSizeAction, SIGNAL(triggered()), this, SLOT(changePuzzleSize()));
    connect(shuffleAction, SIGNAL(triggered()), this, SLOT(setupPuzzle()));
    connect(solveAction, SIGNAL(triggered()), this, SLOT(solvePuzzle()));
}

void MainWindow::setupWidgets()
{
    /* Create a frame and a layout object that will store our puzzle board */

    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);

    /* Create the puzzle board; the size of an item (a puzzle piece) will be equal to the
     * width (or the height) of the resized version of the image loaded by the user, divided by
     * the number of pieces per row (See the comments in the beginning of setupPuzzle()). */

    piecesList = new PiecesList(this, qFloor((windowSize - 40) / puzzleSize) );

    /* Connect the puzzleCompleted() signal to the slot that will process it */

    connect(piecesList, SIGNAL(puzzleCompleted()), this, SLOT(setCompleted()), Qt::QueuedConnection);

    /* Add piecesList to the layout object and the latter to the main window */

    frameLayout->addWidget(piecesList);
    setCentralWidget(frame);
}

void MainWindow::changePuzzleSize()
{
    bool pressedOk;

    /* Ask for user input */

    QString userInput = QInputDialog::getText(this, "Puzzle Size",
                        QString("Puzzle size is currently set to %1x%1. Please enter the new size (3-6): ").arg(puzzleSize),
                        QLineEdit::Normal, QString::number(puzzleSize), &pressedOk);
    int newPuzzleSize;

    if (pressedOk)
    {
        /* Make sure that the new size is between 3 and 6, inclusive */

        if ((newPuzzleSize = userInput.toInt()) && newPuzzleSize >= 3 && newPuzzleSize <= 6)
        {
            puzzleSize = newPuzzleSize;
            setWindowTitle(QString("Jigsaw Puzzle - %1x%1").arg(puzzleSize));

            /* Restart puzzle */

            setupPuzzle();
        }
        else
            QMessageBox::warning(this, "Puzzle Size",
                                  "Puzzle size must be an integer value ranging from 3 to 6, inclusive!",
                                  QMessageBox::Ok);
    }
}

void MainWindow::solvePuzzle()
{
    if (puzzleImage.isNull())
        return;

    /* When a puzzle is solved, piecesLocations[i] = i (see pieceslist.h). For every location l1, look for
     * the puzzle piece whose index is equal to l1 and determine its position l2. Swap pieces placed at l1
     * and l2 and the correct puzzle piece will now be placed at l1. Repeat the process for the remaining ones. */

    for (int i = 0; i < puzzleSize * puzzleSize; i++)
    {
        int pieceIndex = piecesList->getPieceIndex(i);

        if (pieceIndex == i)
            continue;

        int pieceLocation = piecesList->getPieceLocation(i);
        piecesList->swapPieces(i, pieceLocation);
    }
}
