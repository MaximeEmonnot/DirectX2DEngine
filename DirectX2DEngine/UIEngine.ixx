module;
#include <DirectXMath.h>
#include <d2d1helper.h>

export module UIEngine;

import <memory>;
import <vector>;
import <functional>;
import <codecvt>;
import <locale>;

import CoreEngine;
import MathEngine;
import GraphicsEngine;

export
class AUIElement
{
public:
	AUIElement() = default;
	AUIElement(const FRect& position)
		:
		position(position)
	{}
	virtual ~AUIElement() = default;

	virtual void	SetPosition(const FRect& newPosition)
	{
		position = newPosition;
	}

	virtual void	Update()
	{}
	virtual void	Render() const = 0;

protected:
	FRect					position;
	static constexpr float	depth = 256.f;
};


export
class AUICanvas
{
public:
	AUICanvas() = default;
	virtual ~AUICanvas() = default;

	virtual void	Update() = 0;
	void			Render() const
	{
		for (const std::shared_ptr<AUIElement>& pUI : UIElements) pUI->Render();
	}

protected:
	template<class T>
	std::shared_ptr<T> CreateUIElement()
	{
		std::shared_ptr<AUIElement> new_ui = std::make_shared<T>();
		UIElements.emplace_back(new_ui);
		return std::dynamic_pointer_cast<T>(new_ui);
	}

	template<class T, typename... Args>
	std::shared_ptr<T> CreateUIElement(Args&&... args)
	{
		std::shared_ptr<AUIElement> new_ui = std::make_shared<T>(args...);
		UIElements.emplace_back(new_ui);
		return std::dynamic_pointer_cast<T>(new_ui);
	}

private:
	std::vector<std::shared_ptr<AUIElement>>	UIElements;
};


export
class UIButton : public AUIElement
{
public:
	UIButton() = default;
	UIButton(const FRect& position, const DirectX::XMFLOAT4& baseColor, const DirectX::XMFLOAT4& hoverColor, const DirectX::XMFLOAT4& clickColor)
		:
		AUIElement(position),
		pModel(std::make_shared<ColorModel>(position, baseColor)),
		baseColor(baseColor),
		hoverColor(hoverColor),
		clickColor(clickColor),
		task([&] {})
	{
		pModel->Initialize();
		pModel->SetDepth(depth);
	}

	void	SetTask(std::function<void()> newTask)
	{
		task = newTask;
	}

	void	Update() override
	{
		// We adjust the button's color on his Normal state, Hovering state and Click state
		if (position.ContainsVec2D(MOUSE_POSITION)) {
			if (MOUSE.Read() == Mouse::EventType::LPress) {
				pModel->SetColor(clickColor);
				task();
			}
			else pModel->SetColor(hoverColor);
		}
		else pModel->SetColor(baseColor);
	}
	void	Render() const override
	{
		pModel->Render();
	}

private:
	std::shared_ptr<ColorModel>		pModel;
	DirectX::XMFLOAT4				baseColor;
	DirectX::XMFLOAT4				hoverColor;
	DirectX::XMFLOAT4				clickColor;
	std::function<void()>			task;
};


export
class UIImage : public AUIElement
{
public:
	UIImage() = default;
	UIImage(const FRect& position, const Texture& texture)
		:
		AUIElement(position),
		pModel(std::make_shared<TextureModel>(texture))
	{
		pModel->Initialize();
		pModel->SetRectangle(position);
		pModel->SetDepth(depth);
	}
	UIImage(const FRect& position, const std::string& texturePath)
		:
		AUIElement(position),
		pModel(std::make_shared<TextureModel>(texturePath))
	{
		pModel->Initialize();
		pModel->SetRectangle(position);
		pModel->SetDepth(depth);
	}

	void	SetPosition(const FRect& newPosition) override
	{
		AUIElement::SetPosition(newPosition);
		pModel->SetRectangle(newPosition);
	}
	void	SetTexture(const Texture& newTexture) const
	{
		pModel->SetTexture(newTexture);
	}
	void	SetInverted(bool bValue) const
	{
		pModel->SetInverted(bValue);
	}

	void	Render() const override
	{
		pModel->Render();
	}

private:
	std::shared_ptr<TextureModel>	pModel;
};


