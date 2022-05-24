#pragma once

#include <glm/glm.hpp>
namespace Cober {

#define C_WHITE Vector3f	(1.0f,	1.0f,	1.0f)
#define C_RED Vector3f		(1.0f,	0.0f,	0.0f)
#define C_GREEN Vector3f	(0.0f,	1.0f,	0.0f)
#define C_CYAN Vector3f		(0.0f,	1.0f,	1.0f)
#define C_BLUE Vector3f		(0.0f,	0.0f,	1.0f)
#define C_YELLOW Vector3f	(1.0f,	1.0f,	0.0f)

	struct LightAttenuation {
		float Constant;
		float Linear;
		float Exp;

		LightAttenuation() {}
		LightAttenuation(float a, float b, float c)
			: Constant(a), Linear(b), Exp(c) {}
	};

	struct DirectionalLight {
		glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 Color = glm::vec3(1.0f);
		float AmbientIntensity = 1.0f;
		float DiffuseIntensity = 1.0f;
		bool Source = false;
		int index = 0;
		
		DirectionalLight() {}
		DirectionalLight(const glm::vec3& direction, const glm::vec3& color, 
						 float ambient, float diffuse) 
			: Direction(direction), Color(color), 
			AmbientIntensity(ambient), DiffuseIntensity(diffuse) {}
	};

	struct PointLight {
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Color = glm::vec3(1.0f);
		float AmbientIntensity = 1.0f;
		float DiffuseIntensity = 1.0f;
		LightAttenuation Attenuation{1.0f, 10.0f, 20.0f};
		bool Source = false;
		int index = 0;

		PointLight() {}
		PointLight(	const glm::vec3& position, const glm::vec3& color, 
					float ambient, float diffuse, 
					float linear, float exp) 
			:	Position(position), Color(color), 
			AmbientIntensity(ambient), DiffuseIntensity(diffuse),
			Attenuation({ 1.0f, linear, exp }) {}

		inline void SetPosition(const glm::vec3& position) { Position = position; }
	};

	struct SpotLight {
		glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Color = glm::vec3(1.0f);
		float CutOff = 10.0f;
		float OuterCutOff = 20.0f;
		float AmbientIntensity = 1.0f;
		float DiffuseIntensity = 1.0f;
		LightAttenuation Attenuation{ 1.0f, 10.0f, 20.0f };
		bool Source = false;
		int index = 0;

		SpotLight() {}
		SpotLight(	const glm::vec3 & direction, const glm::vec3 & position, const glm::vec3 & color, 
					float cutOff, float outerCutOff, 
					float ambient, float diffuse, 
					float linear, float exp) 
			: Direction(direction), Position(position), Color(color),
			CutOff(cutOff), OuterCutOff(outerCutOff), 
			AmbientIntensity(ambient), DiffuseIntensity(diffuse),
			Attenuation({ 1.0f, linear, exp }) {}

		inline void SetPosition(const glm::vec3& position) { Position = position; }
		inline void SetDirection(const glm::vec3& direction) { Direction = direction; }
	};
}
