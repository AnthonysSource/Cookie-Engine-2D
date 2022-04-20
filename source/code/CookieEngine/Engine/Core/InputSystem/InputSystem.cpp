#include "InputSystem.h"

#include "Core/Types/Containers.h"
#include "Core/Types/PrimitiveTypes.h"

#include "Core/Logging/Log.h"

namespace Cookie {
	namespace InputSystem {

		void LogInput(InputEvent e);
		void WindowKeyEventHandle(GLFWwindow *window, int key, int scancode, int action, int mods);

		// Contains a buffer of the input events that have been
		// registered in the current frame
		TQueue<InputEvent> s_InputEventsBuffer;
		InputComponent g_InputComponent;

		void InputSystem::Update() {
			// Reset previous input state
			for (size_t i = 0; i < 255; i++) {
				// We dont reset keyheld because that gets
				// reset in a keyup event
				g_InputComponent.m_Keyboard.m_KeyDown[i] = false;
				g_InputComponent.m_Keyboard.m_KeyUp[i] = false;
			}

			// Poll All Window Events
			glfwPollEvents();

			// Process input events
			for (i32 i = 0; i < s_InputEventsBuffer.size(); i++) {
				InputEvent e = s_InputEventsBuffer.front();

				LogInput(e);
				// Logging to inspect system state
			}
		}
		
		void InputSystem::Shutdown() {}

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
				g_InputComponent.m_Keyboard.m_KeyDown[key] = true;
				g_InputComponent.m_Keyboard.m_KeyHeld[key] = true;
			} else if (action == 0) {
				g_InputComponent.m_Keyboard.m_KeyHeld[key] = false;
				g_InputComponent.m_Keyboard.m_KeyUp[key] = true;
			}
		}

		void InputSystem::Init(WindowData *window) { glfwSetKeyCallback(window->m_Handle, WindowKeyEventHandle); }

		void LogInput(InputEvent e) {
			CKE_LOG_INFO("Event Data -> Key Name: {} / Keycode: {} / Scancode : {} / Action: {} / Mod: {}",
						 glfwGetKeyName(e.m_KeyCode, e.m_ScanCode), e.m_KeyCode, e.m_ScanCode, e.m_Action, e.m_Mods);

			// if (e.m_Action == 0) {
			// 	CKE_LOG_INFO("[{}] Released", glfwGetKeyName(e.m_KeyCode, e.m_ScanCode));
			// } else if (e.m_Action == 1) {
			// 	CKE_LOG_INFO("[{}] Pressed", glfwGetKeyName(e.m_KeyCode, e.m_ScanCode));
			// }

			CKE_LOG_INFO("Input State Data -> Key Name: {} / Event Action: {} / Keydown: {} / Keyheld: "
						 "{} / Keyup: {}",
						 glfwGetKeyName(e.m_KeyCode, e.m_ScanCode), e.m_Action, g_InputComponent.m_Keyboard.m_KeyDown[e.m_KeyCode],
						 g_InputComponent.m_Keyboard.m_KeyHeld[e.m_KeyCode], g_InputComponent.m_Keyboard.m_KeyUp[e.m_KeyCode]);
			s_InputEventsBuffer.pop();
		}

	} // namespace InputSystem
} // namespace Cookie