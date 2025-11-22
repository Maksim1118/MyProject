#include "ComboBox.h"

#include "ResourñeManager.h"

#include <cassert>
#include <algorithm>

constexpr int VISIBLE_ELEMENTS = 4;
constexpr unsigned int DEFAULT_CHAR_SIZE = 20;
constexpr unsigned int MAX_CHAR_SIZE = 50;
constexpr unsigned int MIN_CHAR_SIZE = 5;

constexpr float paddingLeftOffset = 10.f;
constexpr float paddingRightOffset = 5.f;
constexpr float scrollSpeed = 8.f;
constexpr float Y_Padding = 30.f;

using namespace sf;
using namespace std;

const Color layoutColor = Color(200, 200, 200);
const Color textColor = Color::Black;

namespace
{
	string elideText(const string& str, const Font& font, unsigned int chSize, float maxWidth)
	{
		Text text(str, font, chSize);
		if (text.getGlobalBounds().width <= maxWidth)
		{
			return str;
		}

		const string ellipsis = "...";
		Text ellipsisText(ellipsis, font, chSize);
		float ellipsisWidth = ellipsisText.getLocalBounds().width;

		string res = str;
		while (!res.empty())
		{
			res.pop_back();
			text.setString(res + ellipsis);
			if (text.getLocalBounds().width <= maxWidth)
			{
				return res + ellipsis;
			}
		}
		return "";
	}
}

ComboBox::ComboBox(const Font& font, const vector<std::string>& strItems, const unsigned int selectedInd, const Vector2f& size, const Vector2f& pos, function<void(string&)> onChanged)
	:
	 frameTex(rec.textures[ResourceManager::Textures::FRAME_DROPDOWN])
	,arrowUpTex(rec.textures[ResourceManager::Textures::MARKUP])
	,arrowDownTex(rec.textures[ResourceManager::Textures::MARKDOWN])
	,m_selectedInd(selectedInd)
	,m_font(font)
	,m_items(strItems)
	,m_size(size)
	,onChanged(std::move(onChanged))
	,m_scrollOff(0.f)
	,m_expand(false)
	,m_highlightArrow(false)
	,m_charSize(DEFAULT_CHAR_SIZE)
	,m_itemHeightInLayout(0.f)

{
	assert(frameTex && "frameTex must not be nullptr");
	assert(arrowUpTex && "arrowUpTex texture must not be nullptr");
	assert(arrowDownTex && "arrowDownTex texture must not be nullptr");

	if (m_selectedInd >= strItems.size()) m_selectedInd = 0;

	m_hoveredInd = m_selectedInd;

	m_frame.setTexture(*frameTex);
	m_arrow.setTexture(*arrowDownTex);

	m_selectedText.setFont(font);
	m_selectedText.setString(strItems[m_selectedInd]);
	m_selectedText.setCharacterSize(m_charSize);
	m_selectedText.setFillColor(textColor);

	setSize(size.x, size.y);
	setPosition(pos);
}

ComboBox::ComboBox(ComboBox&& other) noexcept
	:UI_Element(std::move(other))
	,frameTex(exchange(other.frameTex, nullptr))
	,arrowUpTex(exchange(other.arrowUpTex, nullptr))
	,arrowDownTex(exchange(other.arrowDownTex, nullptr))
	,m_items(std::move(other.m_items))
	,m_font(std::move(other.m_font))
	,m_size(other.m_size)
	,onChanged(std::move(other.onChanged))
	,m_selectedText(std::move(other.m_selectedText))
	,m_expand(other.m_expand)
	,m_hoveredInd(other.m_hoveredInd)
	,m_highlightArrow(other.m_highlightArrow)
	,m_selectedInd(other.m_selectedInd)
	,m_charSize(other.m_charSize)
	,m_scrollOff(other.m_scrollOff)
	,m_maxScrollOffset(other.m_maxScrollOffset)
	,m_itemHeightInLayout(other.m_itemHeightInLayout)
	,m_layout(std::move(other.m_layout))
	,m_renderTexture(std::move(other.m_renderTexture))
	,m_frame(std::move(other.m_frame))
	,m_arrow(std::move(other.m_arrow))
{
}

ComboBox& ComboBox::operator=(ComboBox&& other) noexcept
{
	if (this != &other)
	{

		UI_Element::operator=(std::move(other));

		frameTex = exchange(other.frameTex, nullptr);
		arrowUpTex = exchange(other.arrowUpTex, nullptr);
		arrowDownTex = exchange(other.arrowDownTex, nullptr);
		m_items = std::move(other.m_items);
		m_font = std::move(other.m_font);
		m_size = other.m_size;
		onChanged = std::move(other.onChanged);
		m_selectedText = std::move(other.m_selectedText);
		m_expand = other.m_expand;
		m_hoveredInd = other.m_hoveredInd;
		m_highlightArrow = other.m_highlightArrow;
		m_selectedInd = other.m_selectedInd;
		m_charSize = other.m_charSize;
		m_scrollOff = other.m_scrollOff;
		m_maxScrollOffset = other.m_maxScrollOffset;
		m_itemHeightInLayout = other.m_itemHeightInLayout;
		m_layout = std::move(other.m_layout);
		m_renderTexture = std::move(other.m_renderTexture);
		m_frame = std::move(other.m_frame);
		m_arrow = std::move(other.m_arrow);

	}
	return *this;
}

