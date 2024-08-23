#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

namespace test {
	
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}


	};

	class TestMenu : public Test {
	public:
		TestMenu(Test*& current);
		~TestMenu() {}

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name);

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

	};

	template<typename T>
	inline void TestMenu::RegisterTest(const std::string& name) {
		std::cout << "Registering test " << name << std::endl;
		m_Tests.push_back({ name,[]() {return new T();} });
	}
}