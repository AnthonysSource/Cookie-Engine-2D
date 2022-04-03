#include "InputSystem.h"

#include "Core/Common.h"
#include "Core/Logging/Logging.h"

namespace Cookie {
namespace InputSystem {

	struct InputEvent {
		i32 m_KeyCode;
		i32 m_ScanCode;
		i32 m_Action; // Pressed or Released
		i32 m_Mods;
	};

	struct KeyboardInputState {

		// TODO: replace size with actual real size
		bool m_KeyDown[255]; // Key Pressed this frame
		bool m_KeyHeld[255]; // Key being held this frame
		bool m_KeyUp[255];	 // Key Released this frame

		// A key can be down and up in the same frame,
		// that means that it has been pressed and already
		// released
	};

	struct MouseInputState {
		i32 m_XPos;
		i32 m_YPos;

		// This would not support extra
		// mouse buttons
		bool m_ButtonDown[3];
		bool m_ButtonHeld[3];
		bool m_ButtonUp[3];
	};

	struct InputState {
		KeyboardInputState m_Keyboard;
		MouseInputState m_Mouse;
	};

	// Contains a buffer of the input events that have been
	// registered in the current frame
	std::queue<InputEvent> s_InputEventsBuffer;
	InputState s_InputState;
	InputState s_InputStateLastFrame;

	void WindowKeyEventHandle(GLFWwindow *window, int key, int scancode, int action, int mods) {
		// Push input event into queue
		InputEvent e;
		e.m_KeyCode = key;
		e.m_ScanCode = scancode;
		e.m_Action = action;
		e.m_Mods = mods;
		s_InputEventsBuffer.push(e);

		// Save input data
		if (action == 1) {
			s_InputState.m_Keyboard.m_KeyDown[key] = true;
			s_InputState.m_Keyboard.m_KeyHeld[key] = true;
		} else {
			s_InputState.m_Keyboard.m_KeyHeld[key] = false;
			s_InputState.m_Keyboard.m_KeyUp[key] = true;
		}
	}

	void InputSystem::Init(GLFWwindow *window) { glfwSetKeyCallback(window, WindowKeyEventHandle); }

	void InputSystem::Update() {
		// Reset previous input state
		for (size_t i = 0; i < 255; i++) {
			// We dont reset keyheld because that gets
			// reset in a keyup event
			s_InputState.m_Keyboard.m_KeyDown[i] = false;
			s_InputState.m_Keyboard.m_KeyUp[i] = false;
		}

		// Poll All Window Events
		glfwPollEvents();

		// Process input events
		for (i32 i = 0; i < s_InputEventsBuffer.size(); i++) {
			InputEvent e = s_InputEventsBuffer.front();

			// Logging to inspect system state

			// Log::Info(
			// 	"Key Name: %s / Keycode: %d / Scancode : %d / Action: %d / Mod: %d",
			// 	glfwGetKeyName(e.m_KeyCode, e.m_ScanCode), e.m_KeyCode,
			// 	e.m_ScanCode, e.m_Action, e.m_Mods);

			// if (e.m_Action == 0) {
			// 	Log::Info("[%s] Released",
			// 			  glfwGetKeyName(e.m_KeyCode, e.m_ScanCode));
			// } else if (e.m_Action == 1) {
			// 	Log::Info("[%s] Pressed",
			// 			  glfwGetKeyName(e.m_KeyCode, e.m_ScanCode));
			// }

			// Log::Info("Keyname: %s / Event Action: %d / Keydown: %d / Keyheld: %d
			// "
			// 		  "/ Keyup: %d",
			// 		  glfwGetKeyName(e.m_KeyCode, e.m_ScanCode), e.m_Action,
			// 		  s_InputState.m_Keyboard.m_KeyDown[e.m_KeyCode],
			// 		  s_InputState.m_Keyboard.m_KeyHeld[e.m_KeyCode],
			// 		  s_InputState.m_Keyboard.m_KeyUp[e.m_KeyCode]);
			s_InputEventsBuffer.pop();
		}
	}

	void InputSystem::Shutdown() {}

} // namespace InputSystem
} // namespace Cookie
