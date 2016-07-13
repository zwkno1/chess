#include "chesswidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QIcon>
#include <iostream>
#include <QMessageBox>
#include "chess.h"

ChessWidget::ChessWidget(QWidget *parent)
    : QWidget(parent)
    , last_pos_(0)
    , selected_(false)
    , chess_(new Chess)
{
    buildPixmap();
    this->setWindowIcon(QIcon(QPixmap(":/pic/rk.png")));
    this->setWindowTitle("中国象棋");
    this->setFixedSize(pixmaps_[0].size());
}

bool ChessWidget::buildPixmap()
{
    pixmaps_.reserve(16);
    pixmaps_.push_back(QPixmap(":/pic/board.png"));
    pixmaps_.push_back(QPixmap(":pic/mask.png"));
    char color_array[] = { 'r', 'b' };
    char type_array[] = { 'k', 'a', 'b', 'n', 'r', 'c', 'p'};

    for(auto c : color_array)
    {
        for(auto t : type_array)
        {
            QString name = ":/pic/xx.png";
            name[6] = c;
            name[7] = t;
            pixmaps_.push_back(QPixmap(name));
        }
    }
    return true;
}

QPixmap & ChessWidget::getPixmap(char type)
{
    int i;
    switch(type)
    {
    case 16:	i = 1;
        break;
    case 17:
    case 18:	i = 2;
        break;
    case 19:
    case 20:	i = 3;
        break;
    case 21:
    case 22:	i = 4;
        break;
    case 23:
    case 24:	i = 5;
        break;
    case 25:
    case 26:	i = 6;
        break;
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:	i = 7;
        break;
    case 32:	i = 8;
        break;
    case 33:
    case 34:	i = 9;
        break;
    case 35:
    case 36:	i = 10;
        break;
    case 37:
    case 38:	i = 11;
        break;
    case 39:
    case 40:	i = 12;
        break;
    case 41:
    case 42:	i = 13;
        break;
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:	i = 14;
        break;
    default:	i = 0;
    }

    return pixmaps_[i+1];
}

void ChessWidget::paintEvent(QPaintEvent * e)
{
    (void)e;
    QPainter p(this);

    p.drawPixmap(0, 0, pixmaps_[0]);
    if(last_pos_)
    {
        int x, y;
        boardToCoord(last_pos_, x, y);
        p.drawPixmap(x, y, pixmaps_[1]);
    }
    for(int i = 16; i < 48; ++i)
    {
        if(chess_->getPiecePos(i))
        {
            int x, y;
            boardToCoord(chess_->getPiecePos(i), x, y);
            p.drawPixmap(x, y, getPixmap(i));
        }
    }
}

void ChessWidget::mousePressEvent(QMouseEvent *e)
{
    uint8_t i;
    coordToBoard(e->x(), e->y(), i);

    int color = (chess_->getSide() +1)*16;
    if(selected_)
    {
        Move mv = {last_pos_, i, 0};
        if(chess_->move(mv))
        {
            int result = chess_->getChessResult();
            if(result == 0)
            {
                QMessageBox::information(this, "", "红胜");
            }
            else if(result == 1)
            {
                QMessageBox::information(this, "", "黑胜");
            }

            last_pos_ = i;
            selected_ = false;
        }
        else
        {
            if(chess_->getPiece(i) & color)
                last_pos_ = i;
        }
    }
    else
    {
        if(chess_->getPiece(i) & color)
        {
            last_pos_ = i;
            selected_ = true;
        }
    }
    repaint();
}

void ChessWidget::coordToBoard(int x, int y, uint8_t & idx)
{
    x = (x - 40) / 54;
    y = (y - 40) / 54;
    idx = (y + 3)*16 + x + 3;
}

void ChessWidget::boardToCoord(uint8_t idx, int &x, int &y)
{
    x = (idx%16-3)*54+40;
    y = (idx/16-3)*54+40;
}

void ChessWidget::restart()
{
    chess_->init();
    last_pos_ = 0;
    selected_ = false;
    repaint();
}

void ChessWidget::undo()
{
    chess_->undo();
    last_pos_ = 0;
    selected_ = false;
    repaint();
}

void ChessWidget::search()
{
    chess_->search(5);
    chess_->move(chess_->getBestMove());
    repaint();
}

