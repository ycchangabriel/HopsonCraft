#ifndef TOGGLE_OPTION_BUTTON_H_INCLUDED
#define TOGGLE_OPTION_BUTTON_H_INCLUDED

#include "Component.h"

#include "../Display.h"
#include "../Renderer/RMaster.h"

namespace GUI
{
    template<typename T>
    class Toggle_Option_Button : public Component
    {
        public:
            Toggle_Option_Button(std::string&& title,
                                 std::vector<std::string>&& order,
                                 std::vector<T>&&     values,
                                 T& option)
            :   m_options       (std::move(order))
            ,   m_optionValues  (std::move(values))
            ,   m_pOption       (&option)
            ,   m_label         (std::move(title))
            {
                m_quad.setSize({500, 100});
                m_quad.setTexture(&Component::guiTexture);
                m_quad.setOutlineColor(sf::Color::Black);
                m_quad.setOutlineThickness(4);

                nextOption();
            }

            void input (const sf::Event& e) override
            {
                if (touchingMouse(m_quad))
                {
                    m_quad.setFillColor(sf::Color::Blue);
                }
                else
                {
                    m_quad.setFillColor(sf::Color::White);
                }

                if(clicked(m_quad, e))
                {
                    nextOption();
                }
            }

            void update () override{}

            void draw   (Renderer::Master& renderer) override
            {
                renderer.draw(m_quad);
                renderer.draw(m_text);
            }

            void setPosition (const sf::Vector2f& position) override
            {
                m_quad.setPosition(position);
                m_text.setPosition(m_quad.getPosition());
                m_text.move(m_quad.getSize().x / 2 - m_text.getLocalBounds().width  / 2,
                            m_quad.getSize().y / 2 - m_text.getLocalBounds().height / 2);
            }

            const sf::Vector2f getSize () const override
            {
                return m_quad.getSize();
            }

        private:
            void nextOption()
            {
                m_optionIndex++;
                if (m_optionIndex > (int32_t)m_options.size() - 1 )
                {
                    m_optionIndex = 0;
                }
                initText(m_text, 24, m_label + ": " + m_options[m_optionIndex]);
                *m_pOption = m_optionValues[m_optionIndex];

                m_text.setPosition(m_quad.getPosition());
                m_text.move(m_quad.getSize().x / 2 - m_text.getLocalBounds().width  / 2,
                            m_quad.getSize().y / 2 - m_text.getLocalBounds().height / 2);
            }

            sf::RectangleShape  m_quad;
            sf::Text            m_text;

            std::vector<std::string> m_options;
            std::vector<T>     m_optionValues;
            T* m_pOption;
            std::string m_label;
            int32_t m_optionIndex = -1;
    };
}

#endif // TOGGLE_OPTION_BUTTON_H_INCLUDED