ComboBox::~ComboBox()
{
}

void ComboBox::setSize(const float width, const float height)
{
	if (width <= 0.f || height <= 0.f)
	{
		m_size = Vector2f(150.f, 30.f);
	}
	else
	{
		m_size = { width, height };
	}

	const float frameScaleY = m_size.y / m_frame.getTexture()->getSize().y;
	const float frameScaleX = m_size.x / m_frame.getTexture()->getSize().x;
	m_frame.setScale(frameScaleX, frameScaleY);

	float textSize = m_size.y * 0.7f;
	m_charSize = static_cast<unsigned int>(textSize);

	const float arrowScaleY = textSize / m_arrow.getTexture()->getSize().y;
	const float aspectRatioArrow = m_arrow.getTexture()->getSize().x / m_arrow.getTexture()->getSize().y;
	m_arrow.setScale(arrowScaleY * aspectRatioArrow, arrowScaleY);

	m_selectedText.setCharacterSize(m_charSize);

	FloatRect textRectLocal = m_selectedText.getLocalBounds();
	FloatRect arrowRectGlobal = m_arrow.getGlobalBounds();
	FloatRect frameRectGlobal = m_frame.getGlobalBounds();


	float yPos = frameRectGlobal.top + (frameRectGlobal.height - textRectLocal.height)/2.f - textRectLocal.top;
	m_selectedText.setPosition(frameRectGlobal.left + paddingLeftOffset, yPos);

	const float arrowX = frameRectGlobal.left + frameRectGlobal.width - paddingRightOffset - arrowRectGlobal.width;
	const float arrowY = frameRectGlobal.top + (frameRectGlobal.height - arrowRectGlobal.height) / 2.f;
	m_arrow.setPosition(arrowX, arrowY);
}

void ComboBox::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)
{
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		FloatRect layoutBounds = getTransform().transformRect(m_layout.backGround.getGlobalBounds());
		
		FloatRect arrowBounds = getTransform().transformRect(m_arrow.getGlobalBounds());
		if (arrowBounds.contains(mousePos) || (m_expand && !layoutBounds.contains(mousePos)))
		{
			toggleDropDown();
			if (m_expand)
			{
				createLayout();
				m_hoveredInd = m_selectedInd;
			}
			else
			{
				deleteLayout();
			}
		}
	}

	if (m_expand)
	{
		m_hoveredInd = calcHoveredInd(mousePos);
		if (m_hoveredInd != -1)
		{
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
			{
				m_selectedInd = m_hoveredInd;
				m_selectedText.setString(m_items[m_selectedInd]);
				updateSelectedText();
				m_expand = false;

				if (onChanged)
				{
					onChanged(m_items[m_selectedInd]);
				}
				return;
			}
		}
	

		if (event.type == Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel)
			{
				scroll(event.mouseWheelScroll.delta);
			}
		}
	}

}

void ComboBox::update(const int diff, const sf::Vector2f& mousePos)
{
	FloatRect arrowBounds = getTransform().transformRect(m_arrow.getGlobalBounds());
	m_highlightArrow = arrowBounds.contains(mousePos);

	m_arrow.setTexture(m_expand ? *arrowUpTex : *arrowDownTex);
}

bool ComboBox::isBlocking()
{
	return m_expand;
}


void ComboBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_frame, states);

	if (m_highlightArrow)
	{
		FloatRect arrowBounds = getTransform().transformRect(m_arrow.getGlobalBounds());
		RectangleShape highlight;
		highlight.setFillColor(Color(100, 100, 100));
		highlight.setSize({ arrowBounds.width, arrowBounds.height });
		highlight.setPosition(arrowBounds.left, arrowBounds.top);

		target.draw(highlight);
	}

	target.draw(m_arrow, states);
	target.draw(m_selectedText, states);

	if (m_expand)
	{
		m_renderTexture->clear(Color::Transparent);
		RectangleShape bg = m_layout.backGround;
		bg.setPosition(0, 0);
		m_renderTexture->draw(bg);

		sf::View oldView = m_renderTexture->getView();
		sf::View view(sf::FloatRect(0.f, m_scrollOff, m_layout.backGround.getSize().x, m_layout.backGround.getSize().y));
		m_renderTexture->setView(view);

		size_t startIndex = static_cast<size_t>(m_scrollOff / m_itemHeightInLayout);
		size_t endIndex = startIndex + VISIBLE_ELEMENTS + 1;
		if (endIndex > m_items.size()) endIndex = m_items.size();

		for (size_t i = startIndex; i < endIndex; ++i)
		{
			if (i == m_hoveredInd)
			{
				RectangleShape highLight;
				highLight.setSize({ m_layout.backGround.getSize().x, m_itemHeightInLayout});
				const float x_highLight = 0.f;
				const float y_hightLight = static_cast<float>(i) * m_itemHeightInLayout;
				highLight.setPosition(x_highLight, y_hightLight);
				highLight.setFillColor(Color(90, 90,90));

				m_renderTexture->draw(highLight);
			}

			m_renderTexture->draw(m_layout.textItems[i]);
		}

		m_renderTexture->setView(oldView);
		m_renderTexture->display();

		sf::Sprite sprite(m_renderTexture->getTexture());
		sprite.setPosition(m_layout.backGround.getPosition());
		target.draw(sprite, states);
	}
}

void ComboBox::scroll(const float delta)
{
	m_scrollOff = std::clamp(m_scrollOff - delta * scrollSpeed, 0.f, m_maxScrollOffset);
}

void ComboBox::toggleDropDown()
{
	m_expand = !m_expand;
}

void ComboBox::createLayout()
{
	auto it = max_element(m_items.begin(), m_items.end(),
		[](const string& a, const string& b) { return a.size() < b.size(); });
	Text maxText;

	maxText.setFont(m_font);
	maxText.setString(*it);
	maxText.setCharacterSize(m_charSize);

	float sizeX = max(maxText.getLocalBounds().width + 20.f, m_size.x);

	m_itemHeightInLayout = static_cast<float>(m_charSize) * 1.4f;

	m_layout.backGround.setFillColor(layoutColor);
	m_layout.backGround.setSize({ sizeX, m_itemHeightInLayout * static_cast<float>(VISIBLE_ELEMENTS)});

	FloatRect frameGlobalBounds = m_frame.getGlobalBounds();
	m_layout.backGround.setPosition(frameGlobalBounds.left, frameGlobalBounds.top + frameGlobalBounds.height);
	FloatRect backGroundBounds = m_layout.backGround.getGlobalBounds();

	const float totalHeight = static_cast<float>(m_items.size()) * m_itemHeightInLayout;
	m_maxScrollOffset = max(0.f, totalHeight - backGroundBounds.height);

	m_renderTexture = make_unique<RenderTexture>();
	if (!m_renderTexture->create(static_cast<unsigned int>(backGroundBounds.width), static_cast<unsigned int>(backGroundBounds.height))) {
		throw std::runtime_error("Render texture is not created");
	}

	m_layout.textItems.resize(m_items.size());

	for (size_t i = 0; i < m_items.size(); ++i)
	{
		m_layout.textItems[i].setFont(m_font);
		m_layout.textItems[i].setString(m_items[i]);
		m_layout.textItems[i].setCharacterSize(m_charSize);
		m_layout.textItems[i].setFillColor(textColor);

		FloatRect textLocalBounds = m_layout.textItems[i].getLocalBounds();
		float x = (backGroundBounds.width - textLocalBounds.width) / 2.f;
		float y =  i * m_itemHeightInLayout + (m_itemHeightInLayout - textLocalBounds.height) / 2.f - textLocalBounds.top;
		m_layout.textItems[i].setPosition(x, y);
	}
}

void ComboBox::deleteLayout()
{
	m_renderTexture.reset();
	m_layout.textItems.clear();
}


void ComboBox::updateSelectedText()
{
	float maxTextWidth = m_size.x - paddingLeftOffset - paddingRightOffset - m_arrow.getGlobalBounds().width;
	string fullStr = m_selectedText.getString();
	string displayedText = elideText(fullStr, *m_selectedText.getFont(), m_selectedText.getCharacterSize(), maxTextWidth);

	m_selectedText.setString(displayedText);
}

int ComboBox::calcHoveredInd(const Vector2f& mousePos)
{
	Vector2f localPos = getInverseTransform().transformPoint(mousePos);

	Vector2f relativePos = localPos - m_layout.backGround.getPosition();

	if (relativePos.x < 0 || relativePos.x > m_layout.backGround.getSize().x
		|| relativePos.y < 0 || relativePos.y > m_layout.backGround.getSize().y)
	{
		return -1;
	}

	float yScrolled = relativePos.y + m_scrollOff;
	int index = static_cast<int>(yScrolled / m_itemHeightInLayout);

	if (index < 0 || index >= static_cast<int>(m_items.size()))
	{
		return -1;                       
	}

	return index;
}

