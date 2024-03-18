#pragma once
#include "model.h"
#include <memory>

namespace sve {

	struct Transform
	{
		glm::vec3 translation{ 0.0f,0.0f,0.0f };
		glm::vec3 scale{ 1.0f,1.0f, 1.0f };
		glm::vec3 rotation{ 0.0f,0.0f,0.0f };

		glm::mat4 mat4();;
	};

	class GameObject {
	public:
		using id_t = unsigned int;

		GameObject() = delete;
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		static GameObject create_gameobject() {
			static id_t cur_id = 0;
			return GameObject(cur_id++);
		};
		id_t get_id() const {
			return id_obj;
		}
		Transform transform;
		std::shared_ptr<Model> model{};
		glm::vec3 color;

	private:
		GameObject(id_t id) : id_obj(id), color({ 0.2f,0.2f,0.2f }) {};

		id_t id_obj;
	};

}