#pragma once
#include<vector>


//TODO:Because some templates can share lets call it the same signature this can cause problems for example both onScrollChange and onMousePosChange return 
// x and y positions as offset and because of this the update methods overlap one way to fix this is the messageType but this also makes the templates almost pointless
// another way i thought is to make for example the scroll function return only the Y offset but this is a solution that is reling not that much on softwere design but
// more on tricks;
enum MessageType
{
	KEY_PRESSES =1,
	MOUSE_BUTTON_PRESSED =2,
	MOUSE_POSITION_CHANGED =3,
	MOUSE_SCROLL_CHANGED = 4,
	WINDOW_SIZE_CHANGED = 5
};

template<typename... params>
class IObserver {
public:
	//(NOTE)Might be better to template it.
	virtual void Update(MessageType type,params ...args) = 0;
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
	virtual void Notify(MessageType type,params ...args)
	{
		for (auto* observer : m_observers)
		{
			observer->Update(type,args...);
		}
	}
private:
	std::vector<IObserver<params ...>*> m_observers;
};
