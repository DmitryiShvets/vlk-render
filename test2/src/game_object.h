#pragma once
#include "model.h"
#include <memory>

namespace sve {

	struct Transform2D
	{
		glm::vec2 translation{ 0.0f,0.0f };
		glm::vec2 scale{ 1.f,1.f };
		float rotation;

		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rot_mat{
				{c,s},
				{-s, c}
			};

			glm::mat2 scale_mat{
				{scale.x,0.0f},
				{0.0f, scale.y}
			};
			return rot_mat * scale_mat;
		};
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
		Transform2D transform;
		std::shared_ptr<Model> model{};
		glm::vec3 color;

	private:
		GameObject(id_t id) : id_obj(id), color({ 0.2f,0.2f,0.2f }) {};

		id_t id_obj;
	};

}