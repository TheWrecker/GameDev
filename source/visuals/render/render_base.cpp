
#include "../scene.h"
#include "../presenter.h"

#include "render_base.h"

RenderBase::RenderBase(Scene* scene)
	:scene(scene), presenter(scene->GetPresenter())
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
	camera = scene->GetActiveCamera();
	state_master = scene->GetStateMaster();
	model_manager = scene->GetModelManager();
	texture_manager = scene->GetTextureManager();
	buffer_master = scene->GetBufferMaster();
	}

RenderBase::~RenderBase()
{
}

void RenderBase::Update()
{
}
