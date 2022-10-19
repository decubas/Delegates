#ifndef _Delegate_H_
#define _Delegate_H_ 1

#include <vector>
#include <memory>
/*
EXAMPLES:

// No params
Delegate(OnWindowCloseEvent)

// With params
Delegate_OneParam(OnKeyPressedEvent, int, Key)
Delegate_TwoParams(OnKeyPressedEventTwo, int, Key, float, Mat)

// With return values
Delegate_Return(OnKeyPressedEventTwo_Return, bool)

{
	OnKeyPressedEvent m_OnKeyPressed;
	OnKeyPressedEventTwo m_OnKeyPressedTwo;
	OnKeyPressedEvent_Return m_OnKeyPressed_Return;

	m_OnKeyPressed.AddFunction(this, &MyClass::OnKeyPressed);
	m_OnKeyPressedTwo.AddFunction(this, &MyClass::OnKeyPressedTwo);
	m_OnKeyPressed_Return.AddFunction(this, &MyClass::OnKeyPressedTwo_Return);

	m_OnKeyPressed.Broadcast(4);
	m_OnKeyPressedTwo.Broadcast(1, 10.5f);
	if (false == m_OnKeyPressed_Return.Broadcast())
	{
	}
}
*/

	/////////////////////// DELEGATE SYSTEM
	class IDelegateCallbackBase
	{
	public:
	};

	typedef std::vector<std::shared_ptr<IDelegateCallbackBase>> CallbackArrayBase;
	
	class DelegateBase
	{
	public:
		DelegateBase() = default;
		~DelegateBase()
		{
			Clear();
		}

		virtual void Clear()
		{
			actions.clear();
		}

		bool IsBound()
		{
			return actions.size() > 0;
		}

	protected:
		CallbackArrayBase actions;
	};


#define FUNC_CONCAT( ... ) __VA_ARGS__


#define Delegate_Return_AddRemoveMultParams(DelegateName, ReturnType, ParamTypes)\
		template<class T>\
		void AddFunction(void* owner, ReturnType (T::* function_)(ParamTypes))\
		{\
			std::shared_ptr<IDelegateCallbackBase> action = std::make_shared<DelegateCallback_##DelegateName<T>>((T*)owner, function_);\
			CallbackArrayBase::iterator position = find(actions.begin(), actions.end(), action);\
			if (actions.size() > 1)\
			{\
				return;\
			}\
			actions.push_back(action);\
		}\
		template<class T>\
		void RemoveFunction(void* owner, ReturnType (T::* function_)(ParamTypes))\
		{\
			std::shared_ptr<IDelegateCallbackBase> action = std::make_shared<DelegateCallback_##DelegateName<T>>((T*)owner, function_);\
			CallbackArrayBase::iterator position = find(actions.begin(), actions.end(), action);\
			delete action;\
			if (position == actions.end())\
			{\
				return;\
			}\
			actions.erase(position);\
		}

#define Delegate_Broadcast_Return_MultParams(DelegateName, ReturnType, ParamNamesAndTypes, ...)\
		ReturnType Broadcast(ParamNamesAndTypes)\
		{\
			for (std::shared_ptr<IDelegateCallbackBase> action : actions)\
			{\
				IDelegateCallback_##DelegateName* Func = (IDelegateCallback_##DelegateName*)action.get();\
				return (*Func).Broadcast(__VA_ARGS__);\
			}\
		}



#define Delegate_Return_MultParam(DelegateName, ReturnType, ParamTypes, ParamNamesAndTypes, ...)\
	class IDelegateCallback_##DelegateName : public IDelegateCallbackBase\
	{\
	public:\
		virtual ReturnType Broadcast(ParamNamesAndTypes) = 0;\
	};\
	template<class T>\
	class DelegateCallback_##DelegateName : public IDelegateCallback_##DelegateName\
	{\
	public:\
		typedef ReturnType (T::* DelegateFunc)(ParamTypes);\
		DelegateCallback_##DelegateName(T* instance, DelegateFunc function_)\
			: instance(instance), function(function_) {}\
		virtual ReturnType Broadcast(ParamNamesAndTypes) override { return (instance->*function)(__VA_ARGS__); }\
		bool operator == (IDelegateCallback_##DelegateName* other)\
		{\
			DelegateCallback_##DelegateName* otherDelegateCallback = (DelegateCallback_##DelegateName*)other;\
			if (otherDelegateCallback == nullptr)\
				return false;\
				return 	(this->function == otherDelegateCallback->function) &&\
				(this->instance == otherDelegateCallback->instance);\
		}\
	private:\
		DelegateFunc function;\
		T* instance;\
	};\
	class DelegateName : public DelegateBase\
	{\
	public:\
		Delegate_Return_AddRemoveMultParams(DelegateName, ReturnType, ParamTypes)\
		Delegate_Broadcast_Return_MultParams(DelegateName, ReturnType, ParamNamesAndTypes, __VA_ARGS__)\
	};








#define Delegate(DelegateName) Delegate_Return_MultParam(DelegateName, void, void, void, )
#define Delegate_Return(DelegateName, ReturnType) Delegate_Return_MultParam(DelegateName, ReturnType, void, void, )

#define Delegate_OneParam(DelegateName, ParamT1, Param1) Delegate_Return_MultParam(DelegateName, void, ParamT1, FUNC_CONCAT(ParamT1 Param1), Param1)
#define Delegate_TwoParams(DelegateName, ParamT1, Param1, ParamT2, Param2) Delegate_Return_MultParam(DelegateName, void, FUNC_CONCAT(ParamT1, ParamT2), FUNC_CONCAT(ParamT1 Param1, ParamT2 Param2), Param1, Param2)

#define Delegate_ReturnOneParam(DelegateName, ReturnType, ParamT1, Param1) Delegate_Return_MultParam(DelegateName, ReturnType, ParamT1, FUNC_CONCAT(ParamT1 Param1), Param1)
#define Delegate_ReturnTwoParams(DelegateName, ReturnType, ParamT1, Param1, ParamT2, Param2) Delegate_Return_MultParam(DelegateName, ReturnType, FUNC_CONCAT(ParamT1, ParamT2), FUNC_CONCAT(ParamT1 Param1, ParamT2 Param2), Param1, Param2)

#endif
