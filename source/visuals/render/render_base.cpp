
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/buffer_index.h"
#include "../elements/input_layout.h"
#include "../scene/scene.h"
#include "../presenter.h"

#include "render_base.h"

RenderBase::RenderBase(Presenter* parent)
	:scene(nullptr), presenter(parent)
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
}

RenderBase::~RenderBase()
{
}

bool RenderBase::Initialize()
{
	scene = presenter->QueryService<Scene*>("scene");

	if (!scene)
		return false;

	camera = scene->GetActiveCamera();
	state_master = scene->GetStateMaster();
	model_manager = scene->GetModelManager();
	texture_manager = scene->GetTextureManager();
	buffer_master = scene->GetBufferMaster();

	return true;
}

void RenderBase::Update()
{
}
