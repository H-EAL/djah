#ifndef DJAH_CORE_STATE_MACHINE_HPP
#define DJAH_CORE_STATE_MACHINE_HPP

#include <stack>
#include <vector>
#include <cassert>

#pragma warning(disable : 4482)
#define IsValidState(StateID) (StateID > StatesEnum::STATE_INVALID && StateID < StatesEnum::STATES_COUNT)

namespace djah {

	//----------------------------------------------------------------------------------------------
	template<typename StatesEnum, typename OwnerType>
	struct state_machine;

	//----------------------------------------------------------------------------------------------
	template<typename StatesEnum, typename OwnerType>
	struct transition_base
	{
		transition_base(state_machine<StatesEnum, OwnerType> *pMachine, StatesEnum From, StatesEnum To)
			: pMachine_(pMachine)
			, FromState(From)
			, ToState(To)
		{
		}

		virtual ~transition_base()
		{
		}

		virtual bool test() = 0;
		virtual void fire() = 0;

		state_machine<StatesEnum, OwnerType> *pMachine_;
		StatesEnum FromState;
		StatesEnum ToState;
	};
	//----------------------------------------------------------------------------------------------
	template<typename StatesEnum, StatesEnum EFromState, StatesEnum EToState, typename OwnerType>
	struct transition
		: public transition_base<StatesEnum, OwnerType>
	{
		transition(state_machine<StatesEnum, OwnerType> *pMachine)
			: transition_base(pMachine, EFromState, EToState)
		{
		}

		virtual ~transition()
		{
		}

		virtual bool test()
		{ 
			return pMachine_->pOwner_->test<EFromState, EToState>();
		}

		virtual void fire()
		{
			pMachine_->pOwner_->fire<EFromState, EToState>();
		}
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename StatesEnum, typename OwnerType>
	struct state_base
	{
		state_base(state_machine<StatesEnum, OwnerType> *pMachine, StatesEnum Name, StatesEnum Parent, StatesEnum DefaultChild)
			: pMachine_(pMachine)
			, ActiveChildState(DefaultChild)
			, StateName(Name)
			, ParentState(Parent)
			, DefaultChildState(DefaultChild)
			, Active(false)
		{
		}

		virtual ~state_base()
		{
			auto itEnd = transitions_.end();
			for(auto it = transitions_.begin(); it != itEnd; ++it)
			{
				delete (*it);
			}
		}

		virtual void enter()   = 0;
		virtual void update()  = 0;
		virtual void exit()    = 0;
		virtual bool process() = 0;

		StatesEnum ActiveChildState;
		StatesEnum StateName;
		StatesEnum ParentState;
		StatesEnum DefaultChildState;
		bool       Active;

		state_machine<StatesEnum, OwnerType> *pMachine_;
		std::vector< transition_base<StatesEnum, OwnerType>* > transitions_;
	};
	//----------------------------------------------------------------------------------------------
	template
	<
		typename StatesEnum,
		typename OwnerType,
		StatesEnum EStateName,
		StatesEnum EParentState = StatesEnum::STATE_INVALID,
		StatesEnum EDefaultChildState = StatesEnum::STATE_INVALID
	>
	struct state
		: public state_base<StatesEnum, OwnerType>
	{
		state(state_machine<StatesEnum, OwnerType> *pMachine)
			: state_base(pMachine, EStateName, EParentState, EDefaultChildState)
		{
			ActiveChildState = EDefaultChildState;
		}

		virtual void enter()
		{
			assert( !Active );
			assert( !IsValidState(ParentState) || (IsValidState(ParentState) && pMachine_->pvStates_[ParentState]->ActiveChildState == StateName) );

			Active = true;

			pMachine_->statesStack_.push_back(pMachine_->pvStates_[EStateName]);

			pMachine_->pOwner_->enter<EStateName>();

			if( IsValidState(ActiveChildState) )
				pMachine_->pvStates_[ActiveChildState]->enter();
		}

		virtual void update()
		{
			assert( Active );

			if( IsValidState(ActiveChildState) )
				pMachine_->pvStates_[ActiveChildState]->update();

			pMachine_->pOwner_->update<EStateName>();
		}

		virtual void exit()
		{
			assert(Active);

			if( IsValidState(ActiveChildState) )
				pMachine_->pvStates_[ActiveChildState]->exit();

			pMachine_->pOwner_->exit<EStateName>();

			assert( pMachine_->statesStack_.back() == this );
			pMachine_->statesStack_.resize(pMachine_->statesStack_.size() -1);

			Active = false;
			ActiveChildState = EDefaultChildState;
		}

		virtual bool process()
		{
			if( IsValidState(ActiveChildState) )
			{
				if ( pMachine_->pvStates_[ActiveChildState]->process() )
					return true;
			}

			auto itEnd = transitions_.end();
			for(auto it = transitions_.begin(); it != itEnd; ++it)
			{
				if( (*it)->test() )
				{
					StatesEnum TargetState = (*it)->ToState;

					// Build final stack
					std::vector< state_base<StatesEnum, OwnerType>* > TargetStack;
					state_base<StatesEnum, OwnerType> * TmpState = pMachine_->pvStates_[TargetState];
					while (TmpState != NULL )
					{
						TargetStack.insert(TargetStack.begin(), TmpState);

						// move to parent state
						if (IsValidState(TmpState->ParentState))
							TmpState = pMachine_->pvStates_[TmpState->ParentState];
						else
							TmpState = NULL;
					}

					// Locate common parent between target and current stack
					int DeepestSharedStateIdx	= -1;
					int MinStackSize = std::min(pMachine_->statesStack_.size(), TargetStack.size());
					for (int k = 0; k < MinStackSize; k++)
					{
						if (pMachine_->statesStack_[k]->StateName != TargetStack[k]->StateName )
							break;

						DeepestSharedStateIdx = k;
					}

					assert(DeepestSharedStateIdx != -1);

					// deepest shared state is the actual stack deepest state [self transition]
					bool IsSelfTransition = DeepestSharedStateIdx == pMachine_->statesStack_.size() - 1;

					if (IsSelfTransition)
					{						
						pMachine_->statesStack_[DeepestSharedStateIdx]->exit();
					}
					else
					{
						// Exit children of deepest shared state [bottom-up]
						pMachine_->statesStack_[DeepestSharedStateIdx + 1]->exit();
					}

					// Perform transition action
					(*it)->fire();

					// Setup initial states in case we don't go trough default initial ones
					for (int j = DeepestSharedStateIdx; j < TargetStack.size() - 1; j ++)
					{
						TargetStack[j]->ActiveChildState = TargetStack[j+1]->StateName;
					}

					// Enter states until we reach the final stack state [top-down]
					int TopInActiveChildStateID = StatesEnum::STATE_INVALID;
					if (IsSelfTransition)
						TopInActiveChildStateID = TargetStack[DeepestSharedStateIdx]->StateName;
					else
						TopInActiveChildStateID = TargetStack[DeepestSharedStateIdx]->ActiveChildState;

					assert(IsValidState(TopInActiveChildStateID));

					pMachine_->pvStates_[TopInActiveChildStateID]->enter();

					// validate that state stack is what was expected
					assert (pMachine_->statesStack_.size() >= TargetStack.size());
					for (int z=0; z< std::min(pMachine_->statesStack_.size(), TargetStack.size()); z++)
					{
						assert(pMachine_->statesStack_[z]->StateName == TargetStack[z]->StateName);
					}

					return true;
				}
			}

			return false;
		}
	};

	//////////////////////////////////////////////////////////////////////////

	template<typename StatesEnum, typename OwnerType>
	struct state_machine
	{
		state_machine()
		{
			memset(pvStates_, 0, sizeof(pvStates_));
			pOwner_ = static_cast<OwnerType*>(this);
		}

		virtual ~state_machine()
		{
			for(int i = 0; i < StatesEnum::STATES_COUNT; ++i)
			{
				delete pvStates_[i];
			}
		}

		template<StatesEnum StateName, StatesEnum ParentState, StatesEnum DefaultChildState>
		void AddState()
		{
			assert(pvStates_[StateName] == nullptr);

			pvStates_[StateName] = new state<StatesEnum, OwnerType, StateName, ParentState, DefaultChildState>(this);
		}

		template<StatesEnum StateName, StatesEnum ParentState>
		void AddState()
		{
			AddState<StateName, ParentState, STATE_INVALID>();
		}

		template<StatesEnum StateName>
		void AddState()
		{
			AddState<StateName, STATE_INVALID, STATE_INVALID>();
		}

		template<StatesEnum FromState, StatesEnum ToState>
		void AddTransition()
		{
			assert(IsValidState(FromState));
			assert(IsValidState(ToState));
			assert(pvStates_[FromState] != nullptr);
			assert(pvStates_[ToState] != nullptr);

			pvStates_[FromState]->transitions_.push_back( new transition<StatesEnum, FromState, ToState, OwnerType>(this) );
		}

		template<StatesEnum InitialState>
		void Run()
		{
			pvStates_[InitialState]->update();
			pvStates_[InitialState]->process();
		}

		state_base<StatesEnum, OwnerType> *pvStates_[StatesEnum::STATES_COUNT];
		std::vector< state_base<StatesEnum, OwnerType>* > statesStack_;
		OwnerType *pOwner_;
	};

} /*djah*/

#endif /* DJAH_CORE_STATE_MACHINE_HPP */