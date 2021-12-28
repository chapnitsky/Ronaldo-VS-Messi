#include "game.h"
#include <iostream>
#include <queue>
#include "wx/wx.h"
#include <wx/timer.h>

IMPLEMENT_APP(myApp)
 
// ========================================================================================================
// wxApp
// ========================================================================================================
bool myApp::OnInit()
{
    MyFrame *frame = new MyFrame(); 
	frame->Show(true);

 	
    wxImage::AddHandler( new wxPNGHandler );
    wxImage image, enemy_image;

    bool isloadOK = image.LoadFile("messi.png", wxBITMAP_TYPE_PNG);
    bool isEnemyLoadOK = enemy_image.LoadFile("ronaldo.png", wxBITMAP_TYPE_PNG);
	if (isloadOK && isEnemyLoadOK) {
    	DragShape* player = new DragShape(wxBitmap(image));
        DragShape* e1 = new DragShape(wxBitmap(enemy_image));
        DragShape* e2 = new DragShape(wxBitmap(enemy_image));
        DragShape* e3 = new DragShape(wxBitmap(enemy_image));
        DragShape* e4 = new DragShape(wxBitmap(enemy_image));
        DragShape* e5 = new DragShape(wxBitmap(enemy_image));
        e1->SetPosition(wxPoint(250,250));
        e2->SetPosition(wxPoint(360,1));
        e3->SetPosition(wxPoint(1,1));
        e4->SetPosition(wxPoint(280,1));
        e5->SetPosition(wxPoint(1,150));
		player->SetPosition(wxPoint(20, 250));
        frame->GetCanvas()->SetPlayer(player);
        frame->GetCanvas()->SetEnemy(e1);
        frame->GetCanvas()->SetEnemy(e2);
        frame->GetCanvas()->SetEnemy(e3);
        frame->GetCanvas()->SetEnemy(e4);
        frame->GetCanvas()->SetEnemy(e5);
	} 


	frame->Show( true );
	SetTopWindow(frame);
	return true;
}

// ========================================================================================================
// MyFrame
// ========================================================================================================
MyFrame::MyFrame() 
: wxFrame( (wxFrame *)NULL, wxID_ANY, "EX 7",
          wxPoint(20,20), wxSize(470,360) ) 
{
    wxSize sz = GetClientSize(); 
    m_canvas = new MyCanvas( this, wxID_ANY, wxPoint(0,0), wxSize(sz.GetX(),sz.GetY()) );
}

// ========================================================================================================
// MyCanvas
// ========================================================================================================
wxBEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
  EVT_PAINT(MyCanvas::OnPaint)
  EVT_TIMER(wxID_FILE9, MyCanvas::OnTimer)
wxEND_EVENT_TABLE()

// ----------------------------------------------------------------------------
MyCanvas::MyCanvas( wxWindow *parent, wxWindowID id,
                    const wxPoint &pos, const wxSize &size )
        : wxScrolledWindow( parent, id, pos, size, wxSUNKEN_BORDER ) 
        ,  m_timer(this, wxID_FILE9)
{
    m_timer.Start(50);  
    Bind(wxEVT_CHAR_HOOK, &MyCanvas::OnChar, this);
    SetBackgroundColour(* wxWHITE);
    SetCursor(wxCursor(wxCURSOR_ARROW));
}
 
// ----------------------------------------------------------------------------
void MyCanvas::OnTimer(wxTimerEvent& event)
{  
	int size = _enemys.size();
    for(int i = 0; i < size; i++){//Moving and walls collision
        DragShape* temp = _enemys.front();

        switch (temp->GetDirection())
        {
        case 1://UP
            temp->SetPosition(wxPoint(temp->GetPosition().x,temp->GetPosition().y-1));
            break;
        case 2://DOWN
            temp->SetPosition(wxPoint(temp->GetPosition().x,temp->GetPosition().y+1));
            break;
        case 3://RIGHT
            temp->SetPosition(wxPoint(temp->GetPosition().x+1,temp->GetPosition().y));
            break;
        case 4://LEFT
            temp->SetPosition(wxPoint(temp->GetPosition().x-1,temp->GetPosition().y));
            break;
        case 5://UP_RIGHT
            temp->SetPosition(wxPoint(temp->GetPosition().x+1,temp->GetPosition().y-1));
            break;
        case 6://UP_LEFT
            temp->SetPosition(wxPoint(temp->GetPosition().x-1,temp->GetPosition().y-1));
            break;
        case 7://DOWN_RIGHT
            temp->SetPosition(wxPoint(temp->GetPosition().x+1,temp->GetPosition().y+1));
            break;
        case 8://DOWN_LEFT
            temp->SetPosition(wxPoint(temp->GetPosition().x-1,temp->GetPosition().y+1));
            break;
        }
        temp->IncCounter();
        if(temp->GetCounter() == 170){
            temp->GenerateNewDirection();
            temp->ResetCounter();
        }
        wxRect r = temp->GetRect();
        int x = r.GetX(), y = r.GetY(), h = r.GetHeight(), wid = r.GetWidth(), max_x = GetClientSize().GetX(),
        max_y = 330; //Assaf said that there is a problem with the Y of the window because it is about resolution.
        int dir = temp->GetDirection();
        if(x <= 0){//Left Collision
            switch (dir){
                case 4:
                    temp->SetNewDirection(3);
                    break;
                case 6:
                    temp->SetNewDirection(5);
                    break;
                case 8:
                    temp->SetNewDirection(7);
                    break;
            }
        }else if(y <= 0){//Up Collision
            switch(dir){
                case 1:
                    temp->SetNewDirection(2);
                    break;
                case 5:
                    temp->SetNewDirection(7);
                    break;
                case 6:
                    temp->SetNewDirection(8);
                    break;
            }
        }else if(x + wid >= max_x){//Right Collision
            switch(dir){
                case 3:
                    temp->SetNewDirection(4);
                    break;
                case 5:
                    temp->SetNewDirection(6);
                    break;
                case 7:
                    temp->SetNewDirection(8);
                    break;
            }
        }else if(y + h >= max_y){//Down Collision
            switch(dir){
                case 2:
                    temp->SetNewDirection(1);
                    break;
                case 8:
                    temp->SetNewDirection(6);
                    break;
                case 7:
                    temp->SetNewDirection(5);
                    break;
            }
        }
        _enemys.pop();
        _enemys.push(temp);
    }
    
    for(int i = 0; i < _enemys.size(); i++){//Pair checks
        DragShape* check = _enemys.front();
        _enemys.pop();
        bool _paired = false;
        for(int k = 0; k < _enemys.size(); k++){
            DragShape* cross = _enemys.front();
            _enemys.pop();
            if(check->GetRect().Intersects(cross->GetRect())){//Pair-Up
                _paired = true;
                Pair* bunch = new Pair(check, cross);
                _enemys.push(bunch);
                if(_enemys.size() == 1){//Win
                    wxMessageBox( wxT("Game Over - you WIN :-)") );
                    exit(1);
                }
            }else if(_player->GetRect().Intersects(check->GetRect())){//Lose
                wxMessageBox( wxT("Game Over - you LOSE :-(") );
                exit(1);
            }else{
                _enemys.push(cross);
            }
        }
        if(!_paired)
            _enemys.push(check);
    }

    

    Refresh(true);
    Update();
        
}

