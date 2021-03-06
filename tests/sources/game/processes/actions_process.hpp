#ifndef GAME_PROCESSES_ACTIONS_PROCESS_HPP
#define GAME_PROCESSES_ACTIONS_PROCESS_HPP

#include <map>
#include <string>

#include "djah/core/typelist.hpp"
#include "djah/core/string_utils.hpp"
#include "djah/system/input/mouse.hpp"
#include "djah/system/input/keyboard.hpp"
#include "djah/system/input/gamepad.hpp"
#include "djah/gameplay/component_process.hpp"

#include "game/components/action_map.hpp"


namespace game { namespace processes {

	typedef TYPELIST(components::action_map) ActionsProcessComponents;

	template<typename ComponentTypeList>
	class actions_process
        : public djah::gameplay::component_process<ComponentTypeList, ActionsProcessComponents>
	{
	public:
        actions_process(const djah::system::input::mouse &_m, const djah::system::input::keyboard &_kb, const djah::system::input::gamepad &_gp)
			: m(_m)
			, kb(_kb)
			, gp(_gp)
		{
			inputKBDico["KEY_UP"]    = djah::system::input::eKC_UP;
			inputKBDico["KEY_DOWN"]  = djah::system::input::eKC_DOWN;
			inputKBDico["KEY_LEFT"]  = djah::system::input::eKC_LEFT;
			inputKBDico["KEY_RIGHT"] = djah::system::input::eKC_RIGHT;
			inputKBDico["KEY_E"]     = djah::system::input::eKC_E;
			inputKBDico["KEY_Q"]     = djah::system::input::eKC_Q;
			inputKBDico["KEY_A"]     = djah::system::input::eKC_A;
			inputKBDico["KEY_D"]     = djah::system::input::eKC_D;
			inputKBDico["KEY_W"]     = djah::system::input::eKC_W;
			inputKBDico["KEY_S"]     = djah::system::input::eKC_S;

			inputX360AxisDico["X360_AXIS_RIGHT_X+"] = djah::system::input::eX360_RightX;
			inputX360AxisDico["X360_AXIS_RIGHT_X-"] = djah::system::input::eX360_RightX;
			inputX360AxisDico["X360_AXIS_RIGHT_Y+"] = djah::system::input::eX360_RightY;
			inputX360AxisDico["X360_AXIS_RIGHT_Y-"] = djah::system::input::eX360_RightY;
			inputX360AxisDico["X360_AXIS_LEFT_X+"]  = djah::system::input::eX360_LeftX;
			inputX360AxisDico["X360_AXIS_LEFT_X-"]  = djah::system::input::eX360_LeftX;
			inputX360AxisDico["X360_AXIS_LEFT_Y+"]  = djah::system::input::eX360_LeftY;
			inputX360AxisDico["X360_AXIS_LEFT_Y-"]  = djah::system::input::eX360_LeftY;

			inputX360BtnDico["X360_BTN_A"]				= djah::system::input::eX360_A;
			inputX360BtnDico["X360_BTN_B"]				= djah::system::input::eX360_B;
			inputX360BtnDico["X360_BTN_X"]				= djah::system::input::eX360_X;
			inputX360BtnDico["X360_BTN_Y"]				= djah::system::input::eX360_Y;
			inputX360BtnDico["X360_BTN_BACK"]			= djah::system::input::eX360_Back;
			inputX360BtnDico["X360_BTN_START"]			= djah::system::input::eX360_Start;
			inputX360BtnDico["X360_BTN_LEFT_THUMB"]		= djah::system::input::eX360_LeftThumb;
			inputX360BtnDico["X360_BTN_RIGHT_THUMB"]	= djah::system::input::eX360_RighThumb;
			inputX360BtnDico["X360_BTN_DPAD_UP"]		= djah::system::input::eX360_DPad_Up;
			inputX360BtnDico["X360_BTN_DPAD_DOWN"]		= djah::system::input::eX360_DPad_Down;
			inputX360BtnDico["X360_BTN_DPAD_LEFT"]		= djah::system::input::eX360_DPad_Left;
			inputX360BtnDico["X360_BTN_DPAD_RIGHT"]		= djah::system::input::eX360_DPad_Right;
			inputX360BtnDico["X360_BTN_LB"]				= djah::system::input::eX360_LB;
			inputX360BtnDico["X360_BTN_RB"]				= djah::system::input::eX360_RB;
		}

	protected:
		virtual void executeFor(game_object_t &gameObject, float dt)
		{
            djah::gameplay::component<components::action_map> actionMap = gameObject.get<components::action_map>();

			std::for_each(actionMap->actions.begin(), actionMap->actions.end(),
			[&](const components::action_map::actions_map_t::value_type &entry)
			{
				const std::string &actionName = entry.first;
				float &actionState = actionMap->states[actionName];
				actionState = 0.0f;

				const auto &sequences = entry.second;
				auto itEnd = sequences.end();
				for(auto it = sequences.begin(); it != itEnd && actionState == 0.0f; ++it)
				{
					const auto &sequence = (*it);

					float sequenceState = 1.0f;

					std::for_each(sequence.begin(), sequence.end(),
					[&](const std::string &inputStr)
					{
						if( sequenceState == 0.0f )
						{
							return;
						}

                        const std::string &sequenceElementStr = djah::string_utils::trimmed(inputStr);
						float sequenceElementState = 0.0f;

                        if (djah::string_utils::begins_with(sequenceElementStr, "MOUSE_"))
						{
							handleMouseInput(sequenceElementStr, sequenceElementState);
						}
                        else if (djah::string_utils::begins_with(sequenceElementStr, "KEY_"))
						{
							handleKeyboardInput(sequenceElementStr, sequenceElementState);
						}
                        else if (gp.isPlugged() && djah::string_utils::begins_with(sequenceElementStr, "X360_"))
						{
							handleGamepadInput(sequenceElementStr, sequenceElementState);
						}

						sequenceState *= sequenceElementState;
					});

					actionState = sequenceState;
				 }
			});
		}

		void handleMouseInput(const std::string &inputStr, float &actionState)
		{
			if( inputStr == "MOUSE_BTN_LEFT" && m.leftButton().isDown() )
			{
				actionState = 1.0f;
			}
			else if( inputStr == "MOUSE_BTN_MIDDLE" && m.middleButton().isDown() )
			{
				actionState = 1.0f;
			}
			else if( inputStr == "MOUSE_BTN_RIGHT" && m.rightButton().isDown() )
			{
				actionState = 1.0f;
			}
			else if( inputStr == "MOUSE_AXIS_LEFT" && m.delta().x < 0.0f )
			{
				actionState = -float(m.delta().x);
			}
			else if( inputStr == "MOUSE_AXIS_RIGHT" && m.delta().x > 0.0f )
			{
				actionState = float(m.delta().x);
			}
			else if( inputStr == "MOUSE_AXIS_UP" && m.delta().y < 0.0f )
			{
				actionState = float(m.delta().y);
			}
			else if( inputStr == "MOUSE_AXIS_DOWN" && m.delta().y > 0.0f )
			{
				actionState = -float(m.delta().y);
			}
		}

		void handleKeyboardInput(const std::string &inputStr, float &actionState)
		{
			if( kb.isKeyDown(inputKBDico[inputStr]) )
			{
				actionState = 1.0f;
			}
		}

		void handleGamepadInput(const std::string &inputStr, float &actionState)
		{
			if( inputStr.find("_AXIS_") != std::string::npos )
			{
				const float axisValue = gp.getAxis(inputX360AxisDico[inputStr]).value();
                if (axisValue > 0.0f && djah::string_utils::ends_with(inputStr, "+"))
				{
					actionState = axisValue;
				}
                else if (axisValue < 0.0f && djah::string_utils::ends_with(inputStr, "-"))
				{
					actionState = -axisValue;
				}
			}
			else if( inputStr.find("_BTN_") != std::string::npos )
			{
				if( gp.getButton(inputX360BtnDico[inputStr]).isDown() )
				{
					actionState = 1.0;
				}
			}
			else if( inputStr == "X360_TRIGGER_LEFT" )
			{
                actionState = gp.getTrigger(djah::system::input::eX360_LeftTrigger).value();
			}
			else if( inputStr == "X360_TRIGGER_RIGHT" )
			{
                actionState = gp.getTrigger(djah::system::input::eX360_RightTrigger).value();
			}
		}

	private:
        const djah::system::input::mouse    &m;
        const djah::system::input::keyboard &kb;
        const djah::system::input::gamepad  &gp;
        std::map<std::string, djah::system::input::eKeyCode>		inputKBDico;
        std::map<std::string, djah::system::input::eXbox360Axis>	inputX360AxisDico;
        std::map<std::string, djah::system::input::eXbox360Buttons> inputX360BtnDico;
	};

} /*processes*/ } /*game*/

#endif /* GAME_PROCESSES_ACTIONS_PROCESS_HPP */