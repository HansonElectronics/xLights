#include "AssistPanel.h"
#include "../../xLightsMain.h"

//(*InternalHeaders(AssistPanel)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(AssistPanel)
const long AssistPanel::ID_SCROLLEDWINDOW_Assist = wxNewId();
//*)

BEGIN_EVENT_TABLE(AssistPanel,wxPanel)
	//(*EventTable(AssistPanel)
	//*)
END_EVENT_TABLE()

AssistPanel::AssistPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
: mGridCanvas(nullptr)
{
	//(*Initialize(AssistPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxWANTS_CHARS, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(1, 1, 0, 0);
	ScrolledWindowAssist = new wxScrolledWindow(this, ID_SCROLLEDWINDOW_Assist, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS|wxVSCROLL|wxHSCROLL, _T("ID_SCROLLEDWINDOW_Assist"));
	FlexGridSizer2 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	FlexGridSizer2->AddGrowableRow(0);
	ScrolledWindowAssist->SetSizer(FlexGridSizer2);
	FlexGridSizer2->Fit(ScrolledWindowAssist);
	FlexGridSizer2->SetSizeHints(ScrolledWindowAssist);
	FlexGridSizer1->Add(ScrolledWindowAssist, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	//*)
}

AssistPanel::~AssistPanel()
{
	//(*Destroy(AssistPanel)
	//*)
	if( mGridCanvas != nullptr )
    {
        delete mGridCanvas;
    }
}

void AssistPanel::AdjustSize(wxSize& s)
{
    SetSize(s);
    SetMinSize(s);
    SetMaxSize(s);

	if( mGridCanvas != nullptr )
    {
        mGridCanvas->AdjustSize(s);
        mGridCanvas->Refresh();
    }

    ScrolledWindowAssist->SetSize(s);
    ScrolledWindowAssist->SetMinSize(s);
    ScrolledWindowAssist->SetMaxSize(s);

    ScrolledWindowAssist->FitInside();
    ScrolledWindowAssist->SetScrollRate(5, 5);
    ScrolledWindowAssist->Refresh();

    Refresh();
}

void AssistPanel::SetGridCanvas(xlGridCanvas* canvas)
{
    mGridCanvas = canvas;
    FlexGridSizer2->Add(canvas, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    FlexGridSizer2->Fit(ScrolledWindowAssist);
    FlexGridSizer2->SetSizeHints(ScrolledWindowAssist);
}

void AssistPanel::AddPanel(wxPanel* panel)
{
    mPanel = panel;
    FlexGridSizer2->SetCols(2);
    FlexGridSizer2->Add(mPanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    FlexGridSizer2->Fit(ScrolledWindowAssist);
    FlexGridSizer2->SetSizeHints(ScrolledWindowAssist);
    SetHandlers(this);
}

void AssistPanel::SetEffectInfo(Effect* effect_, xLightsFrame* xlights_parent)
{
	if( mGridCanvas != nullptr )
    {
        mEffect = effect_;
        EffectLayer* layer = mEffect->GetParentEffectLayer();
        Element* elem = layer->GetParentElement();
        std::string model_name = elem->GetName();
        Model *cls = xlights_parent->GetModel(model_name);
        mGridCanvas->SetModel(cls);
        
        int bw, bh;
        cls->GetBufferSize(mEffect->GetSettings().Get("T_CHOICE_BufferStyle", "Default"),
                           mEffect->GetSettings().Get("T_CHOICE_BufferTransform", "None"),
                           bw, bh);
        
        mGridCanvas->SetNumColumns(bw);
        mGridCanvas->SetNumRows(bh);
        mGridCanvas->SetEffect(mEffect);
    }
}

void AssistPanel::SetHandlers(wxWindow *window)
{
    if (window) {
        window->Connect(wxID_ANY,
                        wxEVT_CHAR,
                        wxKeyEventHandler(AssistPanel::OnChar),
                        (wxObject*) NULL,
                        this);
        window->Connect(wxID_ANY,
                        wxEVT_CHAR_HOOK,
                        wxKeyEventHandler(AssistPanel::OnCharHook),
                        (wxObject*) NULL,
                        this);
        window->Connect(wxID_ANY,
                        wxEVT_KEY_DOWN,
                        wxKeyEventHandler(AssistPanel::OnKeyDown),
                        (wxObject*) NULL,
                        this);

        wxWindowList &list = window->GetChildren();
        for (wxWindowList::iterator it = list.begin(); it != list.end(); ++it) {
            wxWindow* pclChild = *it;
            SetHandlers(pclChild);
        }
    }
}

void AssistPanel::OnKeyDown(wxKeyEvent& event)
{
    event.Skip();
}

void AssistPanel::OnChar(wxKeyEvent& event)
{
    event.Skip();
}

void AssistPanel::OnCharHook(wxKeyEvent& event)
{
    if( mEffect != NULL )
    {
        if( mEffect->GetEffectIndex() == EffectManager::eff_PICTURES )
        {
            wxChar uc = event.GetUnicodeKey();
            switch(uc)
            {
                case 'c':
                case 'C':
                case WXK_CONTROL_C:
                    if (event.CmdDown() || event.ControlDown()) {
                       	if( mGridCanvas != nullptr )
                        {
                            mGridCanvas->Copy();
                        }
                        event.StopPropagation();
                    }
                    break;
                case 'v':
                case 'V':
                case WXK_CONTROL_V:
                    if (event.CmdDown() || event.ControlDown()) {
                       	if( mGridCanvas != nullptr )
                        {
                            mGridCanvas->Paste();
                        }
                        event.StopPropagation();
                    }
                    break;
#ifdef __WXMSW__
				case WXK_INSERT:
				case WXK_NUMPAD_INSERT:
					if (event.ControlDown()) // Copy
					{
						if (mGridCanvas != nullptr)
						{
							mGridCanvas->Copy();
						}
						event.StopPropagation();
					}
					else if (GetKeyState(VK_LSHIFT) || GetKeyState(VK_RSHIFT)) // Paste
					{
						if (mGridCanvas != nullptr)
						{
							mGridCanvas->Paste();
						}
						event.StopPropagation();
					}
					break;
#endif
				default:
                    event.Skip();
                    break;
            }
        }
    }
}
