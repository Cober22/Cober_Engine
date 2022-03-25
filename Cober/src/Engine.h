#pragma once

// For use by Cober applications
#include "Cober/Application.h"
#include "Cober/Layers/Layer.h"
#include "Cober/Log.h"


// [---- DELTA TIME ----]
#include "Cober/Timestep.h"


// [---- INPUT SYSTEM ----]


// [---- UI ----]
#include "Cober/ImGui/ImGuiLayer.h"

// [---- Renderer ----]
#include "Cober/Renderer/Renderer.h"
#include "Cober/Renderer/API/RenderCommand.h"
#include "Cober/Renderer/Camera/OrthographicCameraController.h"
#include "Cober/Renderer/Camera/PerspectiveCameraController.h"

#include "Cober/Renderer/Buffer.h"
#include "Cober/Renderer/Shader.h"
#include "Cober/Renderer/VertexArray.h"
#include "Cober/Renderer/Texture.h"


// [---- CAMERA ----]
#include "Cober/Renderer/Camera/OrthographicCamera.h"
#include "Cober/Renderer/Camera/PerspectiveCamera.h"