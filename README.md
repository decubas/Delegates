# Delegates
Delegates with Macros (Similar to Unreal Engine)

## No params

    Delegate(OnWindowCloseEvent)

## With params

    Delegate_OneParam(OnKeyPressedEvent, int, Key)

    Delegate_TwoParams(OnKeyPressedEventTwo, int, Key, float, Mat)

## With return values

    Delegate_Return(OnKeyPressedEvent_Return, bool)

## Code example
 
    OnKeyPressedEvent m_OnKeyPressed;
  
	OnKeyPressedEventTwo m_OnKeyPressedTwo;
  
	OnKeyPressedEvent_Return m_OnKeyPressed_Return;
  
	m_OnKeyPressed.AddFunction(&MyClassObject, &MyClass::OnKeyPressed);
	m_OnKeyPressedTwo.AddFunction(&MyClassObject, &MyClass::OnKeyPressedTwo);
	m_OnKeyPressed_Return.AddFunction(&MyClassObject, &MyClass::OnKeyPressed_Return);

	m_OnKeyPressed.Broadcast(4);
	m_OnKeyPressedTwo.Broadcast(1, 10.5f);
	if (false == m_OnKeyPressed_Return.Broadcast())
	{
	}
