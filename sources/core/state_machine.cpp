#include "djah/core/state_machine.hpp"

#include <algorithm>

#include "djah/debug/assertion.hpp"

namespace djah {

    //----------------------------------------------------------------------------------------------
    enum
    {
        STATE_INVALID = -1
    };

    //----------------------------------------------------------------------------------------------
    state_machine::state::state()
    : pOwner_(nullptr)
    , stateId_(STATE_INVALID)
    , parentStateId_(STATE_INVALID)
    , initialChildStateId_(STATE_INVALID)
    , activeChildStateId_(STATE_INVALID)
    , active_(false)
    {

    }


    //----------------------------------------------------------------------------------------------
    void state_machine::state::enter()
    {
        check(pOwner_->isValidState(stateId_));
        check(!active_);
        check( !pOwner_->isValidState(parentStateId_) || (pOwner_->isValidState(parentStateId_) && pOwner_->states_[parentStateId_].activeChildStateId_ == stateId_) );

        active_= true;
        pOwner_->statesStack_.push_back(&(pOwner_->states_[stateId_]));

        if(enter_)
        {
            enter_();
        }

        if( pOwner_->isValidState(activeChildStateId_) )
        {
            pOwner_->states_[activeChildStateId_].enter();
        }
    }

    //----------------------------------------------------------------------------------------------
    void state_machine::state::update()
    {
        check(active_);

        if(pOwner_->isValidState(activeChildStateId_))
        {
            pOwner_->states_[activeChildStateId_].update();
        }

        if(update_)
        {
            update_();
        }
    }

    //----------------------------------------------------------------------------------------------
    void state_machine::state::exit()
    {
        check(active_);

        if(pOwner_->isValidState(activeChildStateId_))
        {
            pOwner_->states_[activeChildStateId_].exit();
        }

        if(exit_)
        {
            exit_();
        }

        check(pOwner_->statesStack_.back() == this);
        pOwner_->statesStack_.resize(pOwner_->statesStack_.size() -1);

        active_ = false;
        activeChildStateId_ = initialChildStateId_;
    }

    //----------------------------------------------------------------------------------------------
    bool state_machine::state::process()
    {
        if(pOwner_->isValidState(activeChildStateId_))
        {
            if ( pOwner_->states_[activeChildStateId_].process())
            {
                return true;
            }
        }

        auto itEnd = transitions_.end();
        for(auto it = transitions_.begin(); it != itEnd; ++it)
        {
            if(it->test_())
            {
                processTransition(*it);
                return true;
            }
        }

        return false;
    }

    //----------------------------------------------------------------------------------------------
    void state_machine::state::processTransition(const state_machine::transition &tr)
    {
        int targetStateId = tr.toStateId_;

        // Build final stack
        std::vector< state *> targetStack;
        state *tmpState = &(pOwner_->states_[targetStateId]);

        while (tmpState != nullptr)
        {
            targetStack.insert(targetStack.begin(), tmpState);

            // move to parent state
            if (pOwner_->isValidState(tmpState->parentStateId_))
            {
                tmpState = &(pOwner_->states_[tmpState->parentStateId_]);
            }
            else
            {
                tmpState = nullptr;
            }
        }

        // Locate common parent between target and current stack
        int deepestSharedStateId = STATE_INVALID;
        int minStackSize = std::min(pOwner_->statesStack_.size(), targetStack.size());

        for (int i = 0; i < minStackSize; ++i)
        {
            if (pOwner_->statesStack_[i]->stateId_ != targetStack[i]->stateId_ )
            {
                break;
            }
            deepestSharedStateId = i;
        }

        check(deepestSharedStateId != STATE_INVALID);

        // deepest shared state is the actual stack deepest state [self transition]
        bool isSelfTransition = deepestSharedStateId == int(pOwner_->statesStack_.size()) - 1;

        if (isSelfTransition)
        {
            pOwner_->statesStack_[deepestSharedStateId]->exit();
        }
        else
        {
            // Exit children of deepest shared state [bottom-up]
            pOwner_->statesStack_[deepestSharedStateId + 1]->exit();
        }

        // Setup initial states in case we don't go trough default initial ones
        for (int i = deepestSharedStateId; i < (int)targetStack.size() - 1; ++i)
        {
            targetStack[i]->activeChildStateId_ = targetStack[i+1]->stateId_;
        }

        // Enter states until we reach the final stack state [top-down]
        int topInActiveChildStateId = STATE_INVALID;
        if (isSelfTransition)
        {
            topInActiveChildStateId = targetStack[deepestSharedStateId]->stateId_;
        }
        else
        {
            topInActiveChildStateId = targetStack[deepestSharedStateId]->activeChildStateId_;

        }

        check(pOwner_->isValidState(topInActiveChildStateId));

        pOwner_->states_[topInActiveChildStateId].enter();

        // validate that state stack is what was expected
        check(pOwner_->statesStack_.size() >= targetStack.size());

        for (int i=0; i < int(std::min(pOwner_->statesStack_.size(), targetStack.size())); ++i)
        {
            check(pOwner_->statesStack_[i]->stateId_ == targetStack[i]->stateId_);
        }
    }