export
class UIProgressBar :
	public AUIElement
{
public:
	enum class ProgressDirection
	{
		LeftToRight,
		RightToLeft,
		TopToBottom,
		BottomToTop,
		BothHorizontal,
		BothVertical,
		AllSides
	};
public:
	UIProgressBar() = default;
	UIProgressBar(const FRect& position, const DirectX::XMFLOAT4& color, ProgressDirection direction = ProgressDirection::LeftToRight)
		:
		AUIElement(position),
		pBackgroundModel(std::make_shared<ColorModel>()),
		pFillModel(std::make_shared<ColorModel>()),
		direction(direction)
	{
		pBackgroundModel->Initialize();
		pBackgroundModel->SetColor(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));
		pBackgroundModel->SetRectangle(position);
		pBackgroundModel->SetDepth(depth);

		pFillModel->Initialize();
		pFillModel->SetColor(color);
		pFillModel->SetRectangle(position);
		pFillModel->SetDepth(depth);
	}

	void    SetPercentage(float newPercentage)
	{
		percentage = newPercentage;
		FRect new_rectangle = position;
		const float left = position.pos.x - position.width / 2;
		const float right = position.pos.x + position.width / 2;
		const float bottom = position.pos.y - position.height / 2;
		const float top = position.pos.y + position.height / 2;
		switch (direction)
		{
		case ProgressDirection::LeftToRight:
			new_rectangle.width *= percentage;
			new_rectangle.pos.x = right - (right - position.pos.x) * percentage;
			break;
		case ProgressDirection::RightToLeft:
			new_rectangle.width *= percentage;
			new_rectangle.pos.x = left + (position.pos.x - left) * percentage;
			break;
		case ProgressDirection::TopToBottom:
			new_rectangle.height *= percentage;
			new_rectangle.pos.y = bottom + (position.pos.y - bottom) * percentage;
			break;
		case ProgressDirection::BottomToTop:
			new_rectangle.height *= percentage;
			new_rectangle.pos.y = top - (top - position.pos.y) * percentage;
			break;
		case ProgressDirection::BothHorizontal:
			new_rectangle.width *= percentage;
			break;
		case ProgressDirection::BothVertical:
			new_rectangle.height *= percentage;
			break;
		case ProgressDirection::AllSides:
			new_rectangle *= percentage;
			break;
		default:;
		}
		pFillModel->SetRectangle(new_rectangle);
	}

	void    Render() const override
	{
		pFillModel->Render();
		pBackgroundModel->Render();
	}

private:
	std::shared_ptr<ColorModel>     pBackgroundModel;
	std::shared_ptr<ColorModel>     pFillModel;
	float                           percentage = 1.f;
	ProgressDirection               direction;
};


export
class UIText :
	public AUIElement
{
public:
	UIText() = default;
	UIText(const FRect& position, const std::wstring& text, const std::wstring& font, float size, const D2D1_COLOR_F& color)
		:
		AUIElement(position),
		text(text),
		font(font),
		size(size),
		color(color)
	{}

	void    SetText(const std::wstring& newText)
	{
		text = newText;
	}
	void    SetFont(const std::wstring& newFont)
	{
		font = newFont;
	}
	void    SetSize(float newSize)
	{
		size = newSize;
	}
	void    SetColor(const D2D1_COLOR_F& newColor)
	{
		color = newColor;
	}

	void    Render() const override
	{
		TEXT_ENGINE.Render(text, font, size, position, color);
	}

private:
	std::wstring    text;
	std::wstring    font;
	float           size;
	D2D1_COLOR_F    color;
};


export
class UITextBox :
	public AUIElement
{
public:
	UITextBox() = default;
	UITextBox(const FRect& position, const DirectX::XMFLOAT4& outlineColor, const std::wstring& font, const std::wstring& defaultText)
		:
		AUIElement(position),
		pOutlineModel(std::make_shared<ColorModel>(FRect(position.pos, position.width + 5, position.height + 5), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f))),
		pBoxModel(std::make_shared<ColorModel>(position, DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f))),
		font(font),
		defaultText(defaultText),
		bIsFocused(false)
	{
		pOutlineModel->Initialize();
		pOutlineModel->SetDepth(depth - 2.f);

		pBoxModel->Initialize();
		pBoxModel->SetDepth(depth);
	}

	void            SetPosition(const FRect& newPosition) override
	{
		AUIElement::SetPosition(newPosition);
		pOutlineModel->SetRectangle(FRect(newPosition.pos, newPosition.width, newPosition.height));
		pBoxModel->SetRectangle(newPosition);
	}
	void            SetOutlineColor(const DirectX::XMFLOAT4& newOutlineColor) const
	{
		pOutlineModel->SetColor(newOutlineColor);
	}

	virtual void    Update() override
	{
		// When we click on the TextBox, we set the Focus state on
		if (MOUSE.Read() == Mouse::EventType::LPress)
			bIsFocused = position.ContainsVec2D(MOUSE_POSITION);

		// When the Focus state is on, we can read the Keyboard inputs
		if (bIsFocused)
		{
			const char c = KBD.ReadChar();
			if (c >= 32) text += c;
			else if (c == 8 && !text.empty()) text.erase(text.size() - 1);
		}
	}
	virtual void    Render() const override
	{
		pBoxModel->Render();
		pOutlineModel->Render();

		// Default text or the User text is displayed, depending on the entry from the user
		if (bIsFocused || !text.empty()) TEXT_ENGINE.Render(text, font, position.height / 2.f, position, D2D1::ColorF(D2D1::ColorF::White));
		else TEXT_ENGINE.Render(defaultText, font, position.height / 2.f, position, D2D1::ColorF(D2D1::ColorF::LightGray));

	}

	std::string     GetText() const
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
		return convert.to_bytes(text);
	}

private:
	std::shared_ptr<ColorModel>     pOutlineModel;
	std::shared_ptr<ColorModel>     pBoxModel;
	std::wstring                    font;
	std::wstring                    defaultText;
	std::wstring                    text;
	bool                            bIsFocused;
};

