#pragma once
#include<vector>
class IObserver {
public:
	virtual void Update(float deltaMouseX, float deltaMouseY) = 0;
};
class INotifier {
public:
	virtual void Attach(IObserver* observer) = 0;
	virtual void Detach(IObserver* observer) = 0;
	virtual void Notify() = 0;
private:
	std::vector<IObserver*> m_observers;
};