    //----------------------------------------------------------------------------------------------
    state_machine::state_machine(int statesCount)
    {
        if(statesCount > 0)
        {
            init(statesCount);
        }
    }

    //----------------------------------------------------------------------------------------------
    void state_machine::init(int statesCount)
    {
        states_.resize(statesCount);
    }

    //----------------------------------------------------------------------------------------------
    void state_machine::addState(int stateId, int parentStateId, int initialChildStateId, const state_callback_t &onEnter, const state_callback_t &onUpdate, const state_callback_t &onExit)
    {
        check(isValidState(stateId));
        
        states_[stateId].pOwner_                = this;
        states_[stateId].stateId_               = stateId;
        states_[stateId].parentStateId_         = parentStateId;
        states_[stateId].initialChildStateId_   = initialChildStateId;
        states_[stateId].activeChildStateId_    = initialChildStateId;

        states_[stateId].enter_  = onEnter;
        states_[stateId].update_ = onUpdate;
        states_[stateId].exit_   = onExit;
    }

    //----------------------------------------------------------------------------------------------
    void state_machine::addTransition(int transitionId, int fromStateId, int toStateId, const test_callback_t &test)
    {
        check(isValidState(fromStateId));
        check(isValidState(toStateId));

        transition tr;
        tr.fromStateId_     = fromStateId;
        tr.toStateId_       = toStateId;
        tr.transitionId_    = transitionId;
        tr.test_            = test;

        states_[fromStateId].transitions_.push_back(tr);
    }

    //----------------------------------------------------------------------------------------------
    void state_machine::setInitialState(int stateId, int initialChildStateId)
    {
        check(isValidState(stateId));
        check(isValidState(initialChildStateId));
        check(states_[initialChildStateId].parentStateId_ == stateId);

        states_[stateId].initialChildStateId_ = initialChildStateId;
        states_[stateId].activeChildStateId_  = initialChildStateId;
    }
    
    //----------------------------------------------------------------------------------------------
    bool state_machine::validate() const
    {
        bool isValid = true;
        
        const unsigned int statesCount = states_.size();
        for(unsigned int i = 0; i < statesCount; ++i)
        {
            const state &s = states_[i];
            isValid  = s.pOwner_  == this;
            isValid &= s.stateId_ == int(i);
            
            check(isValid);
        }
        
        return isValid;
    }

    //----------------------------------------------------------------------------------------------
    void state_machine::execute()
    {
        states_[0].update();
        states_[0].process();
    }

    //----------------------------------------------------------------------------------------------
    bool state_machine::trigger(int fromStateId, int toStateId)
    {
        check(isValidState(fromStateId));
        check(isValidState(toStateId));

        if(states_[fromStateId].active_)
        {
            auto itEnd = states_[fromStateId].transitions_.end();
            for(auto it = states_[fromStateId].transitions_.begin(); it != itEnd; ++it)
            {
                if(it->toStateId_ == toStateId)
                {
                    states_[fromStateId].processTransition(*it);
                    return true;
                }
            }
        }
        return false;
    }
    
    //----------------------------------------------------------------------------------------------
    bool state_machine::trigger(int transitionId)
    {
        auto itEnd = statesStack_.end();
        for(auto it = statesStack_.begin(); it != itEnd; ++it)
        {
            auto trItEnd = (*it)->transitions_.end();
            for(auto trIt = (*it)->transitions_.begin(); trIt != trItEnd; ++trIt)
            {
                if( trIt->transitionId_ == transitionId )
                {
                    (*it)->processTransition(*trIt);
                    return true;
                }
            }
        }
        return false;
    }

} /*djah*/