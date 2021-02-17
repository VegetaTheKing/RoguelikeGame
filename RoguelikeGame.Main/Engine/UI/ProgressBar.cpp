#include "ProgressBar.h"

ProgressBar::ProgressBar()
{
	_progressBarSteps.clear();
	_backgroundLayers.clear();

	_currentValue = 3.F;
	_maxValue = 3.F;

	_prevValue = _currentValue;
	_prevMaxValue = _maxValue;

	_sthChanged = true;
}

ProgressBar::~ProgressBar()
{
}

ProgressBar& ProgressBar::operator=(ProgressBar& other)
{
	_currentValue = other._currentValue;
	_maxValue = other._maxValue;
	_prevValue = other._prevValue;
	_prevMaxValue = other._prevMaxValue;
	_progressBarSteps = other._progressBarSteps;
	_backgroundLayers = other._backgroundLayers;
	_progressBarStepsPos = other._progressBarStepsPos;
	_sthChanged = other._sthChanged;
	_isVisible = other._isVisible;
	_noTexture = other._noTexture;
	_texturesManager = other._texturesManager;
	auto size = other._render.getSize();
	_render.create(size.x, size.y);
	_mouseInput = other._mouseInput;
	_keyboardInput = other._keyboardInput;
	_focusOnHover = other._focusOnHover;
	setPosition(other.getPosition());
	setRotation(other.getRotation());
	setOrigin(other.getOrigin());
	setScale(other.getScale());
	return *this;
}

void ProgressBar::SetCurrentValue(float value)
{
	_currentValue = value;
	if (_currentValue != _prevValue)
	{
		_prevValue = _currentValue;
		_sthChanged = true;
	}
}

void ProgressBar::SetMaxValue(float maxValue)
{
	_maxValue = maxValue;
	if(_maxValue != _prevMaxValue)
	{
		_prevMaxValue = _maxValue;
		_sthChanged = true;
	}
}

float ProgressBar::GetCurrentValue()
{
	return _currentValue;
}

float ProgressBar::GetMaxValue()
{
	return _maxValue;
}

void ProgressBar::AddProgressBarStep(sf::FloatRect step, std::string textureName)
{
	_progressBarSteps.push_back(std::tuple<sf::FloatRect, std::string>(step, textureName));
	_sthChanged = true;
}

void ProgressBar::AddBackgroundLayer(sf::FloatRect layer, std::string textureName)
{
	_backgroundLayers.push_back(std::tuple<sf::FloatRect, std::string>(layer, textureName));
	_sthChanged = true;
}

void ProgressBar::ResetProgressBarSteps()
{
	_progressBarSteps.clear();
	_sthChanged = true;
}

void ProgressBar::ResetBackgroundLayers()
{
	_backgroundLayers.clear();
	_sthChanged = true;
}

void ProgressBar::ReplaceProgressBarStep(size_t index, std::tuple<sf::FloatRect, std::string> newTuple)
{
	if (_progressBarSteps.size() - 1 <= index)
	{
		_progressBarSteps[index] = newTuple;
		_sthChanged = true;
	}
}

void ProgressBar::ReplaceBackgroundLayer(size_t index, std::tuple<sf::FloatRect, std::string> newTuple)
{
	if (_backgroundLayers.size() - 1 <= index)
	{
		_backgroundLayers[index] = newTuple;
		_sthChanged = true;
	}
}

void ProgressBar::RemoveProgressBarStep(size_t index)
{
	if (_progressBarSteps.size() - 1 <= index)
	{
		_progressBarSteps.erase(_progressBarSteps.begin() + index);
		_sthChanged = true;
	}
}

void ProgressBar::RemoveBackgroundLayer(size_t index)
{
	if (_backgroundLayers.size() - 1 <= index)
	{
		_backgroundLayers.erase(_backgroundLayers.begin() + index);
		_sthChanged = true;
	}
}

void ProgressBar::SetProgressBarStepsPos(sf::Vector2f pos)
{
	_progressBarStepsPos = pos;
	_sthChanged = true;
}

