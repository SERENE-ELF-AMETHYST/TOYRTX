#ifndef GLFW_H_
#define GLFW_H_

#include <Utility/Basic/Ptr.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <string>

namespace CppUtility {
	namespace Other {
		class Operation;
	}

	namespace OpenGL {

		class Glfw {
		public:
			static Glfw * GetInstance();
			//------------
			void Init(size_t width = 800, size_t height = 600, const std::string & title = "Title");
			void Terminate();
			void Run(Other::Ptr<Other::Operation> op);
			GLFWwindow * GetWindow();
			void CloseWindow();
			void LockCursor();
			int GetKey(int key);
		private:
			Glfw();
			Glfw(const Glfw &);
			Glfw& operator=(const Glfw &) = default;
			static Glfw * instance;
			//------------
			void GenWindow(size_t width = 800, size_t height = 600, const std::string & title = "title");
			void LoadGL();
			//------------
			GLFWwindow * window;
		};
	}
}
#endif