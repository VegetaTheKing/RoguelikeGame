#pragma once

#include "../UI/Button.h"
#include "../UI/Label.h"

class ListSelect : public UIElement
{
private:
	Button _leftArrow;
	Button _rightArrow;
	Label _currSelectionLabel;

	sf::Text _dummyText;
	bool _sthChanged;

	size_t _selectedIndex;
	std::vector<std::string> _values;
public:
	ListSelect(SoundsManager* sounds, TexturesManager* textures);
	ListSelect(ListSelect& other);
	~ListSelect();

	//Left arrow setters
	void AddLeftArrowState(const std::string& name, const std::string& textureName, const sf::FloatRect& rect);
	void RemoveLeftArrowState(const std::string& name);
	void SetLeftArrowSize(const sf::Vector2f& size);
	sf::Transformable* TransformLeftArrow();

	//Right arrow setters
	void AddRightArrowState(const std::string& name, const std::string& textureName, const sf::FloatRect& rect);
	void RemoveRightArrowState(const std::string& name);
	void SetRightArrowSize(const sf::Vector2f& size);
	sf::Transformable* TransformRightArrow();

	//Text setters
	void SetTextSize(const sf::Vector2f& size);
	void SetFont(const sf::Font& font);
	void SetCharacterSize(uint32_t size);
	void SetLetterSpacing(float spacing);
	void SetStyle(uint32_t style);
	void SetFillColor(const sf::Color& color);
	void SetOutlineColor(const sf::Color& color);
	void SetOutlineThickness(float thickness);
	void SetVerticalAlignment(UIElement::Align align);
	void SetHorizontalAlignment(UIElement::Align align);
	sf::Transformable* TransformText();

	//Global setters
	void AddValue(const std::string& value);
	void RemoveValue(size_t index);
	void ClearValues();
	void SetCurrentIndex(size_t index);
	void NextSelection();
	void PreviousSelection();

	//Text getters
	const sf::Font* GetFont() const;
	uint32_t GetCharacterSize() const;
	float GetLetterSpacing() const;
	uint32_t GetStyle() const;
	const sf::Color& GetFillColor() const;
	const sf::Color& GetOutlineColor() const;
	float GetOutlineThickness() const;
	UIElement::Align GetVerticalAlignment() const;
	UIElement::Align GetHorizontalAlignment() const;

	//Global getters
	std::string GetCurrentValue() const;
	size_t GetCurrentIndex() const;
	size_t GetValuesSize() const;
	const std::vector<std::string>& GetValues() const;

	virtual UIElement* clone() override;
	virtual void Update(bool tick, float delta) override;
	virtual void RedrawElement() override;
	virtual void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
	virtual std::vector<sf::Vector2f> GetAllBoundsPoints() const override;
	virtual std::vector<sf::Vector2f> GetDeepestInFocusBoundsPoints() const;
};

