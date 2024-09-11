#pragma once

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/xLightsSequencer/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/xLightsSequencer/xLights/blob/master/License.txt
 **************************************************************/

#include "Model.h"

class CustomModel : public ModelWithScreenLocation<BoxedScreenLocation>
{
    public:
        CustomModel(wxXmlNode *node, const ModelManager &manager, bool zeroBased = false);
        virtual ~CustomModel();

        void UpdateModel(int width, int height, int depth, const std::vector<std::vector<std::vector<int>>>& modelData);

        [[nodiscard]] virtual const std::vector<std::string>& GetBufferStyles() const override;
        virtual void GetBufferSize(const std::string& type, const std::string& camera, const std::string& transform, int& BufferWi, int& BufferHi, int& BufferDp, int stagger) const override;
        virtual void InitRenderBufferNodes(const std::string &type, const std::string &camera, const std::string &transform,
            std::vector<NodeBaseClassPtr> &Nodes, int &BufferWi, int &BufferHi, int& BufferDp, int stagger, bool deep = false) const override;
        [[nodiscard]] virtual bool Supports3DBuffer() const override {
            return true;
        }

        [[nodiscard]] virtual int GetStrandLength(int strand) const override;
        [[nodiscard]] virtual int MapToNodeIndex(int strand, int node) const override;

        virtual void AddTypeProperties(wxPropertyGridInterface* grid, OutputManager* outputManager) override;
        [[nodiscard]] virtual int OnPropertyGridChange(wxPropertyGridInterface* grid, wxPropertyGridEvent& event) override;

        [[nodiscard]] virtual std::list<std::string> GetFileReferences() override;
        [[nodiscard]] virtual bool CleanupFileLocations(xLightsFrame* frame) override;

        [[nodiscard]] virtual std::string GetStartLocation() const override {
            return "n/a";
        }

        [[nodiscard]] bool IsAllNodesUnique() const;
        [[nodiscard]] long GetCustomWidth() const {
            return parm1;
        }
        [[nodiscard]] long GetCustomHeight() const {
            return parm2;
        }
        [[nodiscard]] long GetCustomDepth() const {
            return _depth;
        }
        void SetCustomWidth(long w);
        void SetCustomHeight(long u);
        void SetCustomDepth(long d);
        [[nodiscard]] virtual int NodesPerString() const override;
        [[nodiscard]] virtual int MapPhysicalStringToLogicalString(int string) const override;

        [[nodiscard]] virtual int GetNumPhysicalStrings() const override;
        [[nodiscard]] bool SupportsChangingStringCount() const override {
            return true;
        };
        [[nodiscard]] bool ChangeStringCount(long count, std::string& message) override;

        [[nodiscard]] std::string GetCustomData() const;
        void SetCustomData(const std::vector<std::vector<std::vector<int>>>& data);

        [[nodiscard]] std::string GetCustomBackground() const {
            return custom_background;
        }
        void SetCustomBackground(std::string background);
        [[nodiscard]] long GetCustomLightness() const;
        void SetCustomLightness(long lightness);

        [[nodiscard]] virtual bool SupportsXlightsModel() override {
            return true;
        }
        [[nodiscard]] virtual bool SupportsExportAsCustom() const override {
            return false;
        }
        [[nodiscard]] virtual bool SupportsWiringView() const override {
            return true;
        }
        [[nodiscard]] virtual bool ImportXlightsModel(wxXmlNode* root, xLightsFrame* xlights, float& min_x, float& max_x, float& min_y, float& max_y) override;
        [[nodiscard]] bool ImportLORModel(std::string const& filename, xLightsFrame* xlights, float& min_x, float& max_x, float& min_y, float& max_y);
        virtual void ExportXlightsModel() override;

        [[nodiscard]] virtual std::string ChannelLayoutHtml(OutputManager* outputManager) override;
        [[nodiscard]] virtual std::string GetNodeName(size_t x, bool def = false) const override;
        [[nodiscard]] virtual std::list<std::string> CheckModelSettings() override;
        [[nodiscard]] virtual int NodesPerString(int string) const override;

        [[nodiscard]] static std::string CustomModelToCompressed(const std::string& customModel);
        [[nodiscard]] static std::string CompressedToCustomModel(const std::string& compressed);
        [[nodiscard]] static std::vector<std::vector<std::vector<int>>> ParseCustomModel(const std::string& customModel);
        [[nodiscard]] static std::vector<std::vector<std::vector<int>>> ParseCompressed(const std::string& compressed);
        [[nodiscard]] static std::vector<std::vector<std::vector<int>>> ParseCustomModelDataFromXml(const wxXmlNode* node);
        [[nodiscard]] static std::string ToCompressed(const std::vector<std::vector<std::vector<int>>>& model);
        [[nodiscard]] static std::string ToCustomModel(const std::vector<std::vector<std::vector<int>>>& model);
        [[nodiscard]] std::vector<std::vector<std::vector<int>>> GetData() const {
            return locations;
        }

    protected:
        virtual void InitModel() override;
        virtual void SetStringStartChannels(bool zeroBased, int NumberOfStrings, int StartChannel, int ChannelsPerString) override;

    private:
        [[nodiscard]] int GetCustomMaxChannel(const std::string& customModel) const;
        void InitCustomMatrix(const std::string& customModel, const std::string& compressed);
        [[nodiscard]] static std::string StartNodeAttrName(int idx)
        {
            return wxString::Format(wxT("String%i"), idx + 1).ToStdString();  // a space between "String" and "%i" breaks the start channels listed in Indiv Start Chans
        }
        [[nodiscard]] std::string ComputeStringStartNode(int x) const;
        [[nodiscard]] int GetCustomNodeStringNumber(int node) const;

        int _depth = 1;
        std::string custom_background;
        int _strings = 1;
        std::vector<int> stringStartNodes;
        std::vector<std::vector<std::vector<int>>> locations;
};
