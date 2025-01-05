#pragma once
#ifndef ES_CORE_COMPONENTS_HELP_COMPONENT_H
#define ES_CORE_COMPONENTS_HELP_COMPONENT_H

#include "GuiComponent.h"
#include "HelpStyle.h"

class ComponentGrid;
class ImageComponent;
class TextureResource;

class HelpComponent : public GuiComponent
{
public:
	HelpComponent(Window* window);

	void clearPrompts();
	void setPrompts(const std::vector<HelpPrompt>& prompts);

	void render(const Transform4x4f& parent) override;
	void setOpacity(unsigned char opacity) override;

	inline HelpStyle& getStyle() { return mStyle; }
	void setStyle(const HelpStyle& style);

	std::shared_ptr<ComponentGrid> getGrid() { return mGrid; };

	virtual bool hitTest(int x, int y, Transform4x4f& parentTransform, std::vector<GuiComponent*>* pResult = nullptr) override;
	virtual bool onMouseClick(int button, bool pressed, int x, int y);

private:
	std::shared_ptr<TextureResource> getIconTexture(const char* name);
	std::map< std::string, std::shared_ptr<TextureResource> > mIconCache;

	std::shared_ptr<ComponentGrid> mGrid;
	void updateGrid();

	std::vector<HelpPrompt> mPrompts;
	HelpStyle mStyle;

	int mHotItem;
};

#endif // ES_CORE_COMPONENTS_HELP_COMPONENT_H