size_t ProgressBar::GetNoOfProgressBarSteps()
{
	return _progressBarSteps.size();
}

size_t ProgressBar::GetNoOfBackgroundLayers()
{
	return _backgroundLayers.size();
}

std::tuple<sf::FloatRect, std::string> ProgressBar::GetProgressBarStep(size_t index)
{
	if (_progressBarSteps.size() - 1 <= index)
		return _progressBarSteps[index];
	return std::tuple<sf::FloatRect, std::string>();
}

std::tuple<sf::FloatRect, std::string> ProgressBar::GetBackgroundLayer(size_t index)
{
	if (_backgroundLayers.size() - 1 <= index)
		return _backgroundLayers[index];
	return std::tuple<sf::FloatRect, std::string>();
}

sf::Vector2f ProgressBar::GetProgressBarStepsPos()
{
	return _progressBarStepsPos;
}

sf::FloatRect ProgressBar::GetProgressBarStepsGlobalBounds()
{
	auto bounds = GetGlobalBounds();
	if (_progressBarSteps.size() == 0)
		return bounds;

	auto startPos = sf::Vector2f(bounds.left, bounds.top) + _progressBarStepsPos;
	auto width = _progressBarSteps.size() * std::get<0>(_progressBarSteps[_progressBarSteps.size() - 1]).width;
	auto endPos = startPos + sf::Vector2f(width, std::get<0>(_progressBarSteps[0]).height);
	return sf::FloatRect(startPos, endPos - startPos);
}

