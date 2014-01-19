#ifndef DJAH_GAMEPLAY_SERVICES_ACTION_SERVICE_HPP
#define DJAH_GAMEPLAY_SERVICES_ACTION_SERVICE_HPP

#include <map>
#include <string>
#include "djah/core/typelist.hpp"
#include "djah/core/string_utils.hpp"
#include "djah/system/input/keyboard.hpp"
#include "djah/system/input/gamepad.hpp"
#include "djah/gameplay/services/game_service.hpp"
#include "djah/gameplay/components/action_map.hpp"

namespace djah { namespace gameplay { namespace services {

	typedef TYPELIST(components::action_map) ActionServiceComponents;

	template<typename ComponentTypeList>
	class action_service
		: public game_service<ComponentTypeList, ActionServiceComponents>
	{
	public:
		action_service(const system::input::keyboard &_kb, const system::input::gamepad &_gp)
			: kb(_kb)
			, gp(_gp)
		{
			inputKBDico["KEY_UP"]    = system::input::eKC_UP;
			inputKBDico["KEY_DOWN"]  = system::input::eKC_DOWN;
			inputKBDico["KEY_LEFT"]  = system::input::eKC_LEFT;
			inputKBDico["KEY_RIGHT"] = system::input::eKC_RIGHT;
			inputKBDico["KEY_E"]     = system::input::eKC_E;
			inputKBDico["KEY_Q"]     = system::input::eKC_Q;
			inputKBDico["KEY_A"]     = system::input::eKC_A;
			inputKBDico["KEY_D"]     = system::input::eKC_D;
			inputKBDico["KEY_W"]     = system::input::eKC_W;
			inputKBDico["KEY_S"]     = system::input::eKC_S;

			inputX360AxisDico["X360_AXIS_RIGHT_Xp"] = system::input::eX360_RightX;
			inputX360AxisDico["X360_AXIS_RIGHT_Xm"] = system::input::eX360_RightX;
			inputX360AxisDico["X360_AXIS_RIGHT_Yp"] = system::input::eX360_RightY;
			inputX360AxisDico["X360_AXIS_RIGHT_Ym"] = system::input::eX360_RightY;
			inputX360AxisDico["X360_AXIS_LEFT_Xp"]  = system::input::eX360_LeftX;
			inputX360AxisDico["X360_AXIS_LEFT_Xm"]  = system::input::eX360_LeftX;
			inputX360AxisDico["X360_AXIS_LEFT_Yp"]  = system::input::eX360_LeftY;
			inputX360AxisDico["X360_AXIS_LEFT_Ym"]  = system::input::eX360_LeftY;
		}

	protected:
		virtual void executeFor(game_object_t &gameObject, float dt)
		{
			component<components::action_map> actionMap = gameObject.get<components::action_map>();

			std::for_each(actionMap->actions.begin(), actionMap->actions.end(),
			[&](const components::action_map::actions_map_t::value_type &entry)
			{
				actionMap->states[entry.first] = 0.0f;

				std::for_each(entry.second.begin(), entry.second.end(),
				/*[&](const std::string &sequenceStr)
				{
					if( actionMap->states[entry.first] != 0.0f )
					{
						return;
					}

					utils::string_list_t sequence;
					utils::split_string(sequenceStr, sequence, "+");

					float val = 1.0f;

					std::for_each(sequence.begin(), sequence.end(),*/
					[&](const std::string &inputStr)
					{
						if( inputStr[0] == 'K' && inputStr[1] == 'E' && inputStr[2] == 'Y' && inputStr[3] == '_' )
						{
							if( kb.isKeyDown(inputKBDico[inputStr]) )
							{
								actionMap->states[entry.first] = 1.0f;
							}
						}
						else if( gp.isPlugged() && inputStr[0] == 'X' && inputStr[1] == '3' && inputStr[2] == '6' && inputStr[3] == '0' && inputStr[4] == '_' )
						{
							if( inputStr[5] == 'A' && inputStr[6] == 'X' && inputStr[7] == 'I' && inputStr[8] == 'S' )
							{
								const float axisValue = gp.getAxis(inputX360AxisDico[inputStr]).value();
								if( axisValue > 0.0f && inputStr[inputStr.size()-1] == 'p' )
								{
									actionMap->states[entry.first] = axisValue;
								}
								else if( axisValue < 0.0f && inputStr[inputStr.size()-1] == 'm' )
								{
									actionMap->states[entry.first] = -axisValue;
								}
							}
						}
					});

					//actionMap->states[entry.first] = val;
				//});
			});
		}

	private:
		const system::input::keyboard &kb;
		const system::input::gamepad &gp;
		std::map<std::string, system::input::eKeyCode>		inputKBDico;
		std::map<std::string, system::input::eXbox360Axis>	inputX360AxisDico;
	};

} /*services*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_SERVICES_ACTION_SERVICE_HPP */