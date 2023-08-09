#include "MhChannelDialog.h"
#include "MhChannel.h"
#include "MhFeature.h"

//(*InternalHeaders(MhChannelDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

namespace {
constexpr unsigned int NAME_COLUMN = 0;
constexpr unsigned int CHANNEL_COLUMN = 1;
constexpr unsigned int FINE_COLUMN = 2;
constexpr unsigned int MIN_COLUMN = 3;
constexpr unsigned int MAX_COLUMN = 4;
constexpr unsigned int RANGE_COLUMN = 5;
constexpr unsigned int DELETE_COLUMN = 6;
}

//(*IdInit(MhChannelDialog)
const long MhChannelDialog::ID_STATICTEXT_FeatureLabel = wxNewId();
const long MhChannelDialog::ID_BUTTON_AddChannel = wxNewId();
//*)

const long MhChannelDialog::ID_GRID_CHANNELS = wxNewId();

BEGIN_EVENT_TABLE(MhChannelDialog,wxDialog)
	//(*EventTable(MhChannelDialog)
	//*)
EVT_COMMAND(wxID_ANY, EVT_GRID_ROW_CLICKED, MhChannelDialog::OnButton_FeatureClick)
EVT_COMMAND(wxID_ANY, EVT_NAME_CHANGE, MhChannelDialog::OnButton_RenameFeatureClick)
END_EVENT_TABLE()

