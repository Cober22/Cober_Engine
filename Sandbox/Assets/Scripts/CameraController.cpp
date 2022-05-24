// Native Scripting Example
/*
class CameraController : public ScriptableEntity {
public:
	virtual void OnCreate() override {
		// Test if OnCreate works well with different cameras 
		auto& translation = GetComponent<TransformComponent>().Translation;
		translation.x = rand() % 10 - 5.0f;

		redQuad = Find("RedQuad");
	}
	virtual void OnDestroy() override {

	}
	virtual void OnUpdate(Timestep ts) override {


		auto& translation = GetComponent<TransformComponent>().Translation;
		float speed = 5.0f;
		if (Input::IsKeyPressed(KeyCode::A))
			translation.x -= speed * ts;
		if (Input::IsKeyPressed(KeyCode::D))
			translation.x += speed * ts;
		if (Input::IsKeyPressed(KeyCode::W))
			translation.y += speed * ts;
		if (Input::IsKeyPressed(KeyCode::S))
			translation.y -= speed * ts;
		if (Input::IsKeyPressed(KeyCode::Q))
			if (redQuad)
				redQuad.GetComponent<TransformComponent>().Rotation.z += 5.0f;
	}
private:
	Entity redQuad;
};
*/