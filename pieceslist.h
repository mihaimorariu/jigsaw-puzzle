#ifndef PIECESLIST_H
#define PIECESLIST_H

#include <QListWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QVector>

class PiecesList : public QListWidget
{
    Q_OBJECT

public:
    PiecesList(QWidget *parent = 0, const int pieceSize = 0);

    void addPiece(QPixmap pixmap);
    void swapPieces(const int index1, const int index2);
    void clearList();

    int getPieceIndex(const int locIndex) const;
    int getPieceLocation(const int pieceIndex) const;

signals:
    void puzzleCompleted();

protected:
    void dropEvent(QDropEvent *event);

private:
    QVector< int > piecesLocations;

    /* Suppose we want to create an NxN puzzle out of a given image. In this case, we discretize the image region
     * into N*N locations, starting from the top, going to the bottom and from left to right, and label
     * the corresponding locations as loc_0, loc_1, ..., loc_(N*N-1). For example, in case of a 3x3 puzzle we
     * would have:
     *
     *      0 1 2
     *      3 4 5
     *      6 7 8
     *
     * We label the puzzle piece the should be placed in location i as piece_i. The piecesLocations vector stores,
     * for each index i, the index of the puzzle piece that is currently placed in loc_i. For example:
     *
     *  - If piecesLocations[0] = 0, then piece_0 is placed in loc_0 (where it should be)
     *  - If piecesLocations[1] = 2, then this piece should be moved one square to the right
     *
     * etc */
};

#endif
