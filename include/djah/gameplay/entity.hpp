#ifndef DJAH_GAMEPLAY_ENTITY_HPP
#define DJAH_GAMEPLAY_ENTITY_HPP

#include <string>
#include <iostream>
#include "djah/gameplay/components_punch_card.hpp"

namespace djah { namespace gameplay {

    template<typename ComponentsTypeList>
    class entity_t
        : public components_punch_card<ComponentsTypeList>
    {
    public:
        //----------------------------------------------------------------------------------------------
        entity_t(const std::string &goName = "unnamed_gameobject")
            : name_(goName)
            , enabled_(true)
        {
            std::cout << "Constructor " << name_ << std::endl;
        }

        //----------------------------------------------------------------------------------------------
        ~entity_t()
        {
            std::cout << "Destructor " << name_ << std::endl;
        }

        //----------------------------------------------------------------------------------------------
        entity_t(const entity_t<ComponentsTypeList> &rhs) = delete;

        //----------------------------------------------------------------------------------------------
        entity_t<ComponentsTypeList>& operator =(const entity_t<ComponentsTypeList> &rhs) = delete;

        //----------------------------------------------------------------------------------------------
        entity_t(entity_t<ComponentsTypeList> &&rhs)
            : components_punch_card<ComponentsTypeList>(std::move(rhs))
            , name_(rhs.name_)
            , enabled_(rhs.enabled_)
        {
            std::cout << "Move Constructor " << name_ << std::endl;
        }

        //----------------------------------------------------------------------------------------------
        entity_t<ComponentsTypeList>& operator =(entity_t<ComponentsTypeList> &&rhs)
        {
            if (this != &rhs)
            {
                std::cout << "Move Assignment Operator " << name_ << " = " << rhs.name_ << std::endl;

                name_ = rhs.name_;
                enabled_ = rhs.enabled_;
                components_punch_card_visitor<ComponentsTypeList>::move(rhs, *this);
            }
            return (*this);
        }

        //----------------------------------------------------------------------------------------------
        entity_t clone(const std::string &cloneName = "")
        {
            entity_t _clone(cloneName.empty() ? name_ + "_clone" : cloneName);
            components_punch_card<ComponentsTypeList>::clone(_clone);
            return _clone;
        }

        //----------------------------------------------------------------------------------------------
        const std::string& name() const
        {
            return name_;
        }
        
        //----------------------------------------------------------------------------------------------
        std::string& name()
        {
            return name_;
        }
        
        //----------------------------------------------------------------------------------------------
        bool isEnabled() const
        {
            return enabled_;
        }
        
        //----------------------------------------------------------------------------------------------
        void setEnabled(bool enabled)
        {
            enabled_ = enabled;
        }
        
        //----------------------------------------------------------------------------------------------
        void enable()
        {
            setEnabled(true);
        }
        
        //----------------------------------------------------------------------------------------------
        void disable()
        {
            setEnabled(false);
        }

    private:
        std::string name_;
        bool		enabled_;
    };

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_ENTITY_HPP */