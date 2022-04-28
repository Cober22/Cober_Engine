#pragma once

#include <filesystem>

namespace Cober {

	class ContentBrowserPanel
	{
	public:

		ContentBrowserPanel();
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
	};
}
