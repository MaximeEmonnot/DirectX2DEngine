#pragma once

#include <memory>

#include "AUIElement.h"

class AUICanvas
{
public:
	AUICanvas() = default;
	virtual ~AUICanvas() = default;

	virtual void	Update() = 0;
	void			Render() const;

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

