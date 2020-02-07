#include "pieceslist.h"
#include "mainwindow.h"

PiecesList::PiecesList(QWidget *parent, const int pieceSize)
    : QListWidget(parent)
{
    /* Set appearance properties for the puzzle board */

    setDragEnabled(true);
    setAcceptDrops(true);
    setAutoScroll(false);
    setSpacing(-3);

    setViewMode(QListView::IconMode);
    setIconSize(QSize(pieceSize, pieceSize));
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setStyleSheet("QListWidget::item { border-style:none }");
}

void PiecesList::addPiece(QPixmap pixmap)
{    
    /* Add a new piece to the puzzle board */

    QListWidgetItem *pieceItem = new QListWidgetItem(this);

    pieceItem->setIcon(QIcon(pixmap));
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled);

    /* Add a new piece index to the piecesLocation vector (see comments in pieceslist.h) */

    piecesLocations.append(this->count() - 1);
}

void PiecesList::swapPieces(const int index1, const int index2)
{
    /* Replace the icons of pieces located at index1 and index2, as well as the corresponding
     * elements in piecesLocations (see comments in pieceslist.h) */

    QListWidgetItem *item1 = this->item(index1);
    QListWidgetItem *item2 = this->item(index2);

    QIcon iconTemp = item1->icon();
    item1->setIcon(item2->icon());
    item2->setIcon(iconTemp);

    int tempLoc = piecesLocations.at(index1);

    piecesLocations.replace(index1, piecesLocations.at(index2));
    piecesLocations.replace(index2, tempLoc);
}

void PiecesList::clearList()
{
    if (!piecesLocations.isEmpty())
        piecesLocations.clear();

    clear();
}

int PiecesList::getPieceIndex(const int index) const
{
    /* Normally, I should check that index >= 0 && index < piecesLocations.count(), but in this
     * case it's safe to leave it out, since we are only working with indexes in this range */

    return piecesLocations.at(index);
}

int PiecesList::getPieceLocation(const int piece_index) const
{
    return piecesLocations.indexOf(piece_index);
}

void PiecesList::dropEvent(QDropEvent *event)
{
    if (event->source() == this)
        event->accept();
    else
        event->ignore();

    /* Get the item we are with the mouse cursor on top of */

    QListWidgetItem *itemMouseOver = this->itemAt(event->pos());

    if (itemMouseOver)
    {
        /* Get the id of the element that is being dragged, but also of the element that we are
         * with the mouse cursor on top of */

        int itemIdFrom = this->row(currentItem());
        int itemIdTo = this->row(itemMouseOver);

        /* Swap the two puzzle pieces */

        swapPieces(itemIdFrom, itemIdTo);
    }

    /* Have we completed the puzzle? If so, then piecesLocations[i] = i for all i (see pieceslist.h)
     * Emit the puzzleCompleted() signal in this case */

    for (int i = 0; i < piecesLocations.count(); i++)
        if (piecesLocations.at(i) != i)
            return;

    emit puzzleCompleted();
}