// ----------------------------------------------------------------------------
void MyCanvas::OnChar(wxKeyEvent& event)
{

    int x = 0, y = 0;
    if ( event.GetUnicodeKey() != WXK_NONE )
        return;
    // It's a special key, deal with all the known ones:
    switch ( event.GetKeyCode() )
    { 
        case WXK_NUMPAD_LEFT:            
        case WXK_LEFT:
            x--;
            break;
        case WXK_NUMPAD_RIGHT:            
        case WXK_RIGHT:
            x++;
            break; 
        case WXK_NUMPAD_UP:            
        case WXK_UP:
            y--;
            break;
        case WXK_NUMPAD_DOWN:            
        case WXK_DOWN:
            y++;
            break;            
    }
 
    _player->SetPosition(wxPoint(_player->GetPosition().x+x, _player->GetPosition().y+y));
    Refresh(true);
    Update();
}


// ----------------------------------------------------------------------------
void MyCanvas::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );
    PrepareDC( dc );
    DrawShapes(dc);
}


// ----------------------------------------------------------------------------
void MyCanvas::DrawShapes(wxDC& dc)
{
    _player->Draw(dc);
    int size = _enemys.size();
    for(int i = 0; i < size; i++){
        DragShape* temp = _enemys.front();
        temp->Draw(dc);
        _enemys.pop();
        _enemys.push(temp);
    }
}


// ----------------------------------------------------------------------------
MyCanvas::~MyCanvas()
{   
   //No need to clear memory as Assaf said so.
}


// ========================================================================================================
// DragShape
// ========================================================================================================
DragShape::DragShape(const wxBitmap& bitmap)
{   
    direction = rand()%8 + 1;
    counter = 0;
    m_bitmap = bitmap;
    m_pos.x = 0;
    m_pos.y = 0;
}

// ========================================================================================================
// Pair
// ========================================================================================================

Pair::Pair(DragShape* one, DragShape* two): DragShape(one->GetBitmap()){
    this->first = one;
    this->second = two;
    this->counter = 0;
    this->direction = one->GetDirection();
    this->second->SetNewDirection(this->direction);
}

wxRect Pair::GetRect() const{
    return (this->first->GetRect().Union(this->second->GetRect())); 
}
void Pair::SetPosition(const wxPoint& pos){
    second->SetPosition(wxPoint(second->GetPosition().x + (pos.x - this->first->GetPosition().x)
     ,second->GetPosition().y + (pos.y - this->first->GetPosition().y)));
    first->SetPosition(pos);
}
wxPoint Pair::GetPosition() const{
    return wxPoint(this->first->GetPosition().x , this->first->GetPosition().y);
}

bool Pair::Draw(wxDC& dc){
    bool first_ok = this->first->Draw(dc);
    bool second_ok = this->second->Draw(dc);
    return first_ok && second_ok;
}

Pair::~Pair(){
    delete first;
    delete second;
}

// ----------------------------------------------------------------------------
bool DragShape::Draw(wxDC& dc)
{
    if (m_bitmap.IsOk())
    {
        wxMemoryDC memDC;
        memDC.SelectObject(m_bitmap);
 
        dc.Blit(m_pos.x, m_pos.y, m_bitmap.GetWidth(), m_bitmap.GetHeight(),
            & memDC, 0, 0, wxCOPY, true);

        return true;
    }
    else
        return false;
}