#pragma once

#include <vector>

// pour figer les états
#define DEBUG true

template <typename T>
class StateBase
{
public:
	virtual void Start(T* type) = 0;
	virtual void Update(T* type, float dt) = 0;
	virtual void End(T* type) = 0;

	virtual bool CanTransitionFrom(T* type, int currentState) { return true; };
};

template<typename T>
class Condition
{
public:
	virtual bool Test(T* type) = 0;
};


template<typename T>
class Transition
{
	int m_targetState;
	std::vector<Condition<T>*> m_conditions;

public:
	Transition(int targetState)
	{
		m_targetState = targetState;
	}

	void AddCondition(Condition<T>* condition)
	{
		m_conditions.push_back(condition);
	}

	int TryTransition(T* type)
	{
		for (int i = 0; i < m_conditions.size(); ++i)
		{
			if (m_conditions[i]->Test(type) == false)
				return -1;
		}

		return m_targetState;
	}
};

template<typename T>
class StateMachine {
private:

	int m_currentState = 0;
	std::vector<StateBase<T>*> m_states;
	std::vector<std::vector<Transition<T>*>> m_transitions;

public:
	void AddState(StateBase<T>* newState)
	{
		m_states.push_back(newState);

		m_transitions.resize(m_states.size());
	}

	void Update(T* type, float dt)
	{
		if (DEBUG == false)
			m_states[m_currentState]->Update(type, dt);

		std::vector<Transition<T>*>& transitions = m_transitions[m_currentState];
		for (int i = 0; i < transitions.size(); ++i)
		{
			int nextState = transitions[i]->TryTransition(type);
			if (nextState == -1)
				continue;

			TransitionTo(type, nextState);
			break;
		}
	}

	void TransitionTo(T* type, int nextState)
	{
		if (m_states[(int)nextState]->CanTransitionFrom(type, (int)m_currentState) == false)
			return;

		if (DEBUG == false)
			m_states[(int)m_currentState]->End(type);

		m_currentState = nextState;

		if (DEBUG == false)
			m_states[(int)m_currentState]->Start(type);
	}

	Transition<T>* AddTransition(int fromState, int toState)
	{
		Transition<T>* transition = new Transition<T>(toState);
		m_transitions[fromState].push_back(transition);

		return transition;
	}

	int GetState() const
	{
		return m_currentState;
	}
};