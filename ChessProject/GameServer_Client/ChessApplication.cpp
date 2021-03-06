/* =============== INCLUDE ============== */
/* <---------------------------------> */
#include <vcl.h>
#pragma hdrstop
#include "ChessApplication.h"
#include "ChessMyClasses.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
/* ====================================== */
/* =============== VAR. GLOBALE ============== */
/* <-------- Obiecte ----------> */
TfMain *fMain;
ChessGame ChessRound;
ChessBoard *board[TABLE_LENGTH][TABLE_LENGTH];
ChessPiece *piece[PIECES_MAX];
TImage *PictureBoard[TABLE_LENGTH][TABLE_LENGTH];
TImage *PicturePiece[PIECES_MAX];
TImage *GameChessBoard;
TMemo *GameLog;
/* <--- const + const. ReadFromFile ---> */
const String ImagePath = "assets\\"; // path-ul unde se afla imaginile
const String rsBoardServer = ImagePath + "chess_table.bmp";  // board-ul specific daca esti server (alb)
const String rsBoardClient = ImagePath + "chess_table_mirror.bmp";  // board-ul specific daca esti client (negru)
const String ImagePiecePath = "ChessPiecePack2\\"; // Piece Pack 2
const String rsPawnW = ImagePath + ImagePiecePath +  "pawnWChess.bmp"; // albe
const String rsRookW = ImagePath + ImagePiecePath +  "rookWChess.bmp";
const String rsKnightW = ImagePath + ImagePiecePath +  "knightWChess.bmp";
const String rsBishopW = ImagePath + ImagePiecePath +  "bishopWChess.bmp";
const String rsKingW = ImagePath + ImagePiecePath +  "kingWChess.bmp";
const String rsQueenW = ImagePath + ImagePiecePath +  "queenWChess.bmp";
const String rsPawnB = ImagePath + ImagePiecePath +  "pawnBChess.bmp"; // negre
const String rsRookB = ImagePath + ImagePiecePath +  "rookBChess.bmp";
const String rsKnightB = ImagePath + ImagePiecePath +  "knightBChess.bmp";
const String rsBishopB = ImagePath + ImagePiecePath +  "bishopBChess.bmp";
const String rsKingB = ImagePath + ImagePiecePath +  "kingBChess.bmp";
const String rsQueenB = ImagePath + ImagePiecePath +  "queenBChess.bmp";
const String rsTransparent = ImagePath + "Transparent.bmp";
const String rsGreenDot = ImagePath + "GreenDot2.bmp";
const String rsPieceClickedLines = ImagePath + "PieceClickedLines.bmp";
/* =============== TFMAIN FUNC. ============== */
//---------------------------------------------------------------------------
__fastcall TfMain::TfMain(TComponent* Owner)
        : TForm(Owner)
{
        // SOCKETS
        ServerSocket->Close();
        ClientSocket->Close();
        // default resolution 1024x600
        Resolution1->Enabled = true;
        //
        this->ClientHeight = 600;
        this->ClientWidth = 1024;
        this->Top = 100;
        this->Left = 180;
        //
        BoardSize = 568;
        BoardDist = BoardSize * 1/25;
        BoardStartY = 74;
        BoardStartX = 73;
        GamelogFontSize = 14;
        GamelogSize = 255;
        GamelogY = BoardDist + 75;
        GamelogX = BoardSize + 75;
        distPiece = BoardSize / 9.5337;
        imageSize = BoardSize / 10.323;
        // initiere imagini
        GameChessBoard = new TImage(this);
        GameChessBoard->Parent = this;
        for(int i = 0 ; i < TABLE_LENGTH ; i++)
                for(int j = 0 ; j < TABLE_LENGTH ; j++)
                {
                        PictureBoard[i][j] = new TImage(this);
                }
        for(int i = 0 ; i < PIECES_MAX ; i++)  
        {
                PicturePiece[i] = new TImage(this);
        }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::FormClose(TObject *Sender, TCloseAction &Action)
{
        ChessRound.OnPlayerExit();
}
//----------------------------------------------------------------------------
void __fastcall TfMain::NewGame1Click(TObject *Sender)
{
        // inchidere sockets
        ServerSocket->Close();
        ClientSocket->Close();
        // INITIALIZARE JOC
        if(!ChessRound.IsGameRoundActive())
        {
                ConnectionForm->Left = this->Left + this->ClientWidth / 8;
                ConnectionForm->Top = this->Top + this->ClientHeight / 6;
                ConnectionForm->ShowModal();
                if(ConnectionForm->ModalResult == mrOk)  // SERVER
                {
                        ServerSocket->Port = StrToInt(ConnectionForm->ServerPortEdit->Text);
                        ServerSocket->Open();
                        ShowMessagePos("Waiting for players ..",this->Left + this->ClientWidth / 2, this->Top + this->ClientHeight / 2);
                }
                if(ConnectionForm->ModalResult == mrYes)  // CLIENT
                {
                        ClientSocket->Address = ConnectionForm->ClientAddressEdit->Text;
                        ClientSocket->Port = StrToInt(ConnectionForm->ClientPortEdit->Text);
                        ClientSocket->Open();
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Exit1Click(TObject *Sender)
{
        ChessRound.OnPlayerExit();
        exit(0);
}
//---------------------------------------------------------------------------

//  PARTEA VIZUALA
//---------------------------------------------------------------------------
void __fastcall TfMain::About1Click(TObject *Sender)
{
        MessageDlgPos("Game and assets created by Barbulescu Adrian!", mtInformation, TMsgDlgButtons() << mbOK, 0,this->Left + this->ClientWidth / 3, this->Top + this->ClientHeight / 2);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Help1Click(TObject *Sender)
{
        const String sbt = "\n";
        String str;
        str += "Press \'Play\' -> \'New Game Round\' and select if you want to HOST the server game room or to CONNECT to a room!";
        str += "\nYou can surrender the match by leaving the game!";
        str += "\nYou can copy the text from the gamelog and paste it where you want!";
        str += sbt;
        str += "\nLearn chess from:  https://www.chess.com/learn-how-to-play-chess";
        str += sbt;
        str += "\nYou can change your game window resolution from \'Window Size\' menu option!";
        str += "\nYou can change your background,gamelog,font color and font size from \'Graphic Interface\' menu option!";
        str += sbt;
        str += "\nRead the Chess Documentation from the Game directory for more informations!";
        MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, this->Left + this->ClientWidth / 6, this->Top + this->ClientHeight / 3);
}
 //---------------------------------------------------------------------------
void __fastcall TfMain::N800x6001Click(TObject *Sender)
{
        this->ClientHeight = 600;
        this->ClientWidth = 800;
        this->Top = 100;
        this->Left = 180;
        //
        BoardSize = 568;
        BoardDist = BoardSize * 1/25;
        BoardStartY = 74;
        BoardStartX = 73;
        GamelogFontSize = 10;
        GamelogSize = 155;
        GamelogY = BoardDist + 45;
        GamelogX = BoardSize + 45;
        distPiece = BoardSize / 9.5337;
        imageSize = BoardSize / 10.323;
}
//---------------------------------------------------------------------------

void __fastcall TfMain::N1024x6001Click(TObject *Sender)
{
        this->ClientHeight = 600;
        this->ClientWidth = 1024;
        this->Top = 100;
        this->Left = 180;
        //
        BoardSize = 568;
        BoardDist = BoardSize * 1/25;
        BoardStartY = 74;
        BoardStartX = 73;
        GamelogFontSize = 14;
        GamelogSize = 255;
        GamelogY = BoardDist + 75;
        GamelogX = BoardSize + 75;
        distPiece = BoardSize / 9.5337;
        imageSize = BoardSize / 10.323;
}
//---------------------------------------------------------------------------

void __fastcall TfMain::N1280x8001Click(TObject *Sender)
{
        this->ClientHeight = 800;
        this->ClientWidth = 1280;
        this->Top = 100;
        this->Left = 180;
        //
        BoardSize = 682;
        BoardDist = BoardSize * 1/20;
        BoardStartY = 104;
        BoardStartX = 100;
        GamelogFontSize = 15;
        GamelogSize = 425;
        GamelogY = BoardDist + 100;
        GamelogX = BoardSize + 100;
        distPiece = BoardSize / 9.5357;
        imageSize = BoardSize / 12.323;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Black1Click(TObject *Sender)
{
        this->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::White1Click(TObject *Sender)
{
        this->Color = clWhite;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Grey1Click(TObject *Sender)
{
        this->Color = clScrollBar;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Blue1Click(TObject *Sender)
{
        this->Color = clHotLight;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Teal1Click(TObject *Sender)
{
        this->Color = clTeal;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Maroon1Click(TObject *Sender)
{
        this->Color = clMaroon;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfMain::Black2Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::White2Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Color = clWhite;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Grey2Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Color = clScrollBar;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Blue2Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Color = clHotLight;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Teal2Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Color = clTeal;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Maroon2Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Color = clMaroon;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfMain::Black3Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::White3Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Font->Color = clWhite;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Grey3Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Font->Color = clScrollBar;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Blue3Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Font->Color = clHotLight;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Teal3Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Font->Color = clTeal;        
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Maroon3Click(TObject *Sender)
{
        if(GameLog)
                GameLog->Font->Color = clMaroon;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfMain::N151Click(TObject *Sender)
{
        if(ChessRound.IsGameRoundActive())
                GameLog->Font->Size = 10;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::N181Click(TObject *Sender)
{
        if(ChessRound.IsGameRoundActive())
                GameLog->Font->Size = 15;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::N182Click(TObject *Sender)
{
        if(ChessRound.IsGameRoundActive())
                GameLog->Font->Size = 18;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::N241Click(TObject *Sender)
{
        if(ChessRound.IsGameRoundActive())
                GameLog->Font->Size = 24;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::N301Click(TObject *Sender)
{
        if(ChessRound.IsGameRoundActive())
                GameLog->Font->Size = 30;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfMain::FormConstrainedResize(TObject *Sender,
      int &MinWidth, int &MinHeight, int &MaxWidth, int &MaxHeight)
{
        MinWidth = 816; MaxWidth = 1296;
        MinHeight = 659; MaxHeight = 859;
}
//---------------------------------------------------------------------------



/************************* SOCKETS CONNECTIONS *************************/

void __fastcall TfMain::ServerSocketAccept(TObject *Sender,
      TCustomWinSocket *Socket)
{
        int players = 0;
        for(int i = 0; i < ServerSocket->Socket->ActiveConnections; i++)
        {
                players++;
                if(players >= 2)
                        ServerSocket->Socket->Connections[i]->SendText("[GETOUT]");
        }
        if(players == 1)
                ChessRound.CreateAndStartChessGame();  // cand serverul accepta pe client
}
//---------------------------------------------------------------------------

void __fastcall TfMain::ClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
        ChessRound.CreateAndStartChessGame();  // cand clientul se conecteaza
}
//---------------------------------------------------------------------------

void __fastcall TfMain::ClientSocketRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
        AnsiString RecText = Socket->ReceiveText();

        if(AnsiSameStr(RecText, "[GETOUT]"))
        {
                ShowMessagePos("There are already enough players for a game! Try again later ..",this->Left + this->ClientWidth / 2, this->Top + this->ClientHeight / 2);
                ClientSocket->Close();
                exit(0);
        }

        if(AnsiSameStr(RecText, "[SURRENDER]"))
        {
                ShowMessagePos("The other player left! You WON!",this->Left + this->ClientWidth / 2, this->Top + this->ClientHeight / 2);
                GameLog->Lines->Add("WHITE has left! BLACK WON!");
                ChessRound.SetGameRoundState(false);
                ServerSocket->Close();
                ClientSocket->Close();
                this->NewGame1->Checked = false;
                this->NewGame1->Enabled = true;
        }

        int PieceSelectedByServer;
        for(int i = 0 ; i < PIECES_MAX ; i++)
                if(AnsiSameStr(RecText , "[ThisPieceSelected]" + IntToStr(i)))
                {
                        ChessPiece del; del.DeleteGreenDot();
                        PictureBoard[piece[i]->GetY()][piece[i]->GetX()]->Picture->LoadFromFile(rsPieceClickedLines);
                }

        bool PieceMovedNotAttacked = false;
        int PieceClickedByServer, PosY, PosX;

        for(int k = 0 ; k < 4 ; k++)
                if(AnsiSameStr(RecText, "[CAN_CASTLING_NR]" + IntToStr(k)))
                {
                        ChessRound.CastlingTypeNumber[k] = true;
                }

        for(int k = 0 ; k < PIECES_MAX ; k++)
                if(AnsiSameStr(RecText, "[EN_PASSANT_MOVE]" + IntToStr(k)))
                {
                        PieceClickedByServer = k;
                        piece[PieceClickedByServer]->EN_PASSANT_PID(PieceClickedByServer);
                        ChessPiece del; del.DeleteGreenDot();
                }

        for(int k = 0 ; k < PIECES_MAX ; k++)
                if(AnsiSameStr(RecText, "[CASTLING_ROOK_MOVE]" + IntToStr(k)))
                {
                        PieceClickedByServer = k;
                        piece[PieceClickedByServer]->CASTLING_ROOK_PID(PieceClickedByServer);
                }

        for(int k = 0 ; k < PIECES_MAX ; k++)
                if(AnsiSameStr(RecText, "[CASTLING_KING_MOVE]" + IntToStr(k)))
                {
                        PieceClickedByServer = k;
                        piece[PieceClickedByServer]->CASTLING_KING_PID(PieceClickedByServer);
                }

        for(int k = 0 ; k < PIECES_MAX ; k++)
                for(int i = 0; i < TABLE_LENGTH; i++)
                        for(int j = 0; j < TABLE_LENGTH; j++)
                                if(AnsiSameStr(RecText,"[MOVE]" + IntToStr(k) + "|" + IntToStr(i) + "|" + IntToStr(j)))
                                {
                                        PieceMovedNotAttacked = true;
                                        PieceClickedByServer = k;
                                        PosY = i;
                                        PosX = j;
                                        piece[PieceClickedByServer]->PieceMovedOnTable(PieceClickedByServer,PosY, PosX);
                                        ChessPiece del; del.DeleteGreenDot();
                                }

        if(!PieceMovedNotAttacked)
        {
                int PieceAttackedByServer;  
                for(int k = 0 ; k < PIECES_MAX ; k++)
                        for(int m = 0 ; m < PIECES_MAX ; m++)
                                if(AnsiSameStr(RecText,"[ATTACK]" + IntToStr(k) + "|" + IntToStr(m)))
                                        {
                                                PieceClickedByServer = k;
                                                PieceAttackedByServer = m;
                                                piece[PieceClickedByServer]->PieceAttackedOnTable(PieceClickedByServer,PieceAttackedByServer);
                                                ChessPiece del; del.DeleteGreenDot();
                                        }
        }

}

//---------------------------------------------------------------------------
void __fastcall TfMain::ServerSocketClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
        AnsiString RecText = Socket->ReceiveText();

        if(AnsiSameStr(RecText, "[SURRENDER]"))
        {
                ShowMessagePos("The other player left! You WON!",this->Left + this->ClientWidth / 2, this->Top + this->ClientHeight / 2);
                GameLog->Lines->Add("BLACK has left! WHITE WON!");
                ChessRound.SetGameRoundState(false);
                ServerSocket->Close();
                ClientSocket->Close();
                this->NewGame1->Checked = false;
                this->NewGame1->Enabled = true;
        }

        int PieceSelectedByServer;
        for(int i = 0 ; i < PIECES_MAX ; i++)
                if(AnsiSameStr(RecText , "[ThisPieceSelected]" + IntToStr(i)))
                {
                        ChessPiece del; del.DeleteGreenDot();
                        PictureBoard[piece[i]->GetY()][piece[i]->GetX()]->Picture->LoadFromFile(rsPieceClickedLines);
                }

        bool PieceMovedNotAttacked = false;
        int PieceClickedByClient, PosY, PosX;

        for(int k = 0 ; k < 4 ; k++)
                if(AnsiSameStr(RecText, "[CAN_CASTLING_NR]" + IntToStr(k)))
                {
                        ChessRound.CastlingTypeNumber[k] = true;
                }

        for(int k = 0 ; k < PIECES_MAX ; k++)
                if(AnsiSameStr(RecText, "[EN_PASSANT_MOVE]" + IntToStr(k)))
                {
                        PieceClickedByClient = k;
                        piece[PieceClickedByClient]->EN_PASSANT_PID(PieceClickedByClient);
                }

        for(int k = 0 ; k < PIECES_MAX ; k++)
                if(AnsiSameStr(RecText, "[CASTLING_ROOK_MOVE]" + IntToStr(k)))
                {
                        PieceClickedByClient = k;
                        piece[PieceClickedByClient]->CASTLING_ROOK_PID(PieceClickedByClient);
                }

        for(int k = 0 ; k < PIECES_MAX ; k++)
                if(AnsiSameStr(RecText, "[CASTLING_KING_MOVE]" + IntToStr(k)))
                {
                        PieceClickedByClient = k;
                        piece[PieceClickedByClient]->CASTLING_KING_PID(PieceClickedByClient);
                }

        for(int k = 0 ; k < PIECES_MAX ; k++)
                for(int i = 0; i < TABLE_LENGTH; i++)
                        for(int j = 0; j < TABLE_LENGTH; j++)
                                if(AnsiSameStr(RecText,"[MOVE]" + IntToStr(k) + "|" + IntToStr(i) + "|" + IntToStr(j)))
                                {
                                        PieceMovedNotAttacked = true;
                                        PieceClickedByClient = k;
                                        PosY = i;
                                        PosX = j;
                                        piece[PieceClickedByClient]->PieceMovedOnTable(PieceClickedByClient,PosY, PosX);
                                        ChessPiece del; del.DeleteGreenDot();
                                }

        if(!PieceMovedNotAttacked)
        {
                int PieceAttackedByClient;
                for(int k = 0 ; k < PIECES_MAX ; k++)
                        for(int m = 0 ; m < PIECES_MAX ; m++)
                                if(AnsiSameStr(RecText,"[ATTACK]" + IntToStr(k) + "|" + IntToStr(m)))
                                        {
                                                PieceClickedByClient = k;
                                                PieceAttackedByClient = m;
                                                piece[PieceClickedByClient]->PieceAttackedOnTable(PieceClickedByClient,PieceAttackedByClient);
                                                ChessPiece del; del.DeleteGreenDot();
                                        }
        }
}
//---------------------------------------------------------------------------

