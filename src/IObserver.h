#pragma once
#include<vector>
template<typename... params>
class IObserver {
public:
	//(NOTE)Might be better to template it.
	virtual void Update(params ...args) = 0;
};
template<typename... params>
class Notifier {
public:
	Notifier()
	{
		m_observers = { };
	}
	virtual void Attach(IObserver<params ...>* observer)
	{
		this->m_observers.push_back(observer);
	}
	virtual void Detach(IObserver<params ...>* observer)
	{
		for (size_t i =0; i < m_observers.size();i++)
		{
			if (observer == m_observers[i])
			{
				m_observers.erase(m_observers.begin()+i);
				break;
			}			
		}
	}
	virtual void Notify(params ...args)
	{
		for (auto* observer : m_observers)
		{
			observer->Update(args...);
		}
	}
private:
	std::vector<IObserver<params ...>*> m_observers;
};
