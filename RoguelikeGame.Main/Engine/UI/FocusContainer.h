#pragma once

#include "../UI/UIElement.h"
#include "../Helpers/ViewHelper.h"

class FocusContainer : public UIElement
{
private:
	std::map<std::string, UIElement*> _uiElements;

	bool _passFocus;
	bool _passClick;

	bool _sthChanged;

	sf::Color _hoverColor;
	sf::Color _focusColor;
	sf::VertexArray _background;

	void ChangeColor(const sf::Color& col);
public:
	FocusContainer();
	FocusContainer(FocusContainer& other);
	~FocusContainer();

	void AddElement(const std::string& name, UIElement* element);
	void RemoveElement(UIElement* element);
	void RemoveElement(const std::string& name);
	void RenameElement(const std::string& oldName, const std::string& newName);
	UIElement* GetElement(const std::string& name);
	const std::map<std::string, UIElement*>& GetElements() const;

	//Setters
	void SetPassHover(bool pass);
	void SetPassClick(bool pass);
	void SetHoverColor(const sf::Color& color);
	void SetFocusColor(const sf::Color& color);

	//Getters
	bool GetPassFocus() const;
	bool GetPassClick() const;
	const sf::Color& GetHoverColor() const;
	const sf::Color& GetFocusColor() const;
	sf::FloatRect GetElementsGlobalBounds() const;

	// Inherited via UIElement
	virtual UIElement* clone() override;
	virtual void Update(bool tick, float delta) override;
	virtual void RedrawElement() override;
	virtual void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
};