void ProgressBar::RedrawElement()
{
	sf::RenderStates rs;

	_render.clear(sf::Color::Transparent);

	//Draw background
	for (auto& layer : _backgroundLayers)
	{
		sf::Texture* texture = _texturesManager->GetTexture(std::get<1>(layer));
		if (texture == nullptr)
			texture = _noTexture;
		else if (texture->getSize().x == 0 || texture->getSize().y == 0)
			texture = _noTexture;

		rs.texture = texture;

		auto& rect = std::get<0>(layer);
		sf::VertexArray arr(sf::Quads, 4);
		arr[0].position = sf::Vector2f(0, 0);
		arr[1].position = sf::Vector2f(rect.width, 0);
		arr[2].position = sf::Vector2f(rect.width, rect.height);
		arr[3].position = sf::Vector2f(0, rect.height);
		arr[0].texCoords = sf::Vector2f(rect.left, rect.top);
		arr[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
		arr[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
		arr[3].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
		_render.draw(arr, rs);
	}

	//Draw full steps
	auto fullStep = _progressBarSteps[_progressBarSteps.size() - 1];
	auto& size = std::get<0>(fullStep);
	sf::Transformable t;
	sf::VertexArray step(sf::Quads, 4);
	step[0].position = sf::Vector2f(0, 0);
	step[1].position = sf::Vector2f(size.width, 0);
	step[2].position = sf::Vector2f(size.width, size.height);
	step[3].position = sf::Vector2f(0, size.height);
	step[0].texCoords = sf::Vector2f(size.left, size.top);
	step[1].texCoords = sf::Vector2f(size.left + size.width, size.top);
	step[2].texCoords = sf::Vector2f(size.left + size.width, size.top + size.height);
	step[3].texCoords = sf::Vector2f(size.left, size.top + size.height);
	sf::Texture* tex = _texturesManager->GetTexture(std::get<1>(fullStep));
	if (tex == nullptr)
		tex = _noTexture;
	else if (tex->getSize().x == 0 || tex->getSize().y == 0)
		tex = _noTexture;
	int fullSteps = -1;
	for (int i = 0; i < (int)_currentValue; i++)
	{
		t.setPosition(_progressBarStepsPos.x + (size.width * i), _progressBarStepsPos.y);
		rs.transform = t.getTransform();
		rs.texture = tex;
		_render.draw(step, rs);

		fullSteps = i;
	}

	//Draw partialy filled step
	float rest = (float)(_currentValue - (int)_currentValue);
	if (rest != 0.f && _maxValue > 0.f && _currentValue > 0.f)
	{
		size_t id = (size_t)roundf((float)(_progressBarSteps.size() - 1) * rest);
		auto& newSize = std::get<0>(_progressBarSteps[id]);
		step[0].position = sf::Vector2f(0, 0);
		step[1].position = sf::Vector2f(newSize.width, 0);
		step[2].position = sf::Vector2f(newSize.width, newSize.height);
		step[3].position = sf::Vector2f(0, newSize.height);
		step[0].texCoords = sf::Vector2f(newSize.left, newSize.top);
		step[1].texCoords = sf::Vector2f(newSize.left + newSize.width, newSize.top);
		step[2].texCoords = sf::Vector2f(newSize.left + newSize.width, newSize.top + newSize.height);
		step[3].texCoords = sf::Vector2f(newSize.left, newSize.top + newSize.height);
		t.setPosition(_progressBarStepsPos.x + (size.width * (fullSteps + 1)), _progressBarStepsPos.y);
		tex = _texturesManager->GetTexture(std::get<1>(_progressBarSteps[id]));
		if (tex == nullptr)
			tex = _noTexture;
		else if (tex->getSize().x == 0 || tex->getSize().y == 0)
			tex = _noTexture;
		rs.texture = tex;
		rs.transform = t.getTransform();
		_render.draw(step, rs);
		fullSteps++;
	}


	//Draw empty steps
	if (fullSteps > -1) 
		fullSteps++;

	auto& emptyStep = std::get<0>(_progressBarSteps[0]);
	step[0].position = sf::Vector2f(0, 0);
	step[1].position = sf::Vector2f(emptyStep.width, 0);
	step[2].position = sf::Vector2f(emptyStep.width, emptyStep.height);
	step[3].position = sf::Vector2f(0, emptyStep.height);
	step[0].texCoords = sf::Vector2f(emptyStep.left, emptyStep.top);
	step[1].texCoords = sf::Vector2f(emptyStep.left + emptyStep.width, emptyStep.top);
	step[2].texCoords = sf::Vector2f(emptyStep.left + emptyStep.width, emptyStep.top + emptyStep.height);
	step[3].texCoords = sf::Vector2f(emptyStep.left, emptyStep.top + emptyStep.height);
	tex = _texturesManager->GetTexture(std::get<1>(_progressBarSteps[0]));
	if (tex == nullptr)
		tex = _noTexture;
	else if (tex->getSize().x == 0 || tex->getSize().y == 0)
		tex = _noTexture;

	for (int j = fullSteps; j < (int)_maxValue; j++)
	{
		t.setPosition(_progressBarStepsPos.x + (size.width * j), _progressBarStepsPos.y);
		rs.transform = t.getTransform();
		rs.texture = tex;
		_render.draw(step, rs);
	}

	_render.display();
}

void ProgressBar::Update(bool, float)
{
	if (_sthChanged)
	{
		RedrawElement();
		_sthChanged = false;
	}
}

void ProgressBar::ProcessEvent(sf::Event* ev, sf::Vector2f mousePos)
{
	if (_mouseInput == true)
	{
		if (ev->type == sf::Event::MouseButtonPressed && ev->mouseButton.button == sf::Mouse::Left)
		{
			auto barBounds = GetProgressBarStepsGlobalBounds();
			if (barBounds.contains(mousePos))
			{
				float percent = (mousePos.x - barBounds.left) / barBounds.width;
				SetCurrentValue(_maxValue * percent);
			}
		}
	}
	if (_keyboardInput == true)
	{
		if (ev->type == sf::Event::KeyReleased)
		{
			auto divide = (_progressBarSteps.size() - 1 == 0) ? 1 : _progressBarSteps.size() - 1;
			float step = 1.f / float(divide);
			if (ev->key.code == sf::Keyboard::Left)
			{
				if (_currentValue - step < 0) SetCurrentValue(0.F);
				else SetCurrentValue(_currentValue - step);
			}
			else if (ev->key.code == sf::Keyboard::Right)
			{
				if (_currentValue + step > _maxValue) SetCurrentValue(_maxValue);
				else SetCurrentValue(_currentValue + step);
			}
		}
	}
}
