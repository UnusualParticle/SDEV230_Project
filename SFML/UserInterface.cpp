#include "UserInterface.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

namespace ui
{
	namespace Settings
	{
		void updateMouse(const sf::Window& window)
		{
			prevMousePos = curMousePos;
			curMousePos = Vector{ sf::Mouse::getPosition(window) };
		}
	}

	ElementStyle::ElementStyle() {};
	ElementStyle::ElementStyle(const ElementStyle& style)
	{
		*this = style;
	}

	Element::Element(const Vector& pos, const Vector& size, const sf::String& string, const ElementStyle& style, const State state)
		: m_text(string, Settings::font, style.regular.characterSize), m_style(&style), m_minWidth(size.x)
	{
		setSize(size);
		setPosition(pos);
		if (isOriginCenter())
			setOrigin({ size.x / 2,size.y / 2 }, true);
		setStates(state);
		setStyle(m_style->regular);
	}
	void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_body, states);
		target.draw(m_text, states);
	}
	void Element::pollEvent(const sf::Event& event)
	{
		if (onClick())
			m_state ^= States::clickframe;
		if (onRelease())
			m_state ^= States::releasefame;
		if (!isIgnored()) {
			switch (event.type)
			{
			case sf::Event::MouseMoved:
				if (m_bound.contains(Settings::curMousePos) && canHover())
					m_state |= States::mouseover;
				else if (isHovered())
					m_state ^= States::mouseover;
				break;
			case sf::Event::MouseButtonPressed:
				if (isHovered() && canClick())
				{
					m_state |= States::mouseclick;
					m_state |= States::clickframe;
				}
				else if (isSelected())
					m_state ^= States::selected;
				break;
			case sf::Event::MouseButtonReleased:
				if (isClicked() && canSelect())
				{
					m_state ^= States::mouseclick;
					m_state |= States::releasefame;
				}
				if (isHovered())
					m_state |= States::selected;
				break;
			}
		}
		if (m_lastState != getPrimaryState())
		{
			setStyle(getStyle());
			m_lastState = getPrimaryState();
		}
		if (isClicked() && canDrag())
			move(Settings::curMousePos - Settings::prevMousePos);
	}

	// State
	void Element::ignore(bool s)
	{
		if (s)
		{
			m_state |= States::ignore;
			resetState();
		}
		else if (isIgnored())
			m_state ^= States::ignore;
	}
	bool Element::isIgnored() const { return m_state & States::ignore; }
	bool Element::isHovered() const {
		if (canHover())
			return m_state & States::mouseover;
		else
			return false;
	}
	bool Element::isClicked() const {
		if (canClick())
			return m_state & States::mouseclick;
		else
			return false;
	}
	bool Element::onClick() const
	{
		if (canClick())
			return m_state & States::clickframe;
		else
			return false;
	}
	bool Element::onRelease() const
	{
		if (canClick())
			return m_state & States::releasefame;
		else
			return false;
	}
	bool Element::isSelected() const {
		if (canSelect())
			return m_state & States::selected;
		else
			return false;
	}
	bool Element::canHover() const
	{
		if (isIgnored())
			return false;
		else
			return m_state & States::canHover;
	}
	bool Element::canClick() const
	{
		if (isIgnored())
			return false;
		else
			return m_state & States::canClick;
	}
	bool Element::canDrag() const
	{
		if (isIgnored())
			return false;
		else
			return m_state & States::canDrag;
	}
	bool Element::canSelect() const
	{
		if (isIgnored())
			return false;
		else
			return m_state & States::canSelect;
	}
	void Element::resetState()
	{
		// all on
		State mask{ States::reserved };

		// turn specifics off
		mask ^= States::mouseover;
		mask ^= States::mouseclick;
		mask ^= States::selected;

		// apply mask
		m_state &= mask;


		setStyle(getStyle());
	}
	void Element::setStates(State s)
	{
		setCanHover(s & States::canHover);
		setCanClick(s & States::canClick);
		setCanDrag(s & States::canDrag);
		setCanSelect(s & States::canSelect);
	}
	void Element::setCanHover(bool b)
	{
		if (b)
			m_state |= States::canHover;
		else if (canHover())
			m_state ^= States::canHover;
	}
	void Element::setCanClick(bool b)
	{
		if (b)
			m_state |= States::canClick;
		else if (canClick())
			m_state ^= States::canClick;
	}
	void Element::setCanDrag(bool b)
	{
		if (b)
			m_state |= States::canDrag;
		else if (canDrag())
			m_state ^= States::canDrag;
	}
	void Element::setCanSelect(bool b)
	{
		if (b)
			m_state |= States::canSelect;
		else if (canSelect())
			m_state ^= States::canSelect;
	}
	State Element::getPrimaryState() const
	{
		if (isClicked())
			return States::mouseclick;
		else if (isHovered())
			return States::mouseover;
		else if (isSelected())
			return States::selected;
		else
			return States::none;
	}

	// Style
	void Element::assignStyle(const ElementStyle& style)
	{
		m_style = &style;
		setStyle(getStyle());
	}
	void Element::setStyle(const StateStyle& style)
	{
		m_body.setFillColor(style.fillColor);
		m_body.setOutlineColor(style.outlineColor);
		m_body.setOutlineThickness(style.outlineThickness);
		m_text.setFillColor(style.textColor);
		m_text.setCharacterSize(style.characterSize);
		resetSize();
		resetPosition();
	}
	const StateStyle& Element::getStyle() const
	{
		switch (getPrimaryState())
		{
		case States::mouseover:
			return m_style->hover;
		case States::mouseclick:
			return m_style->click;
		case States::selected:
			return m_style->select;
		default:
			return m_style->regular;
		}
	}

	// Format
	bool Element::isTextFit() const { return m_style->format & Formats::fitText; }
	bool Element::isTextCenterHorizontal() const { return m_style->format & Formats::centerTextHorizontal; }
	bool Element::isTextCenterVertical() const { return m_style->format & Formats::centerTextVertical; }
	bool Element::isTextCenter() const { return isTextCenterHorizontal() && isTextCenterVertical(); }
	bool Element::isOriginCenter() const { return m_style->format & Formats::centerOrigin; }

	// Transform
	void Element::setSize(const Vector& size)
	{
		float myWidth = m_text.getGlobalBounds().width + getStyle().padding.x * 2;
		if (isTextFit() && myWidth > size.x)
			m_body.setSize({ myWidth, size.y });
		else
			m_body.setSize(size);
		m_bound = m_body.getGlobalBounds();
	}
	void Element::resetSize()
	{
		setSize({ m_minWidth, m_body.getSize().y });
	}
	void Element::setPosition(const Vector& pos)
	{
		m_body.setPosition(pos);
		m_bound = m_body.getGlobalBounds();

		float tX{};
		if (!isTextCenterHorizontal())
			tX = pos.x + getStyle().padding.x;
		else
			tX = pos.x + (m_bound.width - getTextBounds().width) / 2;

		float tY{};
		if (!isTextCenterVertical())
			tY = pos.y + getStyle().padding.y;
		else
			tY = pos.y + (m_bound.height - getTextBounds().height) / 2;
		m_text.setPosition({ tX,tY });
	}
	void Element::resetPosition()
	{
		setPosition(m_body.getPosition());
	}
	void Element::move(const Vector& mov)
	{
		m_body.move(mov);
		m_bound = m_body.getGlobalBounds();
		m_text.move(mov);
	}
	void Element::setTextRatio(float ratio)
	{
		m_text.setCharacterSize(static_cast<size_t>(getSize().y * ratio));
	}
	void Element::setScale(const Vector& scalar)
	{
		m_body.setScale(scalar);
		m_text.setScale(scalar);
		setPosition(m_body.getPosition());
	}
	void Element::setOrigin(const Vector& origin, bool centerOverride)
	{
		m_body.setOrigin(origin);
		m_text.setOrigin(origin);
	}
	void Element::setOriginCenter()
	{
		setOrigin({ m_body.getSize().x / 2,m_body.getSize().y / 2 }, true);
	}
	// Transform Getters
	Vector Element::getPosition() const { return m_body.getPosition(); }
	Vector Element::getSize() const { return m_body.getSize(); }
	Vector Element::getScale() const { return m_body.getScale(); }
	sf::FloatRect Element::getBodyBounds() const
	{
		return m_body.getGlobalBounds();
	}
	sf::FloatRect Element::getTextBounds() const
	{
		// Return the bounding box for the text that does not include line spacing
		// The extra line spacing is above the text
		return {
			m_text.getGlobalBounds().left,
			m_text.getGlobalBounds().top,
			m_text.getGlobalBounds().width,
			m_text.getGlobalBounds().height + (m_text.getFont()->getLineSpacing(m_text.getCharacterSize())) / 2
		};
	}
	sf::FloatRect Element::getBounds() const
	{
		// Return the largest bounding box that covers the shape and the text
		// This is not used for mouse interaction, use m_bound instead. The mouse should only interact when over the body.
		auto shape{ getBodyBounds() };
		auto text{ getTextBounds() };
		return {
			(shape.left < text.left) ? shape.left : text.left,
			(shape.top < text.top) ? shape.top : text.top,
			(shape.width > text.width) ? shape.width : text.width,
			(shape.height > text.height) ? shape.height : text.height,
		};
	}

	// String Operation
	void Element::setString(const sf::String& str)
	{
		m_text.setString(str);
		if (isTextFit())
			resetSize();
		if (isOriginCenter())
			setOriginCenter();
		if (isTextCenterHorizontal() || isTextCenterVertical())
			resetPosition();
	}
	sf::String Element::getString() const
	{
		return m_text.getString();
	}
	void Element::editString(sf::Event::KeyEvent k)
	{
		if (!isSelected())
			return;
		sf::String str{ getString() };
		switch (k.code)
		{
		case sf::Keyboard::Unknown:
			break;
		case sf::Keyboard::A:
		case sf::Keyboard::B:
		case sf::Keyboard::C:
		case sf::Keyboard::D:
		case sf::Keyboard::E:
		case sf::Keyboard::F:
		case sf::Keyboard::G:
		case sf::Keyboard::H:
		case sf::Keyboard::I:
		case sf::Keyboard::J:
		case sf::Keyboard::K:
		case sf::Keyboard::L:
		case sf::Keyboard::M:
		case sf::Keyboard::N:
		case sf::Keyboard::O:
		case sf::Keyboard::P:
		case sf::Keyboard::Q:
		case sf::Keyboard::R:
		case sf::Keyboard::S:
		case sf::Keyboard::T:
		case sf::Keyboard::U:
		case sf::Keyboard::V:
		case sf::Keyboard::W:
		case sf::Keyboard::X:
		case sf::Keyboard::Y:
		case sf::Keyboard::Z:
			str += static_cast<char>((k.shift) ? 'A' + k.code : 'a' + k.code);
			break;
		case sf::Keyboard::Num0:
		case sf::Keyboard::Num1:
		case sf::Keyboard::Num2:
		case sf::Keyboard::Num3:
		case sf::Keyboard::Num4:
		case sf::Keyboard::Num5:
		case sf::Keyboard::Num6:
		case sf::Keyboard::Num7:
		case sf::Keyboard::Num8:
		case sf::Keyboard::Num9:
			str += static_cast<char>('0' - sf::Keyboard::Num9 + k.code);
			break;
		case sf::Keyboard::LBracket:
			str += '[';
			break;
		case sf::Keyboard::RBracket:
			str += ']';
			break;
		case sf::Keyboard::Semicolon:
			str += ':';
			break;
		case sf::Keyboard::Comma:
			str += ',';
			break;
		case sf::Keyboard::Period:
			str += '.';
			break;
		case sf::Keyboard::Quote:
			str += '\'';
			break;
		case sf::Keyboard::Slash:
		case sf::Keyboard::Divide:
			str += '/';
			break;
		case sf::Keyboard::Backslash:
			str += '\\';
			break;
		case sf::Keyboard::Tilde:
			str += '~';
			break;
		case sf::Keyboard::Equal:
			str += '=';
			break;
		case sf::Keyboard::Hyphen:
		case sf::Keyboard::Subtract:
			str += '-';
			break;
		case sf::Keyboard::Space:
			str += ' ';
			break;
		case sf::Keyboard::Backspace:
			if (str.getSize())
				str.erase(str.getSize() - 1);
			break;
		case sf::Keyboard::Tab:
			str += '\t';
			break;
		case sf::Keyboard::Add:
			str += '+';
			break;
		case sf::Keyboard::Multiply:
			str += '*';
			break;
		case sf::Keyboard::Numpad0:
		case sf::Keyboard::Numpad1:
		case sf::Keyboard::Numpad2:
		case sf::Keyboard::Numpad3:
		case sf::Keyboard::Numpad4:
		case sf::Keyboard::Numpad5:
		case sf::Keyboard::Numpad6:
		case sf::Keyboard::Numpad7:
		case sf::Keyboard::Numpad8:
		case sf::Keyboard::Numpad9:
			str += static_cast<char>('0' - sf::Keyboard::Numpad9 + k.code);
			break;
		default:
			break;
		}
		setString(str);
	}
	void Element::setTexture(sf::Texture* t)
	{
		m_body.setTexture(t);
	}
}

sf::Vector2f std::floor(sf::Vector2f v)
{
	return { std::floor(v.x), std::floor(v.y) };
}

bool isEnterPressed()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
}
bool onEscapePress()
{
	static bool previous{};
	if (previous == sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		return false;
	else
	{
		if (previous)
			previous = false;
		else
			previous = true;
		return previous;
	}
}
bool isMouseEvent(const sf::Window& window, const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		ui::Settings::updateMouse(window);
		return true;
	case sf::Event::MouseButtonPressed:
	case sf::Event::MouseButtonReleased:
		return true;
	}
	return false;
}
e_Menu isMenuKey()
{
	using K = sf::Keyboard;
	if (K::isKeyPressed(K::Escape))
		return e_Menu::Settings;
	if (K::isKeyPressed(K::Tab))
		return e_Menu::Director;
	if (K::isKeyPressed(K::I))
		return e_Menu::Inventory;
	return e_Menu::total;
}
