#ifndef INCLUDED_HELLOWORLDAPP_H
#define INCLUDED_HELLOWORLDAPP_H
#include<iostream>
#include<queue>
#include "wx/wx.h"

// ========================================================================================================
// myApp 
// ========================================================================================================
class myApp : public wxApp
{
public:
	virtual bool OnInit();
}; 

// ========================================================================================================
// MyFrame
// ========================================================================================================
class MyCanvas;
class MyFrame: public wxFrame {
public: 
    MyFrame();
  
    MyCanvas* GetCanvas() const { return m_canvas; }
    void SetCanvas(MyCanvas* canvas) { m_canvas = canvas; }

private:
    MyCanvas*       m_canvas;
};
 
// ========================================================================================================
// MyCanvas 
// ========================================================================================================
class DragShape;
class MyCanvas: public wxScrolledWindow
{
public:
    MyCanvas( wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size );
    ~MyCanvas();

    void SetPlayer(DragShape* player) { _player = player;  }
    void SetEnemy(DragShape* enemy) { _enemys.push(enemy);}
    DragShape* GetShape() {
         return _player;
    }

    void DrawShapes(wxDC& dc);
    void ClearShapes();


private:
    wxTimer m_timer;
    DragShape* _player;//Player
    std::queue<DragShape*> _enemys;//Enemys

    wxDECLARE_EVENT_TABLE(); 
    
public:
    void OnPaint( wxPaintEvent &event );
    void OnTimer(wxTimerEvent& event);
    void OnChar(wxKeyEvent& event);

};


// ========================================================================================================
// DragShape
// ========================================================================================================
class DragShape: public wxObject
{
    
public:
    DragShape(const wxBitmap& bitmap);
    virtual ~DragShape(){}

//// Operations 
    virtual bool Draw(wxDC& dc);

//// Accessors
    virtual wxRect GetRect() const { 
        return wxRect(m_pos.x, m_pos.y, 
        m_bitmap.GetWidth(), m_bitmap.GetHeight()); 
    }
    int GetDirection() const{
        return this->direction;
    }
    void GenerateNewDirection(){
        this->direction = rand()%8 + 1;
    }
    void SetNewDirection(int i){
        this->direction = i;
    }
    void ResetCounter(){
        this->counter = 0;
    }
    int GetCounter(){
        return this->counter;
    }
    void IncCounter(){
        ++this->counter;
    }

    virtual wxPoint GetPosition() const { return m_pos; }
    virtual void SetPosition(const wxPoint& pos) { m_pos = pos; }
  
    wxBitmap& GetBitmap() const { return (wxBitmap&) m_bitmap; }
    void SetBitmap(const wxBitmap& bitmap) { m_bitmap = bitmap; }

protected:
    int counter, direction;
    wxPoint     m_pos;
    wxBitmap    m_bitmap;
};


class Pair: public DragShape{
    DragShape *first, *second; 
    public:
        Pair(DragShape* first, DragShape* second);
        ~Pair();
        bool Draw(wxDC& dc);
        wxRect GetRect() const;
        void SetPosition(const wxPoint& pos);
        wxPoint GetPosition() const;

};
#endif