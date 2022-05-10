#include <Engine.h>
#include <Cober/EntryPoint.h>

#include "EditorLayer.h"

#include "checkML.h"

namespace Cober {

	class CoberEditor : public Application {

	public:
		CoberEditor()
			: Application("Cober Editor")
		{
			PushLayer(new EditorLayer());
		}

		~CoberEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new CoberEditor();
	}
}