MhChannelDialog::MhChannelDialog(std::unique_ptr<MhFeature>& _feature,wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
: feature(_feature)
{
	//(*Initialize(MhChannelDialog)
	wxFlexGridSizer* FlexGridSizerGrid;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizerMain = new wxFlexGridSizer(0, 1, 0, 0);
	StaticTextFeatureLabel = new wxStaticText(this, ID_STATICTEXT_FeatureLabel, _("Feature Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_FeatureLabel"));
	wxFont StaticTextFeatureLabelFont(18,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	StaticTextFeatureLabel->SetFont(StaticTextFeatureLabelFont);
	FlexGridSizerMain->Add(StaticTextFeatureLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizerGrid = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizerMain->Add(FlexGridSizerGrid, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer2 = new wxFlexGridSizer(0, 5, 0, 0);
	Button_AddChannel = new wxButton(this, ID_BUTTON_AddChannel, _("Add Channel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_AddChannel"));
	FlexGridSizer2->Add(Button_AddChannel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizerMain->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	FlexGridSizerMain->Add(StdDialogButtonSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizerMain);
	FlexGridSizerMain->Fit(this);
	FlexGridSizerMain->SetSizeHints(this);

	Connect(ID_BUTTON_AddChannel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MhChannelDialog::OnButton_AddChannelClick);
	//*)

    // Setup Grid
    Grid_Channels = new tmGrid(this, ID_GRID_CHANNELS, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE, _T("ID_GRID_CHANNELS"));
    FlexGridSizerGrid->Add(Grid_Channels, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 15);
    Grid_Channels->DisableDragGridSize();
    Grid_Channels->DisableDragRowSize();
    Grid_Channels->DisableDragColSize();
    Grid_Channels->CreateGrid(0, 7);
    Grid_Channels->HideRowLabels();

    Grid_Channels->SetColSize(NAME_COLUMN, 150);
    Grid_Channels->SetColSize(CHANNEL_COLUMN, 50);
    Grid_Channels->SetColSize(FINE_COLUMN, 30);
    Grid_Channels->SetColSize(MIN_COLUMN, 50);
    Grid_Channels->SetColSize(MAX_COLUMN, 50);
    Grid_Channels->SetColSize(RANGE_COLUMN, 250);
    Grid_Channels->SetColSize(DELETE_COLUMN, 23);
    Grid_Channels->SetColLabelValue(NAME_COLUMN, "Name");
    Grid_Channels->SetColLabelValue(CHANNEL_COLUMN, "Channel");
    Grid_Channels->SetColLabelValue(FINE_COLUMN, "Fine");
    Grid_Channels->SetColLabelValue(MIN_COLUMN, "Min");
    Grid_Channels->SetColLabelValue(MAX_COLUMN, "Max");
    Grid_Channels->SetColLabelValue(RANGE_COLUMN, "Function");
    Grid_Channels->SetColLabelValue(DELETE_COLUMN, "");

    std::vector<std::unique_ptr<MhChannel>>& channels = feature->GetChannels();
    for (auto it = channels.begin(); it != channels.end(); ++it) {
        int channel_row {Grid_Channels->GetNumberRows()};
        AddChannel(channel_row, (*it)->GetBaseName());
        // Now add the ranges
        std::vector<std::unique_ptr<MhChannel::MhRange>>& ranges = (*it)->GetRanges();
        int range_row = channel_row+1;
        for (auto it2 = ranges.begin(); it2 != ranges.end(); ++it2, ++range_row) {
            Grid_Channels->AppendRows(1);
            wxGridCellButtonRenderer* new_renderer = new wxGridCellButtonRenderer("");
            Grid_Channels->SetCellRenderer(range_row, DELETE_COLUMN, new_renderer);
            Grid_Channels->SetCellValue(range_row, RANGE_COLUMN, (*it2)->GetName());
            channel_map.push_back(std::make_pair((*it)->GetBaseName(), (*it2)->GetName()));
        }
        Grid_Channels->SetCellSize(channel_row, NAME_COLUMN, ranges.size()+1, 1);
        Grid_Channels->SetCellSize(channel_row, CHANNEL_COLUMN, ranges.size()+1, 1);
        Grid_Channels->SetCellSize(channel_row, FINE_COLUMN, ranges.size()+1, 1);
    }
    FlexGridSizerMain->Fit(this);
    FlexGridSizerMain->SetSizeHints(this);
}

MhChannelDialog::~MhChannelDialog()
{
	//(*Destroy(MhChannelDialog)
	//*)
}

void MhChannelDialog::SetFeatureName(const std::string& text)
{
    StaticTextFeatureLabel->SetLabelText(text);
}

void MhChannelDialog::OnButton_FeatureClick(wxCommandEvent& event)
{
    int row = Grid_Channels->GetGridCursorRow();
    
    auto info = channel_map[row];
    bool is_range = info.second != "";
    std::string selected_channel = info.first;
    std::vector<std::unique_ptr<MhChannel>>& channels = feature->GetChannels();

    if( Grid_Channels->GetGridCursorCol() == DELETE_COLUMN )
    {
        wxString question;
        if( is_range ) {
            question = wxString::Format("Are you sure you want to delete Function: %s?", info.second);
        } else {
            question = wxString::Format("Are you sure you want to delete Channel: %s?", selected_channel);
        }
        if (wxMessageBox(question, "Are you sure?", wxYES_NO | wxCENTER, this) == wxNO)
        {
            return;
        }
        //Grid_Channels->SetCellSize(row, NAME_COLUMN, 1, 1);
        //Grid_Channels->SetCellSize(row, CHANNEL_COLUMN, 1, 1);
        //Grid_Channels->SetCellSize(row, FINE_COLUMN, 1, 1);
        if( is_range ) {
            // deleting one range from channel
            Grid_Channels->DeleteRows(row);
            for (auto it = channels.begin(); it != channels.end(); ++it) {
                if( (*it)->GetBaseName() == selected_channel ) {
                    std::vector<std::unique_ptr<MhChannel::MhRange>>& ranges = (*it)->GetRanges();
                    int num_rows = ranges.size();
                    for (auto it2 = ranges.begin(); it2 != ranges.end(); ++it2) {
                        if( (*it2)->GetName() == info.second ) {
                            ranges.erase(it2);
                        }
                    }
                    channel_map.erase(channel_map.begin()+row);
                    break;
                }
            }
            
        } else {
            // deleting entire channel
            for (auto it = channels.begin(); it != channels.end(); ++it) {
                if( (*it)->GetBaseName() == selected_channel ) {
                    std::vector<std::unique_ptr<MhChannel::MhRange>>& ranges = (*it)->GetRanges();
                    int num_ranges = ranges.size();
                    Grid_Channels->DeleteRows(row);  // delete main channel row
                    channel_map.erase(channel_map.begin()+row);
                    for( int i = 0; i < num_ranges; ++i ) {  // delete all range rows
                        Grid_Channels->DeleteRows(row);
                        channel_map.erase(channel_map.begin()+row);
                    }
                    wxXmlNode* node = (*it)->GetXmlNode();
                    if( node != nullptr ) {
                        feature->GetXmlNode()->RemoveChild(node);
                        delete node;
                    }
                    ranges.clear();
                    channels.erase(it);
                    break;
                }
            }
        }
        FlexGridSizerMain->Layout();
        this->Fit();
    } else if( Grid_Channels->GetGridCursorCol() == RANGE_COLUMN ) {
        for (auto it = channels.begin(); it != channels.end(); ++it) {
            if( (*it)->GetBaseName() == selected_channel ) {
                wxTextEntryDialog dlg(this, "Function Name", "Enter Function Name:");
                OptimiseDialogPosition(&dlg);
                if (dlg.ShowModal()) {
                    if (dlg.GetValue().ToStdString() != "") {
                        std::string new_range = dlg.GetValue().ToStdString();
                        std::vector<std::unique_ptr<MhChannel::MhRange>>& ranges = (*it)->GetRanges();
                        int new_row = row+ranges.size()+1;
                        Grid_Channels->InsertRows(new_row, 1);
                        wxGridCellButtonRenderer* new_renderer = new wxGridCellButtonRenderer("");
                        Grid_Channels->SetCellRenderer(new_row, DELETE_COLUMN, new_renderer);
                        Grid_Channels->SetCellSize(row, NAME_COLUMN, ranges.size()+2, 1);
                        Grid_Channels->SetCellSize(row, CHANNEL_COLUMN, ranges.size()+2, 1);
                        Grid_Channels->SetCellSize(row, FINE_COLUMN, ranges.size()+2, 1);
                        Grid_Channels->SetCellValue(new_row, RANGE_COLUMN, new_range);
                        Grid_Channels->SetCellAlignment(row, NAME_COLUMN, wxALIGN_CENTER, wxALIGN_CENTER);
                        Grid_Channels->SetCellAlignment(row, CHANNEL_COLUMN, wxALIGN_CENTER, wxALIGN_CENTER);
                        Grid_Channels->SetCellAlignment(row, FINE_COLUMN, wxALIGN_CENTER, wxALIGN_CENTER);
                        this->Fit();
                        (*it)->AddRange(new_range);
                        channel_map.insert(channel_map.begin()+new_row, std::make_pair(selected_channel, new_range));
                    }
                }
                break;
            }
        }
    }
}

void MhChannelDialog::OnButton_RenameFeatureClick(wxCommandEvent& event)
{
    int row = Grid_Channels->GetGridCursorRow();
    
    auto info = channel_map[row];
    bool is_range = info.second != "";
    std::string selected_channel = info.first;
    std::vector<std::unique_ptr<MhChannel>>& channels = feature->GetChannels();
    for (auto it = channels.begin(); it != channels.end(); ++it) {
        if( (*it)->GetBaseName() == selected_channel ) {
            std::vector<std::unique_ptr<MhChannel::MhRange>>& ranges = (*it)->GetRanges();
            int col = Grid_Channels->GetGridCursorCol();
            std::string cell_value = Grid_Channels->GetCellValue(row, col);
            switch( col ) {
                case NAME_COLUMN:
                    (*it)->GetXmlNode()->SetName(cell_value);
                    break;
                case CHANNEL_COLUMN:
                    (*it)->SetChannelCoarse(cell_value);
                    break;
                case FINE_COLUMN:
                    (*it)->SetChannelFine(cell_value);
                    break;
                case MIN_COLUMN:
                    for (auto it2 = ranges.begin(); it2 != ranges.end(); ++it2) {
                        if( (*it2)->GetName() == info.second ) {
                            (*it2)->SetRangeMin(cell_value);
                            break;
                        }
                    }
                    break;
                case MAX_COLUMN:
                    for (auto it2 = ranges.begin(); it2 != ranges.end(); ++it2) {
                        if( (*it2)->GetName() == info.second ) {
                            (*it2)->SetRangeMax(cell_value);
                            break;
                        }
                    }
                    break;
                case RANGE_COLUMN:
                    for (auto it2 = ranges.begin(); it2 != ranges.end(); ++it2) {
                        if( (*it2)->GetName() == info.second ) {
                            (*it2)->SetName(cell_value);
                            break;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void MhChannelDialog::OnButton_AddChannelClick(wxCommandEvent& event)
{
    wxTextEntryDialog dlg(this, "Channel Name", "Enter Channel Name:");
    OptimiseDialogPosition(&dlg);
    if (dlg.ShowModal()) {
        if (dlg.GetValue().ToStdString() != "") {

            std::string new_channel = dlg.GetValue().ToStdString();
            std::vector<std::unique_ptr<MhChannel>>& channels = feature->GetChannels();

            // Protect against duplicate channel names
            for (auto it = channels.begin(); it != channels.end(); ++it) {
                if( (*it)->GetBaseName() == new_channel ) {
                    wxMessageBox(wxString::Format("Channel name %s already exists", new_channel), _("ERROR"));
                    return;
                }
            }

            int num_rows {Grid_Channels->GetNumberRows()};
            AddChannel(num_rows, new_channel);

            std::string node_name = new_channel;
            wxXmlNode* new_node = new wxXmlNode(wxXML_ELEMENT_NODE, node_name);
            feature->GetXmlNode()->AddChild(new_node);
            std::unique_ptr<MhChannel> newChannel(new MhChannel(new_node, new_channel));
            feature->GetChannels().push_back(std::move(newChannel));
        }
        this->Fit();
    }
}

void MhChannelDialog::AddChannel(int row, const std::string& name)
{
    Grid_Channels->AppendRows(1);
    Grid_Channels->SetCellValue(row, NAME_COLUMN, name);
    Grid_Channels->SetCellAlignment(row, NAME_COLUMN, wxALIGN_CENTER, wxALIGN_CENTER);
    Grid_Channels->SetCellAlignment(row, CHANNEL_COLUMN, wxALIGN_CENTER, wxALIGN_CENTER);
    Grid_Channels->SetCellAlignment(row, FINE_COLUMN, wxALIGN_CENTER, wxALIGN_CENTER);
    // Add buttons
    wxGridCellButtonRenderer* new_renderer = new wxGridCellButtonRenderer("Add New Function");
    Grid_Channels->SetCellRenderer(row, RANGE_COLUMN, new_renderer);
    new_renderer = new wxGridCellButtonRenderer("");
    Grid_Channels->SetCellRenderer(row, DELETE_COLUMN, new_renderer);
    channel_map.push_back(std::make_pair(name, ""));
}
