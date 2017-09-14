#include "EffectIconPanel.h"
#include "sequencer/DragEffectBitmapButton.h"
#include "BitmapCache.h"

//(*InternalHeaders(EffectIconPanel)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(EffectIconPanel)
//*)


#include "effects/EffectManager.h"

BEGIN_EVENT_TABLE(EffectIconPanel,wxPanel)
	//(*EventTable(EffectIconPanel)
	//*)
END_EVENT_TABLE()



void AddEffectPanelButtons(const EffectManager &manager, EffectIconPanel *panel, wxGridSizer *sizer) {

    for (int x = 0; x < manager.size(); x++) {
        DragEffectBitmapButton *BitmapButton = new DragEffectBitmapButton(panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(16,16),
                                                                            wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON38"));
        BitmapButton->SetMinSize(wxSize(16,16));
        BitmapButton->SetMaxSize(wxSize(16,16));
        BitmapButton->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
        BitmapButton->SetEffect(manager[x]);
        sizer->Add(BitmapButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, x);

    }
}


EffectIconPanel::EffectIconPanel(const EffectManager &m, wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) : manager(m)
{
	//(*Initialize(EffectIconPanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	GridSizer1 = new wxGridSizer(0, 5, 0, 0);
	SetSizer(GridSizer1);
	GridSizer1->Fit(this);
	GridSizer1->SetSizeHints(this);

	Connect(wxEVT_SIZE,(wxObjectEventFunction)&EffectIconPanel::OnResize);
	//*)



    GridSizer1->SetCols(10);
    AddEffectPanelButtons(manager, this, GridSizer1);
    
    wxSizeEvent evt;
    OnResize(evt);
    
    GridSizer1->Fit(this);
    GridSizer1->SetSizeHints(this);
}

EffectIconPanel::~EffectIconPanel()
{
	//(*Destroy(EffectIconPanel)
	//*)
}

void EffectIconPanel::OnResize(wxSizeEvent& event) {
    int cnt = manager.size();
    wxSize wsz = event.GetSize();
    if (wsz.GetWidth() <= 10) {
        return;
    }

    double cols = wsz.GetWidth() * cnt;
    cols /= std::max(wsz.GetHeight(), 1);
    cols = std::sqrt(cols);
    int i = std::round(cols);
    GridSizer1->SetCols(i);
    GridSizer1->SetDimension(0, 0, wsz.GetWidth(), wsz.GetHeight());
    GridSizer1->RecalcSizes();
}
