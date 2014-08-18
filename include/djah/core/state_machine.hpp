#ifndef DJAH_CORE_STATE_MACHINE_HPP
#define DJAH_CORE_STATE_MACHINE_HPP

#include <vector>
#include <functional>
#include "djah/platform.hpp"


//----------------------------------------------------------------------------------------------
#ifdef DJAH_COMPILE_WINDOWS
#define DECLARE_CURRENT_STATE(STATE) enum { __currentState = STATE }
#else
#define DECLARE_CURRENT_STATE(STATE) const auto __currentState = STATE
#endif

//----------------------------------------------------------------------------------------------
#define fsmBeginMachine(ROOT) \
    do\
    {\
        DECLARE_CURRENT_STATE(ROOT);\
        (void)__currentState;\
        addState(ROOT,-1,-1, [this](){enter<ROOT>();}, [this](){update<ROOT>();}, [this](){exit<ROOT>();})

#define fsmEndMachine()\
    } while(0)
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
#define fsmBeginState(...)\
    do\
    {\
        VA_ARGS_MACRO(fsmBeginState_, __VA_ARGS__)

#define fsmBeginState_1(STATE)\
        addState(STATE, __currentState, -1, [this](){enter<STATE>(); },[this](){update<STATE>(); },[this](){exit<STATE>();});\
        {\
            DECLARE_CURRENT_STATE(STATE);\
            (void)__currentState

#define fsmBeginState_2(STATE, INITIAL)\
        addState(STATE, __currentState, -1, [this](){enter<STATE>(); },[this](){update<STATE>(); },[this](){exit<STATE>();});\
        setInitialState(__currentState, STATE);\
        {\
            DECLARE_CURRENT_STATE(STATE);\
            (void)__currentState

#define fsmEndState()\
        }\
    } while(0)
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
#define fsmTransition(TO_STATE)\
    fsmTransitionOnTrigger(-1, TO_STATE)

#define fsmTransitionOnTrigger(TRIGGER_ID, TO_STATE)\
    addTransition(TRIGGER_ID, __currentState, TO_STATE, [=]()->bool{ return test<__currentState, TO_STATE>(); })

#define fsmTransitionOnEvent(EVENT_TYPE, TO_STATE)
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
#define fsmDeclare(STATES)\
    template<STATES>            void enter()    {}\
    template<STATES>            void update()   {}\
    template<STATES>            void exit()     {}\
    template<STATES, STATES>    bool test()     { return true; }
//----------------------------------------------------------------------------------------------

namespace djah {

    //----------------------------------------------------------------------------------------------
    class state_machine
    {
    private:
        typedef std::function<bool()> test_callback_t;
        typedef std::function<void()> state_callback_t;

        //----------------------------------------------------------------------------------------------
        struct transition
        {
            test_callback_t test_;
            int fromStateId_;
            int toStateId_;
            int transitionId_;
        };

        //----------------------------------------------------------------------------------------------
        struct state
        {
            state();

            void enter();
            void update();
            void exit();
            bool process();
            void processTransition(const transition &tr);

            state_callback_t enter_;
            state_callback_t update_;
            state_callback_t exit_;

            state_machine *pOwner_;
            int stateId_;
            int parentStateId_;
            int initialChildStateId_;
            int activeChildStateId_;
            bool active_;
            std::vector<transition> transitions_;
        };

    public:
        state_machine(int statesCount = 0);
        virtual ~state_machine() {}

        void init(int statesCount);
        virtual void setup() = 0;

        void addState(int stateId, int parentStateId, int initialChildStateId,
                      const state_callback_t &onEnter, const state_callback_t &onUpdate, const state_callback_t &onExit);
        void addTransition(int transitionId_, int fromStateId, int toStateId, const test_callback_t &test);
        void setInitialState(int stateId, int initialChildStateId);
        
        bool validate() const;

        void execute();
        bool trigger(int fromStateId, int toStateId);
        bool trigger(int transitionId);

    protected:
        bool isValidState(int stateId) const { return (stateId >= 0) && (stateId < (int)states_.size()); }
        void launch() { states_[0].enter(); }

    protected:
        std::vector<state> states_;
        std::vector<state*> statesStack_;
    };

} /*djah*/

#endif /* DJAH_CORE_STATE_MACHINE_HPP */