#ifndef DJAH_GAMEPLAY_ENTITY_HPP
#define DJAH_GAMEPLAY_ENTITY_HPP

#include <string>
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
		}

		entity_t clone(const std::string &cloneName = "")
		{
			entity _clone(cloneName.empty() ? name_ + "_clone" : cloneName);
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