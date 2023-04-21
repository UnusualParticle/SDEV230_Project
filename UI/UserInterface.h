#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

namespace ui
{
	using Vector = sf::Vector2f;
	using State = int;
	using Format = int;
	
	enum States
	{
		none = 0,
		ignore = 1,
		mouseover = 1 << 1,
		mouseclick = 1 << 2,
		clickframe = 1 << 3,
		releasefame = 1 << 4,
		selected = 1 << 5,
		canHover = 1 << 6,
		canClick = 1 << 7,
		canDrag = 1 << 8,
		canSelect = 1 << 9,
		reserved = std::numeric_limits<State>::max(),
	};
	enum Formats
	{
		fitText = 1,
		centerTextHorizontal = 2,
		centerTextVertical = 4,
		centerText = centerTextHorizontal | centerTextVertical,
		centerOrigin = 8,
	};

	struct StateStyle
	{
		sf::Color fillColor{ sf::Color::Black };	// default: Black
		sf::Color outlineColor{ sf::Color::White };	// default: White
		float outlineThickness{ -1.f };				// default: -1
		sf::Color textColor{ sf::Color::White };	// default: White
		unsigned int characterSize{ 30 };			// default: 30
		Vector padding{ 5.f,5.f };					// default: 5, 5
	};
	struct ElementStyle
	{
		StateStyle regular{};
		StateStyle hover{};
		StateStyle click{};
		StateStyle select{};
		Format format{};

		ElementStyle();
		ElementStyle(const ElementStyle&);
	};

	namespace Settings
	{
		inline sf::Font font{};
		inline ElementStyle style{};
		inline State stateDefault{ canHover | canClick | canSelect };
		inline Vector prevMousePos{};
		inline Vector curMousePos{};
		inline Vector windowSize{};

		void updateMouse(const sf::Window&);
	}

	class Element_Base : public sf::Drawable
	{
	public:
		virtual void pollEvent(const sf::Event&) = 0;
	};

	class Element : public Element_Base
	{
	public:
	private:
		sf::RectangleShape m_body{};
		sf::FloatRect m_bound{};
		sf::Text m_text{ "", Settings::font };

		float m_minWidth{};
		size_t m_textSize{};
		const ElementStyle* m_style{ &Settings::style };
		State m_state{};
		State m_lastState{ States::reserved };
	public:
		Element() = default;
		Element(const Vector& pos, const Vector& size, const sf::String & = "", const ElementStyle* = &Settings::style, const State state = Settings::stateDefault);
		~Element() = default;

		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void pollEvent(const sf::Event&);

		// States
		void ignore(bool = true);
		bool isIgnored() const;
		bool isHovered() const;
		bool isClicked() const;
		bool onClick() const;
		bool onRelease() const;
		bool isSelected() const;
		bool canHover() const;
		bool canClick() const;
		bool canDrag() const;
		bool canSelect() const;
		void resetState();
		void setStates(State);
		void setCanHover(bool);
		void setCanClick(bool);
		void setCanDrag(bool);
		void setCanSelect(bool);
		State getPrimaryState() const;

		// Style
		void assignStyle(const ElementStyle*);
		void setStyle(const StateStyle&);
		const StateStyle& getStyle() const;

		// Format
		bool isTextFit() const;
		bool isTextCenterHorizontal() const;
		bool isTextCenterVertical() const;
		bool isTextCenter() const;
		bool isOriginCenter() const;

		// Transform
		void setSize(const Vector&);
		void resetSize();
		void setPosition(const Vector&);
		void resetPosition();
		void move(const Vector&);
		void setTextRatio(float);
		void setScale(const Vector&);
		void setOrigin(const Vector&, bool override = false);
		void setOriginCenter();
		// Transform Getters
		sf::FloatRect getBodyBounds() const;
		sf::FloatRect getTextBounds() const;
		sf::FloatRect getBounds() const;
		Vector getPosition() const;
		Vector getSize() const;
		Vector getScale() const;

		void setString(const sf::String&);
		sf::String getString() const;
		void editString(sf::Event::KeyEvent);

		void setTexture(sf::Texture*);
	};

	namespace TEST_UI
	{
		void RunTest();
	}
}

// Check if the enter key is currently pressed
bool isEnterPressed();
// Check if the Escape key is currently pressed
bool onEscapePress();
// Check if the last event was a mouse event and updates ui::Settings mouse positions
bool isMouseEvent(const sf::Window& window, const sf::Event& event);
