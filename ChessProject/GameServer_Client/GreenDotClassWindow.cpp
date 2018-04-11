//---------------------------------------------------------------------------
#include <vcl.h>
#include <memory>
#pragma hdrstop
#include "ChessMyClasses.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------



//  green dot functions

void ChessPiece::ShowWhereToMove()
{
        switch(this->GetType())
        {
                case PAWN:
                        if(!this->GetColor())
                        {
                              if(this->PawnMechanic(this->GetY()-1,this->GetX()))
                                        PictureBoard[this->GetY()-1][this->GetX()]->Picture->LoadFromFile(rsGreenDot);
                              if(this->GetY() == 6 and this->PawnMechanic(this->GetY()-2,this->GetX()))
                                        PictureBoard[this->GetY()-2][this->GetX()]->Picture->LoadFromFile(rsGreenDot);
                              // ATAC
                              for(int i = 0; i < PIECES_MAX ; i++)
                                        if(piece[i]->GetGameObjectState() and piece[i]->GetColor() != this->GetColor() and this->GetY()-1 == piece[i]->GetY() and this->GetX()-1 == piece[i]->GetX())
                                                if(this->PawnAttack(piece[i]))
                                                        PictureBoard[this->GetY()-1][this->GetX()-1]->Picture->LoadFromFile(rsGreenDot);
                              for(int i = 0; i < PIECES_MAX ; i++)
                                        if(piece[i]->GetGameObjectState() and piece[i]->GetColor() != this->GetColor() and this->GetY()-1 == piece[i]->GetY() and this->GetX()+1 == piece[i]->GetX())
                                                if(this->PawnAttack(piece[i]))
                                                        PictureBoard[this->GetY()-1][this->GetX()+1]->Picture->LoadFromFile(rsGreenDot);
                              // en passant
                              if(this->GetY() == 3 and (this->PawnMechanic(2,this->GetX()-1) or this->PawnMechanic(2,this->GetX()+1)))
                              {
                                        for(int k = 8; k < TABLE_LENGTH*2 ; k++)
                                                if(ChessRound.en_passant[k] == 2) // conditia sa stim daca nu a stat mai mult decat o runda acolo
                                                        if(piece[k]->GetY() == 3)
                                                                if(this->GetX() - 1 == piece[k]->GetX() or this->GetX() + 1 == piece[k]->GetX())
                                                                        PictureBoard[piece[k]->GetY()-1][piece[k]->GetX()]->Picture->LoadFromFile(rsGreenDot);
                              }
                        }
                        else
                        {
                              if(this->PawnMechanic(this->GetY()+1,this->GetX()))
                                        PictureBoard[this->GetY()+1][this->GetX()]->Picture->LoadFromFile(rsGreenDot);
                              if(this->GetY() == 1 and this->PawnMechanic(this->GetY()+2,this->GetX()))
                                        PictureBoard[this->GetY()+2][this->GetX()]->Picture->LoadFromFile(rsGreenDot);
                              // ATAC
                              for(int i = 0; i < PIECES_MAX ; i++)
                                        if(piece[i]->GetGameObjectState() and piece[i]->GetColor() != this->GetColor() and this->GetY()+1 == piece[i]->GetY() and this->GetX()-1 == piece[i]->GetX())
                                                if(this->PawnAttack(piece[i]))
                                                        PictureBoard[this->GetY()+1][this->GetX()-1]->Picture->LoadFromFile(rsGreenDot);
                              for(int i = 0; i < PIECES_MAX ; i++)
                                        if(piece[i]->GetGameObjectState() and piece[i]->GetColor() != this->GetColor() and this->GetY()+1 == piece[i]->GetY() and this->GetX()+1 == piece[i]->GetX())
                                                if(this->PawnAttack(piece[i]))
                                                        PictureBoard[this->GetY()+1][this->GetX()+1]->Picture->LoadFromFile(rsGreenDot);
                              // en passant
                              if(this->GetY() == 4 and (this->PawnMechanic(5,this->GetX()-1) or this->PawnMechanic(5,this->GetX()+1)))
                              {
                                        for(int k = 0; k < TABLE_LENGTH ; k++)
                                                if(ChessRound.en_passant[k] == 2) // conditia sa stim daca nu a stat mai mult decat o runda acolo
                                                        if(piece[k]->GetY() == 4)
                                                                if(this->GetX() - 1 == piece[k]->GetX() or this->GetX() + 1 == piece[k]->GetX())
                                                                        PictureBoard[piece[k]->GetY()+1][piece[k]->GetX()]->Picture->LoadFromFile(rsGreenDot);
                              }
                        }
                break;
                case KNIGHT:
                {
                        int avaibleMovesY[8] = {-1,-2,-2,-1,1,2,2,1};
                        int avaibleMovesX[8] = {-2,-1,1,2,2,1,-1,-2};
                        for(int iter = 0 ; iter < 8 ; iter++)
                                if(this->GetY() + avaibleMovesY[iter] >= 0 and this->GetY() + avaibleMovesY[iter] < TABLE_LENGTH and this->GetX()+avaibleMovesX[iter] >= 0 and this->GetX()+avaibleMovesX[iter] < TABLE_LENGTH)
                                        if(this->KnightMechanic(this->GetY() + avaibleMovesY[iter], this->GetX() + avaibleMovesX[iter]))
                                                PictureBoard[this->GetY()+avaibleMovesY[iter]][this->GetX()+avaibleMovesX[iter]]->Picture->LoadFromFile(rsGreenDot);
                                        else
                                        {
                                                for(int i = 0 ; i < PIECES_MAX ; i++)
                                                        if(piece[i]->GetGameObjectState() and piece[i]->GetColor() != this->GetColor() and this->GetY() + avaibleMovesY[iter] == piece[i]->GetY() and this->GetX()+avaibleMovesX[iter] == piece[i]->GetX())
                                                               if(this->KnightAttack(piece[i]))
                                                                        PictureBoard[this->GetY()+avaibleMovesY[iter]][this->GetX()+avaibleMovesX[iter]]->Picture->LoadFromFile(rsGreenDot);
                                        }
                }
                break;
                case BISHOP:
                {
                        int AvaibleMovesY[4] = {-1,-1,1,1};
                        int AvaibleMovesX[4] = {-1,1,1,-1};
                        int i = 0;
                        bool stop = false;
                        while(i<4)
                        {
                                if(this->GetY() + AvaibleMovesY[i] >= 0 and this->GetY() + AvaibleMovesY[i] < TABLE_LENGTH and this->GetX()+AvaibleMovesX[i] >= 0 and this->GetX()+AvaibleMovesX[i] < TABLE_LENGTH)
                                        if(!stop and this->BishopMechanic(this->GetY() + AvaibleMovesY[i], this->GetX() + AvaibleMovesX[i]))
                                                PictureBoard[this->GetY()+AvaibleMovesY[i]][this->GetX()+AvaibleMovesX[i]]->Picture->LoadFromFile(rsGreenDot);
                                        else
                                        {
                                                for(int rep = 0 ; rep < PIECES_MAX ; rep++)
                                                        if(piece[rep]->GetGameObjectState() and this->GetY() + AvaibleMovesY[i] == piece[rep]->GetY() and this->GetX()+AvaibleMovesX[i] == piece[rep]->GetX())
                                                                if(this->BishopAttack(piece[rep]))
                                                                {
                                                                        if(!stop and piece[rep]->GetColor() != this->GetColor())
                                                                        {
                                                                                PictureBoard[this->GetY()+AvaibleMovesY[i]][this->GetX()+AvaibleMovesX[i]]->Picture->LoadFromFile(rsGreenDot);
                                                                                stop = true;
                                                                        }
                                                                        else if(piece[rep]->GetColor() == this->GetColor())
                                                                        {
                                                                                stop = true;
                                                                        }
                                                                }
                                        }
                                AvaibleMovesY[i] < 0 ? AvaibleMovesY[i]-- : AvaibleMovesY[i]++;
                                AvaibleMovesX[i] < 0 ? AvaibleMovesX[i]-- : AvaibleMovesX[i]++;
                                if(AvaibleMovesY[i] <= -8 or AvaibleMovesY[i] >= 8 or AvaibleMovesX[i] >= 8 or AvaibleMovesX[i] <= -8)
                                       {i++; stop = false;}
                        }
                }
                break;
                case ROOK:
                {
                        int AvaibleMovesY[4] = {-1,0,1,0};
                        int AvaibleMovesX[4] = {0,1,0,-1};
                        int i = 0;
                        bool stop = false;
                        while(i<4)
                        {
                                if(this->GetY() + AvaibleMovesY[i] >= 0 and this->GetY() + AvaibleMovesY[i] < TABLE_LENGTH and this->GetX()+AvaibleMovesX[i] >= 0 and this->GetX()+AvaibleMovesX[i] < TABLE_LENGTH)
                                        if(!stop and this->RookMechanic(this->GetY() + AvaibleMovesY[i], this->GetX() + AvaibleMovesX[i])) //!board[this->GetY() + AvaibleMovesY[i]][this->GetX() + AvaibleMovesX[i]]->IsGameObjectHere())
                                                PictureBoard[this->GetY()+AvaibleMovesY[i]][this->GetX()+AvaibleMovesX[i]]->Picture->LoadFromFile(rsGreenDot);
                                        else
                                        {
                                                for(int rep = 0 ; rep < PIECES_MAX ; rep++)
                                                        if(piece[rep]->GetGameObjectState() and this->GetY() + AvaibleMovesY[i] == piece[rep]->GetY() and this->GetX()+AvaibleMovesX[i] == piece[rep]->GetX())
                                                                if(this->RookAttack(piece[rep]))
                                                                {
                                                                        if(!stop and piece[rep]->GetColor() != this->GetColor())
                                                                        {
                                                                                PictureBoard[this->GetY()+AvaibleMovesY[i]][this->GetX()+AvaibleMovesX[i]]->Picture->LoadFromFile(rsGreenDot);
                                                                                stop = true;
                                                                        }
                                                                        else if(piece[rep]->GetColor() == this->GetColor())
                                                                        {
                                                                                stop = true;
                                                                        }
                                                                }
                                        }
                                if(AvaibleMovesY[i]!=0)
                                        AvaibleMovesY[i] < 0 ? AvaibleMovesY[i]-- : AvaibleMovesY[i]++;
                                if(AvaibleMovesX[i]!=0)
                                        AvaibleMovesX[i] < 0 ? AvaibleMovesX[i]-- : AvaibleMovesX[i]++;
                                if(AvaibleMovesY[i] <= -8 or AvaibleMovesY[i] >= 8 or AvaibleMovesX[i] >= 8 or AvaibleMovesX[i] <= -8)
                                       {i++; stop = false;}
                        }
                }
                break;
                case QUEEN:
                {
                      int AvaibleMovesY[8] = {-1,0,1,0,-1,-1,1,1};
                      int AvaibleMovesX[8] = {0,1,0,-1,-1,1,1,-1};
                      int i = 0;
                      bool stop = false;
                      while(i<8)
                      {
                                if(this->GetY() + AvaibleMovesY[i] >= 0 and this->GetY() + AvaibleMovesY[i] < TABLE_LENGTH and this->GetX()+AvaibleMovesX[i] >= 0 and this->GetX()+AvaibleMovesX[i] < TABLE_LENGTH)
                                        if(!stop and this->QueenMechanic(this->GetY() + AvaibleMovesY[i], this->GetX() + AvaibleMovesX[i])) //!board[this->GetY() + AvaibleMovesY[i]][this->GetX() + AvaibleMovesX[i]]->IsGameObjectHere())
                                                PictureBoard[this->GetY()+AvaibleMovesY[i]][this->GetX()+AvaibleMovesX[i]]->Picture->LoadFromFile(rsGreenDot);
                                        else
                                        {
                                                for(int rep = 0 ; rep < PIECES_MAX ; rep++)
                                                        if(piece[rep]->GetGameObjectState() and this->GetY() + AvaibleMovesY[i] == piece[rep]->GetY() and this->GetX()+AvaibleMovesX[i] == piece[rep]->GetX())
                                                                if(this->QueenAttack(piece[rep]))
                                                                {
                                                                        if(!stop and piece[rep]->GetColor() != this->GetColor())
                                                                        {
                                                                                PictureBoard[this->GetY()+AvaibleMovesY[i]][this->GetX()+AvaibleMovesX[i]]->Picture->LoadFromFile(rsGreenDot);
                                                                                stop = true;
                                                                        }
                                                                        else if(piece[rep]->GetColor() == this->GetColor())
                                                                        {
                                                                                stop = true;
                                                                        }
                                                                }
                                        }
                                if(AvaibleMovesY[i]!=0)
                                        AvaibleMovesY[i] < 0 ? AvaibleMovesY[i]-- : AvaibleMovesY[i]++;
                                if(AvaibleMovesX[i]!=0)
                                        AvaibleMovesX[i] < 0 ? AvaibleMovesX[i]-- : AvaibleMovesX[i]++;
                                if(AvaibleMovesY[i] <= -8 or AvaibleMovesY[i] >= 8 or AvaibleMovesX[i] >= 8 or AvaibleMovesX[i] <= -8)
                                       {i++; stop = false;}
                      }
                }
                break;
                case KING:
                {
                        int AvaibleMovesY[8] = {0,-1,-1,-1,0,1,1,1};
                        int AvaibleMovesX[8] = {-1,-1,0,1,1,1,0,-1};
                        for(int iter = 0 ; iter < 8 ; iter++)
                        if(this->GetY() + AvaibleMovesY[iter] >= 0 and this->GetY() + AvaibleMovesY[iter] < TABLE_LENGTH and this->GetX()+AvaibleMovesX[iter] >= 0 and this->GetX()+AvaibleMovesX[iter] < TABLE_LENGTH)
                                if(this->CanKingMoveThere(this->GetY() + AvaibleMovesY[iter],this->GetX() + AvaibleMovesX[iter]))
                                        if(!board[this->GetY() + AvaibleMovesY[iter]][this->GetX() + AvaibleMovesX[iter]]->IsGameObjectHere())
                                                PictureBoard[this->GetY()+AvaibleMovesY[iter]][this->GetX()+AvaibleMovesX[iter]]->Picture->LoadFromFile(rsGreenDot);
                                        else
                                        {
                                                for(int i = 0 ; i < PIECES_MAX ; i++)
                                                        if(piece[i]->GetGameObjectState() and piece[i]->GetColor() != this->GetColor() and this->GetY() + AvaibleMovesY[iter] == piece[i]->GetY() and this->GetX()+AvaibleMovesX[iter] == piece[i]->GetX())
                                                                PictureBoard[this->GetY()+AvaibleMovesY[iter]][this->GetX()+AvaibleMovesX[iter]]->Picture->LoadFromFile(rsGreenDot);
                                        }
                        // pt rocade
                        if(!this->GetColor()) // white
                        {
                                // ROCADA MARE WHITE
                                if(piece[24]->GetGameObjectState() and !ChessRound.CastlingPieceMovedWhite[0] and !ChessRound.CastlingPieceMovedWhite[1] and this->GetY()==7 and this->GetX()==4 and !board[this->GetY()][this->GetX()-1]->IsGameObjectHere() and !board[this->GetY()][this->GetX()-2]->IsGameObjectHere() and !board[this->GetY()][this->GetX()-3]->IsGameObjectHere())
                                        if(this->CanKingMoveThere(this->GetY(),this->GetX()-1) and this->CanKingMoveThere(this->GetY(),this->GetX()-2))
                                                if(!this->isKingInCheck())
                                                        PictureBoard[this->GetY()][this->GetX()-2]->Picture->LoadFromFile(rsGreenDot);
                                // ROCADA MICA WHITE
                                if(piece[25]->GetGameObjectState() and !ChessRound.CastlingPieceMovedWhite[0] and !ChessRound.CastlingPieceMovedWhite[2] and this->GetY()==7 and this->GetX()==4 and !board[this->GetY()][this->GetX()+1]->IsGameObjectHere() and !board[this->GetY()][this->GetX()+2]->IsGameObjectHere())
                                        if(this->CanKingMoveThere(this->GetY(),this->GetX()+1) and this->CanKingMoveThere(this->GetY(),this->GetX()+2))
                                                if(!this->isKingInCheck())
                                                        PictureBoard[this->GetY()][this->GetX()+2]->Picture->LoadFromFile(rsGreenDot);
                        }
                        else
                        {
                                // ROCADA MARE BLACK
                                if(piece[26]->GetGameObjectState() and !ChessRound.CastlingPieceMovedBlack[0] and !ChessRound.CastlingPieceMovedBlack[1] and this->GetY()==0 and this->GetX()==4 and !board[this->GetY()][this->GetX()-1]->IsGameObjectHere() and !board[this->GetY()][this->GetX()-2]->IsGameObjectHere() and !board[this->GetY()][this->GetX()-3]->IsGameObjectHere())
                                        if(this->CanKingMoveThere(this->GetY(),this->GetX()-1) and this->CanKingMoveThere(this->GetY(),this->GetX()-2))
                                                if(!this->isKingInCheck())
                                                        PictureBoard[this->GetY()][this->GetX()-2]->Picture->LoadFromFile(rsGreenDot);
                                // ROCADA MICA BLACK
                                if(piece[27]->GetGameObjectState() and !ChessRound.CastlingPieceMovedBlack[0] and !ChessRound.CastlingPieceMovedBlack[2] and this->GetY()==0 and this->GetX()==4 and !board[this->GetY()][this->GetX()+1]->IsGameObjectHere() and !board[this->GetY()][this->GetX()+2]->IsGameObjectHere())
                                        if(this->CanKingMoveThere(this->GetY(),this->GetX()+1) and this->CanKingMoveThere(this->GetY(),this->GetX()+2))
                                                if(!this->isKingInCheck())
                                                        PictureBoard[this->GetY()][this->GetX()+2]->Picture->LoadFromFile(rsGreenDot);
                        }
                }
                break;
        }
}



void ChessPiece::DeleteGreenDot()
{
        for(int i = 0 ; i < TABLE_LENGTH ; i++)
                for(int j = 0 ; j < TABLE_LENGTH ; j++)
                        PictureBoard[i][j]->Picture->LoadFromFile(rsTransparent);
}
